#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "apply.h"
#include "env.h"
#include "error.h"
#include "gfx.h"
#include "mgc.h"
#include "parser.h"
#include "utils.h"

static struct sval *apply_printf(struct sval **args, size_t num_args, struct environment *env);
static struct sval *apply_exit(struct sval **args, size_t num_args, struct environment *env);
static struct sval *apply_error(struct sval **args, size_t num_args, struct environment *env);
static struct sval *apply_read_string(struct sval **args, size_t num_args, struct environment *env);
static struct sval *apply_read_number(struct sval **args, size_t num_args, struct environment *env);
static struct sval *apply_addition(struct sval **args, size_t num_args, struct environment *env);
static struct sval *apply_subtraction(struct sval **args, size_t num_args, struct environment *env);
static struct sval *apply_multiplication(struct sval **args, size_t num_args,
                                         struct environment *env);
static struct sval *apply_division(struct sval **args, size_t num_args, struct environment *env);
static struct sval *apply_pow(struct sval **args, size_t num_args, struct environment *env);
static struct sval *apply_floor(struct sval **args, size_t num_args, struct environment *env);
static struct sval *apply_ceil(struct sval **args, size_t num_args, struct environment *env);
static struct sval *apply_sin(struct sval **args, size_t num_args, struct environment *env);
static struct sval *apply_cos(struct sval **args, size_t num_args, struct environment *env);
static struct sval *apply_num_eq(struct sval **args, size_t num_args, struct environment *env);
static struct sval *apply_num_lt(struct sval **args, size_t num_args, struct environment *env);
static struct sval *apply_num_leq(struct sval **args, size_t num_args, struct environment *env);
static struct sval *apply_num_gt(struct sval **args, size_t num_args, struct environment *env);
static struct sval *apply_num_geq(struct sval **args, size_t num_args, struct environment *env);
static struct sval *apply_car(struct sval **args, size_t num_args, struct environment *env);
static struct sval *apply_cdr(struct sval **args, size_t num_args, struct environment *env);
static struct sval *apply_cons(struct sval **args, size_t num_args, struct environment *env);
static struct sval *apply_number_to_string(struct sval **args, size_t num_args,
                                           struct environment *env);
static struct sval *apply_string_to_number(struct sval **args, size_t num_args,
                                           struct environment *env);
static struct sval *apply_number_to_char(struct sval **args, size_t num_args,
                                         struct environment *env);
static struct sval *apply_char_to_number(struct sval **args, size_t num_args,
                                         struct environment *env);
static struct sval *apply_char_to_string(struct sval **args, size_t num_args,
                                         struct environment *env);
static struct sval *apply_symbol_to_string(struct sval **args, size_t num_args,
                                           struct environment *env);
static struct sval *apply_string_to_symbol(struct sval **args, size_t num_args,
                                           struct environment *env);
static struct sval *apply_symbol_eq(struct sval **args, size_t num_args, struct environment *env);
static struct sval *apply_boolean_eq(struct sval **args, size_t num_args, struct environment *env);
static struct sval *apply_char_eq(struct sval **args, size_t num_args, struct environment *env);
static struct sval *apply_string_eq(struct sval **args, size_t num_args, struct environment *env);
static struct sval *apply_string_length(struct sval **args, size_t num_args,
                                        struct environment *env);
static struct sval *apply_string_append(struct sval **args, size_t num_args,
                                        struct environment *env);
static struct sval *apply_string_char_at(struct sval **args, size_t num_args,
                                         struct environment *env);
static struct sval *apply_string_index_of(struct sval **args, size_t num_args,
                                          struct environment *env);
static struct sval *apply_string_substring(struct sval **args, size_t num_args,
                                           struct environment *env);
static struct sval *apply_number_predicate(struct sval **args, size_t num_args,
                                           struct environment *env);
static struct sval *apply_symbol_predicate(struct sval **args, size_t num_args,
                                           struct environment *env);
static struct sval *apply_boolean_predicate(struct sval **args, size_t num_args,
                                            struct environment *env);
static struct sval *apply_string_predicate(struct sval **args, size_t num_args,
                                           struct environment *env);
static struct sval *apply_char_predicate(struct sval **args, size_t num_args,
                                         struct environment *env);
static struct sval *apply_cons_predicate(struct sval **args, size_t num_args,
                                         struct environment *env);
static struct sval *apply_null_predicate(struct sval **args, size_t num_args,
                                         struct environment *env);
static struct sval *apply_procedure_predicate(struct sval **args, size_t num_args,
                                              struct environment *env);
