#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// tokenize.c
//
typedef enum {
  TK_RESERVED, // symbol
  TK_IDENT,    // ident
  TK_NUM,      // int token
  TK_EOF,      // end of input token
  TK_RETURN    // return
} TokenKind;

typedef struct Token Token;
struct Token {
  TokenKind kind; // token type
  Token *next;    // next input token
  int val;        // int value when kind is TK_NUM
  char *str;      // token string
  int len;        // token length
};

void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);
bool consume(char *op);
Token *consume_ident();
void expect(char *op);
int expect_number();
bool at_eof();
Token *new_token(TokenKind kind, Token *cur, char *str, int len);
Token *tokenize();

// Input program
extern char *user_input;
extern Token *token; // current token

typedef struct LVar LVar;
struct LVar {
  LVar *next; // next variable or NULL
  char *name;
  int len;
  int offset; // offset from RBP
};

//
// parse.c
//
typedef enum {
  ND_ADD, // +
  ND_SUB, // -
  ND_MUL, // *
  ND_DIV, // /
  ND_NUM, // num
  ND_EQ,  // ==
  ND_NE,  // !=
  ND_LT,  // <
  ND_LE,  // <=
  ND_ASSIGN, // assignment
  ND_LVAR,   // local variable
  ND_RETURN, // return
} NodeKind;

typedef struct Node Node;
struct Node {
  NodeKind kind; // node type
  Node *lhs;
  Node *rhs;
  int val;       // used only when kind is ND_NUM
  int offset;    // used only when kind is ND_LVAR
};

extern Node *code[100];
void program();

//
// codegen.c
//
void codegen();
