#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// token
typedef enum {
  TK_RESERVED, // symbol
  TK_NUM,      // int token
  TK_EOF,      // end of input token
} TokenKind;

typedef struct Token Token;

struct Token {
  TokenKind kind; // token type
  Token *next;    // next input token
  int val;        // int value when kind is TK_NUM
  char *str;      // token string
};

Token *token; // current token

// report error func
// use same params as printf
void error(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

// consume token
// next token is expected symbol -> process token + return false
// otherwise -> return false
bool consume(char op) {
  if (token->kind != TK_RESERVED || token->str[0] != op)
    return false;
  token = token->next;
  return true;
}

// next token is expected symbol -> process token 
// otherwise -> report error
void expect(char op) {
  if (token->kind != TK_RESERVED || token->str[0] != op)
    error("not '%c'", op);
  token = token->next;
}

// next token is num -> process token + return num
// otherwise -> report error
int expect_number() {
  if (token->kind != TK_NUM)
    error("not a number");
  int val = token->val;
  token = token->next;
  return val;
}

bool at_eof() {
  return token->kind == TK_EOF;
}

// create new token to connect to cur
Token *new_token(TokenKind kind, Token *cur, char *str) {
  Token *tok = calloc(1, sizeof(Token));
  tok->kind = kind;
  tok->str = str;
  cur->next = tok;
  return tok;
}

// tokenize input str
Token *tokenize(char *p) {
  Token head;
  head.next = NULL;
  Token *cur = &head;

  while (*p) {
    // skip space
    if (isspace(*p)) {
      p++;
      continue;
    }

    if (*p == '+' || *p == '-') {
      cur = new_token(TK_RESERVED, cur, p++);
      continue;
    }

    if (isdigit(*p)) {
      cur = new_token(TK_NUM, cur, p);
      cur->val = strtol(p, &p, 10);
      continue;
    }

    error("could not tokenize");
  }

  new_token(TK_EOF, cur, p);
  return head.next;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "arg num is invalid\n");
    return 1;
  }

  // tokenize
  token = tokenize(argv[1]);

  // output former part of assembly
  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");

  // formula must start with num
  printf("  mov rax, %d\n", expect_number());

  // consume `+ <NUM>` or `- <NUM>` using token
  while (!at_eof()) {
    if (consume('+')) {
      printf("  add rax, %d\n", expect_number());
      continue;
    }

    expect('-');
    printf("  sub rax, %d\n", expect_number());
  }

  printf("  ret\n");
  return 0;
}