static struct sval *apply_vector_predicate(struct sval **args, size_t num_args,
                                           struct environment *env);
static struct sval *apply_eq_predicate(struct sval **args, size_t num_args,
                                       struct environment *env);
static struct sval *apply_eqv_predicate(struct sval **args, size_t num_args,
                                        struct environment *env);
static struct sval *apply_make_vector(struct sval **args, size_t num_args, struct environment *env);
static struct sval *apply_vector_set(struct sval **args, size_t num_args, struct environment *env);
static struct sval *apply_vector_get(struct sval **args, size_t num_args, struct environment *env);
static struct sval *apply_include(struct sval **args, size_t num_args, struct environment *env);
static struct sval *apply_file_open(struct sval **args, size_t num_args, struct environment *env);
static struct sval *apply_file_close(struct sval **args, size_t num_args, struct environment *env);
static struct sval *apply_file_read_line(struct sval **args, size_t num_args,
                                         struct environment *env);
static struct sval *apply_file_read_integer(struct sval **args, size_t num_args,
                                            struct environment *env);
static struct sval *apply_file_write_line(struct sval **args, size_t num_args,
                                          struct environment *env);
static struct sval *apply_file_write_integer(struct sval **args, size_t num_args,
                                             struct environment *env);
static struct sval *apply_file_has_next_predicate(struct sval **args, size_t num_args,
                                                  struct environment *env);
static struct sval *apply_file_reset(struct sval **args, size_t num_args, struct environment *env);
static struct sval *apply_gfx_open(struct sval **args, size_t num_args, struct environment *env);
static struct sval *apply_gfx_delay(struct sval **args, size_t num_args, struct environment *env);
static struct sval *apply_gfx_refresh(struct sval **args, size_t num_args, struct environment *env);
static struct sval *apply_gfx_clear(struct sval **args, size_t num_args, struct environment *env);
static struct sval *apply_gfx_poll(struct sval **args, size_t num_args, struct environment *env);
static struct sval *apply_gfx_time_ns(struct sval **args, size_t num_args, struct environment *env);
static struct sval *apply_gfx_draw_line(struct sval **args, size_t num_args,
                                        struct environment *env);
static struct sval *apply_gfx_draw_circle(struct sval **args, size_t num_args,
                                          struct environment *env);

