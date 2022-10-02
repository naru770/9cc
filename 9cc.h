#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>

typedef struct Vector Vector;
typedef struct Node Node;
typedef struct Token Token;
typedef struct LVar LVar;

// 抽象構文木のノードの種類
typedef enum {
  ND_ADD, // +
  ND_SUB, // -
  ND_MUL, // *
  ND_DIV, // /
  ND_EQ,  // ==
  ND_NEQ, // !=
  ND_LT,  // <
  ND_LTE, // <=
  ND_NUM, // 整数
  ND_ASSIGN,  // =
  ND_LVAR,  // ローカル変数
  ND_RETURN, // return
  ND_BLOCK
} NodeKind;


// 抽象構文木のノードの型
struct Node {
  NodeKind kind;  // ノードの型
  Node *lhs;      // 左辺
  Node *rhs;      // 右辺
  int val;        // kindがND_NUMの場合のみ使う
  int offset;     // kindがND_LVARの場合のみ使う
  Vector *vector; // kindがND_BLOCKの場合のみ使う
};


typedef enum {
  TK_RESERVED,
  TK_RETURN,
  TK_IDENT,
  TK_NUM,
  TK_EOF,
} TokenKind;


struct Vector {
  Vector *next;
  Node *value;
};


struct Token {
  TokenKind kind; // トークンの型
  Token *next;    // 次の入力トークン
  int val;        // kindがTK_NUMの場合、その数値
  char *str;      // トークン文字列
  int len;        // トークンの長さ
};


// ローカル変数の型
struct LVar {
  LVar *next;
  char *name;
  int len;
  int offset;
};

// tokenize.c
bool at_eof();
Token *new_token(TokenKind, Token *, char *, int);
void tokenize();
int is_alnum(char);

// parse.c
void error_at(char *, char *, ...);
void error(char *, ...);
bool consume(char *);
void expect(char *);
int expect_number();
Node *new_node(NodeKind, Node *, Node *);
Node *new_node_num(int);
void program();
Node *stmt();
Node *assign();
Node *expr();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *primary();

// codegen.c
void gen_lval(Node *node);
void gen(Node *);

// debug.c
void debug_token();
void print_node();
void debug_tree();

extern Token *token;
extern char *user_input;
extern Vector *code;

extern LVar *locals;
