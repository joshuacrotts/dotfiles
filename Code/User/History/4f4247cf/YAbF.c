#include <stdio.h>
#include <stdlib.h>

#include "eval.h"
#include "mgc.h"
#include "mpc.h"
#include "parser.h"

#define NUM_RULES 14

#define MAX_TREES 10

/* Keeps track of all abstract available syntax trees. */
struct tree_buffer {
  size_t tree_count;
  mpc_ast_t *tree_buffer_array[MAX_TREES];
};

static struct tree_buffer trees;

/* Static function prototypes. */
static void parser_read(const char *contents, int mode);
static mpc_ast_t *parser_open(const char *contents, int mode);
static void parser_init_rules(void);

/* Global parser rule declarations. */
static mpc_parser_t *expr_rule;
static mpc_parser_t *application_rule;
static mpc_parser_t *datum_rule;
static mpc_parser_t *comment_rule;
static mpc_parser_t *number_rule;
static mpc_parser_t *boolean_rule;
static mpc_parser_t *symbol_rule;
static mpc_parser_t *string_rule;
static mpc_parser_t *character_rule;
static mpc_parser_t *quoted_rule;
static mpc_parser_t *quasiquoted_rule;
static mpc_parser_t *unquoted_rule;
static mpc_parser_t *unquote_splice_rule;
static mpc_parser_t *quasi_rule;

void
parser_init(void) {
  parser_init_rules();
}

void
parser_parse(const char *contents, int mode) {
  parser_read(contents, mode);
}

ast * 
parser_create_ast(const char *contents) {
  mpc_ast_t *mpc_ast = parser_open(contents, PARSE_STRING);
  trees.tree_buffer_array[trees.tree_count++] = mpc_ast;
  return ast_create(mpc_ast, mpc_ast->tag, mpc_ast->children_num, mpc_ast->contents);
}

void
parser_cleanup(void) {
  mpc_cleanup(NUM_RULES, expr_rule, comment_rule, application_rule, datum_rule, number_rule,
              boolean_rule, symbol_rule, string_rule, character_rule, quoted_rule, quasiquoted_rule,
              unquoted_rule, unquote_splice_rule, quasi_rule);

  for (int i = 0; i < trees.tree_count; i++) {
    mpc_ast_delete(trees.tree_buffer_array[i]);
  }
}

static void
parser_read(const char *contents, int mode) {
  mpc_ast_t *mpc_ast = parser_open(contents, mode);
  trees.tree_buffer_array[trees.tree_count++] = mpc_ast;
  ast *my_ast = ast_create(mpc_ast, mpc_ast->tag, mpc_ast->children_num, mpc_ast->contents);
  eval_ast(my_ast);
}

static mpc_ast_t *
parser_open(const char *contents, int mode) {
  mpc_result_t result;
  int code;
  // If the file is null or empty, we read from the terminal.
  if (mode == PARSE_STRING) {
    code = mpc_parse("input", contents, quasi_rule, &result);
  } else if (mode == PARSE_PIPE) {
    code = mpc_parse_pipe("<stdin>", stdin, quasi_rule, &result);
  } else if (mode == PARSE_FILE) {
    FILE *fp = fopen(contents, "r");
    // If an error occurs with the file, NULL is returned.
    if (fp == NULL) {
      perror("Error opening file");
      exit(EXIT_FAILURE);
    } else {
      code = mpc_parse_file(contents, fp, quasi_rule, &result);
    }

    // Close all file handles after opening.
    fclose(fp);
  } else {
    fprintf(stderr, "parse_read: Unknown mode %d\n", mode);
    exit(EXIT_FAILURE);
  }

  if (code <= 0) {
    mpc_err_print(result.error);
    mpc_err_delete(result.error);
    exit(EXIT_FAILURE);
  }
  return result.output;
}

static void
parser_init_rules(void) {
  /* First, define/instantiate the rules. */
  expr_rule = mpc_new("expr");
  application_rule = mpc_new("application");
  datum_rule = mpc_new("datum");
  comment_rule = mpc_new("comment");
  number_rule = mpc_new("number");
  boolean_rule = mpc_new("boolean");
  symbol_rule = mpc_new("symbol");
  string_rule = mpc_new("mystring");
  character_rule = mpc_new("character");
  quoted_rule = mpc_new("quoted");
  quasiquoted_rule = mpc_new("quasiquoted");
  unquoted_rule = mpc_new("unquoted");
  unquote_splice_rule = mpc_new("unquote_splice");
  quasi_rule = mpc_new("quasi");

  /* Now, define the grammar: */
  mpc_err_t *error =
      mpca_lang(MPCA_LANG_DEFAULT,
                "expr       : <comment>                                                          "
                "           \n"
                "              | <define>                                                        "
                "           \n"
                "              | <application>                                                   "
                "           \n"
                "              | <datum>                                                         "
                "         ; \n"
                "application: ('('<expr>*')')                                                    "
                "           \n"
                "              | ('{'<expr>*'}')                                                 "
                "           \n"
                "              | ('['<expr>*']')                                                 "
                "         ; \n"
                "datum      : <quoted>                                                           "
                "           \n"
                "              | <quasiquoted>                                                   "
                "           \n"
                "              | <unquoted>                                                      "
                "           \n"
                "              | <unquote_splice>                                                "
                "           \n"
                "              | <number>                                                        "
                "           \n"
                "              | <boolean>                                                       "
                "           \n"
                "              | <character>                                                     "
                "           \n"
                "              | <symbol>                                                        "
                "           \n"
                "              | <mystring>                                                      "
                "         ; \n"
                "quoted     : '\''<expr>                                                         "
                "         ; \n"
                "quasiquoted: '`'<expr>                                                          "
                "         ; \n"
                "unquote_splice: ',''@' <expr>                                                   "
                "         ; \n"
                "unquoted   : ','<expr>                                                          "
                "         ; \n"
                "symbol     : "
                "/[a-zA-Z\\-\\+_\\*<=>$:^\\/!\\?\\.][a-zA-Z0-9\\-\\+_\\*<=>$:^\\/!\\?\\.]*/    ; \n"
                "character  : '\\\\''#'/[a-zA-Z0-9\\-\\+_\\*<=>$\\/!\\?]/                       "
                "          ; \n"
                "mystring   : /\"(\\\\.|[^\"])*\"/                                               "
                "         ; \n"
                "boolean    : '#'('t'|'f')                                                       "
                "         ; \n"
                "number     : /(\\+|\\-)?[0-9]+(\\.[0-9]+)?/                                     "
                "         ; \n"
                "comment    : ';'/[^\\n]*/                                                       "
                "         ; \n"
                "quasi      : <expr>*                                                            "
                "         ; \n",
                expr_rule, comment_rule, application_rule, datum_rule, number_rule, boolean_rule,
                symbol_rule, string_rule, character_rule, quoted_rule, quasiquoted_rule,
                unquoted_rule, unquote_splice_rule, quasi_rule, NULL);

  if (error != NULL) {
    mpc_err_print(error);
    exit(EXIT_FAILURE);
  }
}