void
builtin_functions_init(struct environment *env) {
  environment_put(env, "printf", sval_builtin_create(apply_printf));
  environment_put(env, "exit", sval_builtin_create(apply_exit));
  environment_put(env, "error", sval_builtin_create(apply_error));
  environment_put(env, "read-string", sval_builtin_create(apply_read_string));
  environment_put(env, "read-number", sval_builtin_create(apply_read_number));
  environment_put(env, "+", sval_builtin_create(apply_addition));
  environment_put(env, "-", sval_builtin_create(apply_subtraction));
  environment_put(env, "*", sval_builtin_create(apply_multiplication));
  environment_put(env, "/", sval_builtin_create(apply_division));
  environment_put(env, "pow", sval_builtin_create(apply_pow));
  environment_put(env, "floor", sval_builtin_create(apply_floor));
  environment_put(env, "ceil", sval_builtin_create(apply_ceil));
  environment_put(env, "sin", sval_builtin_create(apply_sin));
  environment_put(env, "cos", sval_builtin_create(apply_cos));
  environment_put(env, "=", sval_builtin_create(apply_num_eq));
  environment_put(env, "<", sval_builtin_create(apply_num_lt));
  environment_put(env, "<=", sval_builtin_create(apply_num_leq));
  environment_put(env, ">", sval_builtin_create(apply_num_gt));
  environment_put(env, ">=", sval_builtin_create(apply_num_geq));
  environment_put(env, "car", sval_builtin_create(apply_car));
  environment_put(env, "cdr", sval_builtin_create(apply_cdr));
  environment_put(env, "cons", sval_builtin_create(apply_cons));
  environment_put(env, "number->string", sval_builtin_create(apply_number_to_string));
  environment_put(env, "string->number", sval_builtin_create(apply_string_to_number));
  environment_put(env, "number->char", sval_builtin_create(apply_number_to_char));
  environment_put(env, "char->number", sval_builtin_create(apply_char_to_number));
  environment_put(env, "symbol->string", sval_builtin_create(apply_symbol_to_string));
  environment_put(env, "string->symbol", sval_builtin_create(apply_string_to_symbol));
  environment_put(env, "symbol=?", sval_builtin_create(apply_symbol_eq));
  environment_put(env, "boolean=?", sval_builtin_create(apply_boolean_eq));
  environment_put(env, "char=?", sval_builtin_create(apply_char_eq));
  environment_put(env, "string=?", sval_builtin_create(apply_string_eq));
  environment_put(env, "string-length", sval_builtin_create(apply_string_length));
  environment_put(env, "string-append", sval_builtin_create(apply_string_append));
  environment_put(env, "char-at", sval_builtin_create(apply_string_char_at));
  environment_put(env, "substring", sval_builtin_create(apply_string_substring));
  environment_put(env, "index-of", sval_builtin_create(apply_string_index_of));
  environment_put(env, "number?", sval_builtin_create(apply_number_predicate));
  environment_put(env, "symbol?", sval_builtin_create(apply_symbol_predicate));
  environment_put(env, "boolean?", sval_builtin_create(apply_boolean_predicate));
  environment_put(env, "char?", sval_builtin_create(apply_char_predicate));
  environment_put(env, "string?", sval_builtin_create(apply_string_predicate));
  environment_put(env, "procedure?", sval_builtin_create(apply_procedure_predicate));
  environment_put(env, "cons?", sval_builtin_create(apply_cons_predicate));
  environment_put(env, "null?", sval_builtin_create(apply_null_predicate));
  environment_put(env, "eq?", sval_builtin_create(apply_eq_predicate));
  environment_put(env, "eqv?", sval_builtin_create(apply_eqv_predicate));
  environment_put(env, "make-vector", sval_builtin_create(apply_make_vector));
  environment_put(env, "vector-set!", sval_builtin_create(apply_vector_set));
  environment_put(env, "vector-get", sval_builtin_create(apply_vector_get));
  environment_put(env, "vector?", sval_builtin_create(apply_vector_predicate));
  environment_put(env, "include", sval_builtin_create(apply_include));
  environment_put(env, "file-open!", sval_builtin_create(apply_file_open));
  environment_put(env, "file-close!", sval_builtin_create(apply_file_close));
  environment_put(env, "file-read-line!", sval_builtin_create(apply_file_read_line));
  environment_put(env, "file-read-integer!", sval_builtin_create(apply_file_read_integer));
  environment_put(env, "file-write-line!", sval_builtin_create(apply_file_write_line));
  environment_put(env, "file-write-integer!", sval_builtin_create(apply_file_write_integer));
  environment_put(env, "file-has-next?", sval_builtin_create(apply_file_has_next_predicate));
  environment_put(env, "file-reset!", sval_builtin_create(apply_file_reset));
  environment_put(env, "gfx-open", sval_builtin_create(apply_gfx_open));
  environment_put(env, "gfx-delay", sval_builtin_create(apply_gfx_delay));
  environment_put(env, "gfx-poll", sval_builtin_create(apply_gfx_poll));
  environment_put(env, "gfx-refresh", sval_builtin_create(apply_gfx_refresh));
  environment_put(env, "gfx-clear", sval_builtin_create(apply_gfx_clear));
  environment_put(env, "gfx-time-ns", sval_builtin_create(apply_gfx_time_ns));
  environment_put(env, "gfx-line", sval_builtin_create(apply_gfx_draw_line));
  environment_put(env, "gfx-circle", sval_builtin_create(apply_gfx_draw_circle));
}

struct sval *
apply(const struct sval *function, struct sval **args, size_t num_args, struct environment *env) {
  return (function->data.builtin)(args, num_args, env);
}

struct sval *
apply_printf(struct sval **args, size_t num_args, struct environment *env) {
  if (num_args == 0) {
    fprintf(stderr, "printf requires at least one argument but got 0\n");
    exit(EXIT_FAILURE);
  }

  char *fstr = args[0]->data.string;
  size_t len = strlen(fstr);
  size_t arg_idx = 1;

  // Traverse through the string to find any occurrence of ~
  for (int i = 0; i < len; i++) {
    char curr = fstr[i];
    // If there is at least one more character available after a tilde, scan it.
    if (i < len - 1) {
      char next = fstr[i + 1];
      if (curr == '~') {
        switch (next) {
        case 'n':
          printf("\n");
          break;
        case '\"':
          printf("\"");
          break;
        case 'r':
          printf("\r");
          break;
        case '~':
          printf("~");
          break;
        case 's':
          printf("%s", args[arg_idx++]->data.string);
          break;
        case 'a':
          sval_print(args[arg_idx++]);
          break;
        default:
          printf("printf: unknown format specifier %c\n", next);
          exit(EXIT_FAILURE);
        }
        i++;
      }
    }
    // Do not print a raw tilde if it exists.
    if (curr != '~' && curr != '\"') {
      printf("%c", curr);
    }
  }
  return NULL;
}

