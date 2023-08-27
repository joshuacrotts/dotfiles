#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "mgc.h"
#include "sval.h"

static struct sval *sval_create(const enum sval_type type);
static void sval_fprint(struct sval *sv, FILE *stream);
static bool is_proper_list(const struct sval *ls);

struct sval *
sval_number_create(char *number) {
  struct sval *sv = sval_create(SVAL_NUMBER);
  sv->data.number = bignum_create_str(number);
  return sv;
}

struct sval *
sval_number_create_bn(bignum *number) {
  struct sval *sv = sval_create(SVAL_NUMBER);
  sv->data.number = bignum_create_num(number);
  return sv;
}

struct sval *(*sval_get_builtin(struct sval *sv))(struct sval **, const size_t,
                                                  struct environment *) {
  return sv->data.builtin;
}

struct sval *
sval_symbol_create(char *symbol) {
  struct sval *sv = sval_create(SVAL_SYMBOL);
  sv->data.symbol = mgc_alloc(strlen(symbol) + 1, free);
  strcpy(sv->data.symbol, symbol);
  return sv;
}

struct sval *
sval_boolean_create(bool boolean) {
  struct sval *sv = sval_create(SVAL_BOOLEAN);
  sv->data.boolean = boolean;
  return sv;
}

struct sval *
sval_character_create(char character) {
  struct sval *sv = sval_create(SVAL_CHARACTER);
  sv->data.character = character;
  return sv;
}

struct sval *
sval_string_create(char *string) {
  struct sval *sv = sval_create(SVAL_STRING);
  sv->data.string = string;
  return sv;
}

struct sval *
sval_pair_create(struct sval *car, struct sval *cdr) {
  struct sval *sv = sval_create(SVAL_PAIR);
  sv->data.pair.car = car;
  sv->data.pair.cdr = cdr;
  return sv;
}

struct sval *
sval_list_create(struct sval **args, size_t num_args) {
  struct sval *curr = sval_pair_create(NULL, NULL);
  struct sval *front = curr;
  for (int i = 0; i < num_args; i++) {
    curr->data.pair.car = args[i];
    curr->data.pair.cdr = sval_pair_create(NULL, NULL);
    curr = curr->data.pair.cdr;
  }
  curr->data.pair.cdr = NULL;
  return front;
}

struct sval *
sval_vector_create(size_t vector_size) {
  struct sval *vector = sval_create(SVAL_VECTOR);
  vector->data.vector.elements = mgc_alloc(vector_size * sizeof(struct sval *), free);
  vector->data.vector.size = vector_size;
  return vector;
}

struct sval *
sval_builtin_create(struct sval *(*builtin)(struct sval **args, size_t num_args,
                                            struct environment *env)) {
  struct sval *sv = sval_create(SVAL_BUILTIN);
  sv->data.builtin = builtin;
  return sv;
}

struct sval *
sval_macro_create(char **formals, size_t num_formals, ast *body) {
  struct sval *sv = sval_create(SVAL_MACRO);
  struct macro *macro = mgc_alloc(sizeof(struct macro), sval_destroy);
  macro->num_formals = num_formals;
  macro->formals = formals;
  macro->body = body;
  sv->data.macro = macro;
  return sv;
}

struct sval *
sval_file_create(char *filename, char *mode) {
  struct sval *sv = sval_create(SVAL_FILE);
  sv->data.file.fp = fopen(filename, mode);
  if (NULL == sv->data.file.fp) {
    fprintf(stderr, "sval_file_create: could not open file %s with mode %s\n", filename, mode);
    exit(EXIT_FAILURE);
  }
  sv->data.file.position = 0;
  sv->data.file.is_open = true;
  return sv;
}

struct sval *
sval_procedure_create(char **formals, size_t num_formals, ast *body, struct environment *env,
                      bool is_variadic) {
  struct sval *sv = sval_create(SVAL_PROCEDURE);
  struct procedure *proc = mgc_alloc(sizeof(struct procedure), sval_destroy);
  proc->num_formals = num_formals;
  proc->formals = formals;
  proc->body = body;
  proc->env = env;
  proc->is_variadic = is_variadic;
  sv->data.proc = proc;
  return sv;
}

struct sval *
sval_quote_create(struct sval *rest) {
  struct sval **args = mgc_ualloc(sizeof(struct sval *) * 2);
  struct sval *s = sval_symbol_create("quote");
  args[0] = s;
  args[1] = rest;
  struct sval *sym = sval_list_create(args, 2);
  free(args);
  return sym;
}

char *
sval_tostring(struct sval *sv) {
  char *buffer = NULL;
  size_t buffer_size = 0;
  FILE *stream = open_memstream(&buffer, &buffer_size);

  sval_fprint(sv, stream);
  fclose(stream);
  return buffer;
}

void
sval_print(struct sval *sv) {
  sval_fprint(sv, stdout);
}

bool
sval_list_contains(struct sval *ls, struct sval *e) {
  struct sval *curr = ls;
  if (curr->data.pair.car != NULL) {
    while (curr->data.pair.cdr->data.pair.cdr != NULL) {
      if (sval_equals(curr->data.pair.car, e)) {
        return true;
      }
      curr = curr->data.pair.cdr;
    }
  }

  return false;
}