struct sval *
apply_exit(struct sval **args, size_t num_args, struct environment *env) {
  if (0 != num_args) {
    ASSERT_ARITY("exit", 0, num_args);
    exit(EXIT_FAILURE);
  } else {
    exit(EXIT_SUCCESS);
  }
}

struct sval *
apply_error(struct sval **args, size_t num_args, struct environment *env) {
  if (1 != num_args) {
    ASSERT_ARITY("error", 1, num_args);
    exit(EXIT_FAILURE);
  } else {
    printf("err: %s\n", args[0]->data.string);
    exit(EXIT_FAILURE);
  }
}

struct sval *
apply_read_string(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("read-string", 0, num_args);

  char *str = NULL;
  size_t n_chars;
  if (getline(&str, &n_chars, stdin) == -1) {
    fprintf(stderr, "read-string: expected an input but received nothing.\n");
    exit(EXIT_FAILURE);
  }

  str[strlen(str) - 1] = '\0';
  return sval_string_create(str);
}

struct sval *
apply_read_number(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("read-number", 0, num_args);

  char num[PREC];
  if (fgets(num, PREC, stdin) == NULL) {
    fprintf(stderr, "read-number: expected an input but received nothing.\n");
    exit(EXIT_FAILURE);
  }

  return sval_number_create_bn(bignum_create_str(num));
}

static struct sval *
apply_addition(struct sval **args, size_t num_args, struct environment *env) {
  struct sval *result = sval_number_create("0");
  for (int i = 0; i < num_args; i++) {
    if (args[i]->type != SVAL_NUMBER) {
      fprintf(stderr, "+ expected number; did not receive number");
      exit(EXIT_FAILURE);
    }
    bignum_add(result->data.number, args[i]->data.number);
  }
  return result;
}

static struct sval *
apply_subtraction(struct sval **args, size_t num_args, struct environment *env) {
  struct sval *difference = sval_number_create_bn(args[0]->data.number);

  // // Unary negation.
  if (num_args == 1) {
    bignum_neg(difference->data.number);
  }

  // // Subtraction.
  for (int i = 1; i < num_args; i++) {
    bignum_sub(difference->data.number, args[i]->data.number);
  }

  return difference;
}

static struct sval *
apply_multiplication(struct sval **args, size_t num_args, struct environment *env) {
  struct sval *result = sval_number_create("1");
  for (int i = 0; i < num_args; i++) {
    bignum_mul(result->data.number, args[i]->data.number);
  }
  return result;
}

static struct sval *
apply_division(struct sval **args, size_t num_args, struct environment *env) {
  struct sval *quotient = sval_number_create_bn(args[0]->data.number);
  for (int i = 1; i < num_args; i++) {
    bignum *value = args[i]->data.number;
    if (bignum_is_zero(value)) {
      fprintf(stderr, "/ by zero\n");
      exit(EXIT_FAILURE);
    }
    bignum_div(quotient->data.number, value);
  }
  return quotient;
}

static struct sval *
apply_pow(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("pow", 2, num_args);
  struct sval *operand = sval_number_create_bn(args[0]->data.number);
  bignum_pow(operand->data.number, args[1]->data.number);
  return operand;
}

static struct sval *
apply_floor(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("floor", 1, num_args);
  struct sval *operand = sval_number_create_bn(args[0]->data.number);
  ASSERT_TYPE(SVAL_NUMBER, operand->type);
  bignum_floor(operand->data.number);
  return operand;
}

static struct sval *
apply_ceil(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("ceil", 1, num_args);
  struct sval *operand = sval_number_create_bn(args[0]->data.number);
  ASSERT_TYPE(SVAL_NUMBER, operand->type);
  bignum_ceil(operand->data.number);
  return operand;
}

static struct sval *
apply_sin(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("sin", 1, num_args);
  struct sval *operand = sval_number_create_bn(args[0]->data.number);
  bignum_sin(operand->data.number);
  return operand;
}

static struct sval *
apply_cos(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("cos", 1, num_args);
  struct sval *operand = sval_number_create_bn(args[0]->data.number);
  bignum_cos(operand->data.number);
  return operand;
}

static struct sval *
apply_num_eq(struct sval **args, size_t num_args, struct environment *env) {
  // If we only have one argument then that is an error.
  if (num_args < 2) {
    fprintf(stderr, "= expects two arguments but got %zu\n", num_args);
    exit(EXIT_FAILURE);
  }

  bool eq_result = true;
  struct sval *curr_number = args[0];
  // TODO Type checking!

  // Iterate over each number and make sure they're equivalent.
  for (int i = 1; i < num_args; i++) {
    if (!bignum_equal(curr_number->data.number, args[i]->data.number)) {
      eq_result = false;
      break;
    }
    curr_number = args[i];
  }

  return sval_boolean_create(eq_result);
}

static struct sval *
apply_num_lt(struct sval **args, size_t num_args, struct environment *env) {
  if (num_args < 2) {
    fprintf(stderr, "< expects two arguments but got %zu\n", num_args);
    exit(EXIT_FAILURE);
  }

  bool lt_result = true;

  // TODO add type checking.
  struct sval *curr_number = args[0];
  for (int i = 1; i < num_args; i++) {
    if (bignum_cmp(curr_number->data.number, args[i]->data.number) >= 0) {
      lt_result = false;
      break;
    }
    curr_number = args[i];
  }

  return sval_boolean_create(lt_result);
}

static struct sval *
apply_num_leq(struct sval **args, size_t num_args, struct environment *env) {
  if (num_args < 2) {
    fprintf(stderr, "<= expects two arguments but got %zu\n", num_args);
    exit(EXIT_FAILURE);
  }

  bool leq_result = true;

  // TODO add type checking.
  struct sval *curr_number = args[0];
  for (int i = 1; i < num_args; i++) {
    if (bignum_cmp(curr_number->data.number, args[i]->data.number) > 0) {
      leq_result = false;
      break;
    }
    curr_number = args[i];
  }

  return sval_boolean_create(leq_result);
}

static struct sval *
apply_num_gt(struct sval **args, size_t num_args, struct environment *env) {
  if (num_args < 2) {
    fprintf(stderr, "> expects two arguments but got %zu\n", num_args);
    exit(EXIT_FAILURE);
  }

  bool gt_result = true;

  // TODO add type checking.
  struct sval *curr_number = args[0];
  for (int i = 1; i < num_args; i++) {
    if (bignum_cmp(curr_number->data.number, args[i]->data.number) <= 0) {
      gt_result = false;
      break;
    }
    curr_number = args[i];
  }

  return sval_boolean_create(gt_result);
}

static struct sval *
apply_num_geq(struct sval **args, size_t num_args, struct environment *env) {
  if (num_args < 2) {
    fprintf(stderr, ">= expects two arguments but got %zu\n", num_args);
    exit(EXIT_FAILURE);
  }

  bool geq_result = true;

  // TODO add type checking.
  struct sval *curr_number = args[0];
  for (int i = 1; i < num_args; i++) {
    if (bignum_cmp(curr_number->data.number, args[i]->data.number) < 0) {
      geq_result = false;
      break;
    }
    curr_number = args[i];
  }

  return sval_boolean_create(geq_result);
}

static struct sval *
apply_car(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("car", 1, num_args);
  struct sval *sv = args[0];
  return sv->data.pair.car;
}

static struct sval *
apply_cdr(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("cdr", 1, num_args);
  struct sval *sv = args[0];
  return sv->data.pair.cdr;
}

static struct sval *
apply_cons(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("cons", 2, num_args);
  return sval_pair_create(args[0], args[1]);
}

static struct sval *
apply_number_to_string(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("number->string", 1, num_args);
  return sval_string_create(bignum_to_string(args[0]->data.number));
}

static struct sval *
apply_string_to_number(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("string->number", 1, num_args);
  return sval_number_create(args[0]->data.string);
}

static struct sval *
apply_number_to_char(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("number->char", 1, num_args);
  bignum *number = args[0]->data.number;
  int character_value = bignum_to_integer(number);
  return sval_character_create(character_value);
}

static struct sval *
apply_char_to_number(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("char->number", 1, num_args);
  struct sval *character = args[0];
  bignum *number = bignum_create_long(character->data.character);
  return sval_number_create_bn(number);
}

static struct sval *
apply_symbol_to_string(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("symbol->string", 1, num_args);
  struct sval *sym = args[0];
  return sval_string_create(sym->data.symbol);
}

static struct sval *
apply_string_to_symbol(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("string->symbol", 1, num_args);
  struct sval *str = args[0];
  return sval_symbol_create(str->data.string);
}

static struct sval *
apply_boolean_eq(struct sval **args, size_t num_args, struct environment *env) {
  if (num_args < 2) {
    fprintf(stderr, "boolean=? expects at least two arguments but got %zu\n", num_args);
    exit(EXIT_FAILURE);
  }

  bool eq_result = true;
  struct sval *curr_boolean = args[0];
  for (int i = 1; i < num_args; i++) {
    if (curr_boolean->data.boolean != args[i]->data.boolean) {
      eq_result = false;
      break;
    }
    curr_boolean = args[i];
  }

  return sval_boolean_create(eq_result);
}