bool
sval_equals(struct sval *sv1, struct sval *sv2) {
  if ((NULL == sv1) ^ (NULL == sv2)) {
    return false;
  } else if (sv1->type != sv2->type) {
    return false;
  } else {
    switch (sv1->type) {
    case SVAL_NUMBER:
      return bignum_equal(sv1->data.number, sv2->data.number);
    case SVAL_SYMBOL:
      return strcmp(sv1->data.symbol, sv2->data.symbol) == 0;
    case SVAL_BOOLEAN:
      return sv1->data.boolean == sv2->data.boolean;
    case SVAL_CHARACTER:
      return sv1->data.character == sv2->data.character;
    case SVAL_STRING:
      return strcmp(sv1->data.string, sv2->data.string) == 0;
    case SVAL_PAIR:
      if (NULL == sv1->data.pair.car && NULL == sv2->data.pair.car) {
        return true;
      } else if (sval_equals(sv1->data.pair.car, sv2->data.pair.car)) {
        return sval_equals(sv1->data.pair.cdr, sv2->data.pair.cdr);
      } else {
        return false;
      }
    default:
      return false;
    }
  }
}

const char *
sval_type(const enum sval_type type) {
  switch (type) {
  case SVAL_NUMBER:
    return "number";
  case SVAL_BOOLEAN:
    return "boolean";
  case SVAL_SYMBOL:
    return "symbol";
  case SVAL_PAIR:
    return "pair";
  case SVAL_CHARACTER:
    return "character";
  case SVAL_VECTOR:
    return "vector";
  case SVAL_MACRO:
    return "macro";
  case SVAL_FILE:
    return "file";
  case SVAL_PROCEDURE:
  case SVAL_BUILTIN:
    return "procedure";
  default:
    fprintf(stderr, "Invalid enum type %d\n", type);
    exit(EXIT_FAILURE);
  }
}

void
sval_destroy(void *ptr) {
  struct sval *sv = ( struct sval * ) ptr;
  if (SVAL_NUMBER == sv->type) {
    mgc_free(sv->data.number);
  } else if (SVAL_FILE == sv->type) {
    if (sv->data.file.is_open) {
      fclose(sv->data.file.fp);
    }
  }
  mgc_ufree(sv);
}

static struct sval *
sval_create(const enum sval_type type) {
  struct sval *sv = mgc_alloc(sizeof(struct sval), sval_destroy);
  sv->type = type;
  return sv;
}

static void
sval_fprint(struct sval *sv, FILE *stream) {
  if (NULL == sv) {
    fprintf(stream, "nil");
  } else if (SVAL_NUMBER == sv->type) {
    if (stdout == stream) {
      bignum_print(sv->data.number);
    } else {
      fprintf(stream, "%s", bignum_to_string(sv->data.number));
    }
  } else if (SVAL_SYMBOL == sv->type) {
    fprintf(stream, "%s", sv->data.symbol);
  } else if (SVAL_BOOLEAN == sv->type) {
    fprintf(stream, "#%c", sv->data.boolean ? 't' : 'f');
  } else if (SVAL_CHARACTER == sv->type) {
    fprintf(stream, "\\#%c", sv->data.character);
  } else if (SVAL_STRING == sv->type) {
    fprintf(stream, "\"%s\"", sv->data.string);
  } else if (SVAL_FILE == sv->type) {
    fprintf(stream, "%s", "<file>");
  } else if (SVAL_PROCEDURE == sv->type || SVAL_BUILTIN == sv->type) {
    fprintf(stream, "%s", "<function>");
  } else if (SVAL_VECTOR == sv->type) {
    fprintf(stream, "#(");
    int i = 0;
    for (; i < sv->data.vector.size - 1; i++) {
      sval_fprint(sv->data.vector.elements[i], stream);
      fprintf(stream, " ");
    }
    if (i < sv->data.vector.size) {
      sval_fprint(sv->data.vector.elements[i], stream);
    }
    fprintf(stream, ")");
  } else if (SVAL_PAIR == sv->type) {
    fprintf(stream, "(");
    if (is_proper_list(sv)) {
      struct sval *curr = sv;
      if (curr->data.pair.car != NULL) {
        while (curr->data.pair.cdr->data.pair.cdr != NULL) {
          sval_fprint(curr->data.pair.car, stream);
          curr = curr->data.pair.cdr;
          fprintf(stream, " ");
        }
        sval_fprint(curr->data.pair.car, stream);
      }
    } else if (sv != NULL) {
      sval_fprint(sv->data.pair.car, stream);
      fprintf(stream, " . ");
      sval_fprint(sv->data.pair.cdr, stream);
    }
    fprintf(stream, ")");
  } else {
    fprintf(stderr, "Invalid sval: %d\n", sv->type);
    exit(EXIT_FAILURE);
  }
}

static bool
is_proper_list(const struct sval *ls) {
  if (NULL == ls || ls->data.pair.car == NULL) {
    return true;
  } else if (ls->data.pair.car != NULL && ls->data.pair.cdr != NULL &&
             ls->data.pair.cdr->type != SVAL_PAIR) {
    return false;
  } else {
    return is_proper_list(ls->data.pair.cdr);
  }
}