static struct sval *
apply_symbol_eq(struct sval **args, size_t num_args, struct environment *env) {
  if (num_args < 2) {
    fprintf(stderr, "symbol=? expects at least two arguments but got %zu\n", num_args);
    exit(EXIT_FAILURE);
  }

  bool eq_result = true;
  struct sval *curr_symbol = args[0];
  for (int i = 1; i < num_args; i++) {
    if (strcmp(curr_symbol->data.symbol, args[i]->data.symbol) != 0) {
      eq_result = false;
      break;
    }
    curr_symbol = args[i];
  }

  return sval_boolean_create(eq_result);
}

static struct sval *
apply_char_eq(struct sval **args, size_t num_args, struct environment *env) {
  if (num_args < 2) {
    fprintf(stderr, "char=? expects at least two arguments but got %zu\n", num_args);
    exit(EXIT_FAILURE);
  }

  bool eq_result = true;
  struct sval *curr_char = args[0];
  for (int i = 1; i < num_args; i++) {
    if (curr_char->data.character != args[i]->data.character) {
      eq_result = false;
      break;
    }
    curr_char = args[i];
  }

  return sval_boolean_create(eq_result);
}

static struct sval *
apply_string_length(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("string-length", 1, num_args);
  return sval_number_create_bn(bignum_create_long(strlen(args[0]->data.string)));
}

static struct sval *
apply_string_append(struct sval **args, size_t num_args, struct environment *env) {
  // Must have at least one character due to the \0.
  size_t num_characters = 1;

  // First compute the number of characters needed to copy.
  for (int i = 0; i < num_args; i++) {
    num_characters += strlen(args[i]->data.string);
  }

  // Allocate the new string array then copy over the chars.
  // NUL-byte is accounted for in size.
  char *new_str = mgc_alloc(num_characters, free);
  size_t i = 0;
  size_t curr_str = 0;
  for (int cs = 0; cs < num_args; cs++) {
    for (int j = 0; j < strlen(args[cs]->data.string); j++) {
      new_str[i++] = args[cs]->data.string[j];
    }
  }

  new_str[num_characters - 1] = '\0';
  return sval_string_create(new_str);
}

static struct sval *
apply_string_char_at(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("char-at", 2, num_args);
  char *str = args[0]->data.string;
  int idx = bignum_to_integer(args[1]->data.number);
  return sval_character_create(str[idx]);
}

static struct sval *
apply_string_substring(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("substring", 3, num_args);
  char *str = args[0]->data.string;
  int m = bignum_to_integer(args[1]->data.number);
  int n = bignum_to_integer(args[2]->data.number);

  // Create a string of length n - m + 1 for NUL-byte.
  size_t num_characters = n - m;
  char *new_str = mgc_alloc(num_characters + 1, free);

  // Copy each character over.
  for (int i = 0; i < num_characters; i++) {
    new_str[i] = str[m + i];
  }
  new_str[num_characters] = '\0';
  return sval_string_create(new_str);
}

static struct sval *
apply_string_index_of(struct sval **args, size_t num_args, struct environment *env) {
  char *s1 = args[0]->data.string;
  char *s2 = args[1]->data.string;
  char *sub = strstr(s1, s2);
  if (sub == NULL) {
    return sval_number_create_bn(bignum_create_long(-1));
  } else {
    return sval_number_create_bn(bignum_create_long(sub - s1));
  }
}

static struct sval *
apply_string_eq(struct sval **args, size_t num_args, struct environment *env) {
  if (num_args < 2) {
    fprintf(stderr, "string=? expects at least two arguments but got %zu\n", num_args);
    exit(EXIT_FAILURE);
  }

  bool eq_result = true;
  struct sval *curr_string = args[0];
  for (int i = 1; i < num_args; i++) {
    if (strcmp(curr_string->data.string, args[i]->data.string) != 0) {
      eq_result = false;
      break;
    }
    curr_string = args[i];
  }

  return sval_boolean_create(eq_result);
}

static struct sval *
apply_number_predicate(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("number?", 1, num_args);
  return sval_boolean_create(args[0]->type == SVAL_NUMBER);
}

static struct sval *
apply_symbol_predicate(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("symbol?", 1, num_args);
  return sval_boolean_create(args[0]->type == SVAL_SYMBOL);
}

static struct sval *
apply_boolean_predicate(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("boolean?", 1, num_args);
  return sval_boolean_create(args[0]->type == SVAL_BOOLEAN);
}

static struct sval *
apply_char_predicate(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("char?", 1, num_args);
  return sval_boolean_create(args[0]->type == SVAL_CHARACTER);
}

static struct sval *
apply_string_predicate(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("string?", 1, num_args);
  return sval_boolean_create(args[0]->type == SVAL_STRING);
}

static struct sval *
apply_cons_predicate(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("cons?", 1, num_args);
  return sval_boolean_create(args[0]->type == SVAL_PAIR && args[0]->data.pair.car != NULL);
}

static struct sval *
apply_eq_predicate(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("eq?", 2, num_args);
  if (args[0]->type != args[1]->type) {
    return sval_boolean_create(false);
  } else {
    // Compare the pointer results.
    if (args[0] == args[1]) {
      // printf("Here?1\n");
      return sval_boolean_create(true);
    } else if (args[0]->data.number == args[1]->data.number) {
      return sval_boolean_create(true);
    } else if (streq(args[0]->data.symbol, args[1]->data.symbol)) {
      return sval_boolean_create(true);
    } else {
      return sval_boolean_create(false);
    }
  }
}

static struct sval *
apply_eqv_predicate(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("eqv?", 2, num_args);
  // If they do not share the same type then they're automatically not eqv.
  if (args[0]->type != args[1]->type) {
    return sval_boolean_create(false);
  } else {
    // The types must match so we can switch on the first.
    switch (args[0]->type) {
    case SVAL_NUMBER:
      return apply_num_eq(args, num_args, env);
    case SVAL_BOOLEAN:
      return apply_boolean_eq(args, num_args, env);
    case SVAL_CHARACTER:
      return apply_char_eq(args, num_args, env);
    case SVAL_STRING:
      return apply_string_eq(args, num_args, env);
    case SVAL_SYMBOL:
      return apply_symbol_eq(args, num_args, env);
    case SVAL_PAIR: {
      struct sval *ls1 = args[0];
      struct sval *ls2 = args[1];
      return sval_boolean_create(ls1->data.pair.car == NULL && ls2->data.pair.car == NULL);
    }
    default:
      return sval_boolean_create(false);
    }
  }
}

static struct sval *
apply_procedure_predicate(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("procedure?", 1, num_args);
  return sval_boolean_create(args[0]->type == SVAL_PROCEDURE || args[0]->type == SVAL_BUILTIN);
}

static struct sval *
apply_null_predicate(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("null?", 1, num_args);
  bool null_result = false;

  // The predicate needs to be both a list and have nothing in it for null? to be true.
  if (args[0]->type == SVAL_PAIR && args[0]->data.pair.car == NULL &&
      args[0]->data.pair.cdr == NULL) {
    null_result = true;
  }
  return sval_boolean_create(null_result);
}

static struct sval *
apply_make_vector(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("make-vector", 1, num_args);
  return sval_vector_create(bignum_to_integer(args[0]->data.number));
}

static struct sval *
apply_vector_set(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("vector-set!", 3, num_args);
  struct sval *vector_arg = args[0];
  struct sval *index_arg = args[1];
  struct sval *element_arg = args[2];

  int idx = bignum_to_integer(index_arg->data.number);
  vector_arg->data.vector.elements[idx] = element_arg;
  return NULL;
}

static struct sval *
apply_vector_get(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("vector-get", 2, num_args);
  struct sval *vector_arg = args[0];
  struct sval *index_arg = args[1];
  int idx = bignum_to_integer(index_arg->data.number);
  return vector_arg->data.vector.elements[idx];
}

static struct sval *
apply_vector_predicate(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("vector?", 1, num_args);
  return sval_boolean_create(args[0]->type == SVAL_VECTOR);
}

struct sval *
apply_include(struct sval **args, size_t num_args, struct environment *env) {
  if (num_args != 1) {
    fprintf(stderr, "include expects one argument but got %zu\n", num_args);
    exit(EXIT_FAILURE);
  }
  parser_parse(args[0]->data.string, PARSE_FILE);
  return NULL;
}

static struct sval *
apply_file_open(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("file-open", 2, num_args);
  char *fname = args[0]->data.string;
  char *fmode = args[1]->data.string;
  return sval_file_create(fname, fmode);
}

static struct sval *
apply_file_close(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("file-close", 1, num_args);
  struct sval *f = args[0];
  f->data.file.is_open = false;
  fclose(f->data.file.fp);
  return NULL;
}

static struct sval *
apply_file_read_line(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("file-read-line!", 1, num_args);

  // Read the current line into a dynamic buffer.
  struct sval *fsv = args[0];
  char *line = NULL;
  size_t sz;
  ssize_t nchars = getline(&line, &sz, fsv->data.file.fp);

  // If we read nothing, just return an empty string.
  if (nchars < 0) {
    free(line);
    return sval_string_create("");
  } else {
    // Remove '\n', create a garbage-collected string.
    line[nchars - 1] = '\0';
    char *nline = mgc_alloc(nchars, free);
    strncpy(nline, line, nchars);
    free(line);
    return sval_string_create(nline);
  }
}

static struct sval *
apply_file_read_integer(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("file-read-integer!", 1, num_args);

  // Scan the current item assuming its an integer.
  struct sval *fsv = args[0];
  int n;
  size_t sz;
  size_t nchars = fscanf(fsv->data.file.fp, "%d", &n);

  // Set the FILE position to be the current position + number of chars read.
  fsv->data.file.position = fseek(fsv->data.file.fp, nchars, SEEK_CUR);
  return sval_number_create_bn(bignum_create_long(n));
}

static struct sval *
apply_file_write_line(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("file-write-line!", 2, num_args);
  struct sval *fsv = args[0];
  struct sval *line = args[1];
  fprintf(fsv->data.file.fp, "%s\n", line->data.string);
  return NULL;
}

static struct sval *
apply_file_write_integer(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("file-write-integer!", 2, num_args);
  struct sval *fsv = args[0];
  struct sval *n = args[1];
  fprintf(fsv->data.file.fp, "%d", bignum_to_integer(n->data.number));
  return NULL;
}

static struct sval *
apply_file_has_next_predicate(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("file-has-next?", 1, num_args);
  struct sval *fsv = args[0];
  return sval_boolean_create(!feof(fsv->data.file.fp));
}

static struct sval *
apply_file_reset(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("file-reset!", 1, num_args);
  struct sval *fsv = args[0];
  fseek(fsv->data.file.fp, 0, SEEK_SET);
  return NULL;
}

static struct sval *
apply_gfx_open(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("gfx-open", 3, num_args);
  const char *title = args[0]->data.string;
  const size_t width = bignum_to_integer(args[1]->data.number);
  const size_t height = bignum_to_integer(args[2]->data.number);
  gfx_open_window(title, width, height);
  return NULL;
}

static struct sval *
apply_gfx_delay(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("gfx-delay", 1, num_args);
  gfx_delay(bignum_to_integer(args[0]->data.number));
  return NULL;
}

static struct sval *
apply_gfx_clear(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("gfx-clear", 0, num_args);
  gfx_clear();
  return NULL;
}

static struct sval *
apply_gfx_refresh(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("gfx-refresh", 0, num_args);
  gfx_refresh();
  return NULL;
}

static struct sval *
apply_gfx_poll(struct sval **args, size_t num_args, struct environment *env) {
  gfx_poll_events();
  return NULL;
}

static struct sval *
apply_gfx_time_ns(struct sval **args, size_t num_args, struct environment *env) {
  return sval_number_create_bn(bignum_create_long(gfx_time_ns()));
}

static struct sval *
apply_gfx_draw_line(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("gfx-draw-line", 8, num_args);
  const bignum *x1 = args[0]->data.number;
  const bignum *y1 = args[1]->data.number;
  const bignum *x2 = args[2]->data.number;
  const bignum *y2 = args[3]->data.number;

  int r = bignum_to_integer(args[4]->data.number);
  int g = bignum_to_integer(args[5]->data.number);
  int b = bignum_to_integer(args[6]->data.number);
  int a = bignum_to_integer(args[7]->data.number);

  gfx_draw_line(bignum_to_double(x1), bignum_to_double(y1), bignum_to_double(x2),
                bignum_to_double(y2), GET_COLOR(r, g, b, a));
  return NULL;
}

static struct sval *
apply_gfx_draw_circle(struct sval **args, size_t num_args, struct environment *env) {
  ASSERT_ARITY("gfx-draw-circle", 7, num_args);
  const bignum *cx = args[0]->data.number;
  const bignum *cy = args[1]->data.number;
  const bignum *rad = args[2]->data.number;

  int r = bignum_to_integer(args[3]->data.number);
  int g = bignum_to_integer(args[4]->data.number);
  int b = bignum_to_integer(args[5]->data.number);
  int a = bignum_to_integer(args[6]->data.number);

  gfx_draw_circle(bignum_to_double(cx), bignum_to_double(cy), bignum_to_double(rad),
                  GET_COLOR(r, g, b, a));
  return NULL;
}
