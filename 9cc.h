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
typedef struct GVar GVar;

// 抽象構文木のノードの種類
typedef enum {
  ND_ADD,     // +
  ND_SUB,     // -
  ND_MUL,     // *
  ND_DIV,     // /
  ND_EQ,      // ==
  ND_NEQ,     // !=
  ND_LT,      // <
  ND_LTE,     // <=
  ND_NUM,     // 整数
  ND_ASSIGN,  // =
  ND_LVAR,    // ローカル変数
  ND_RETURN,  // return
  ND_BLOCK,   // { * }
  ND_IF,
  ND_FOR,
  ND_WHILE,
  ND_FUNC,
  ND_ADDR,    // &
  ND_DEREF,   // *
  ND_DEC      // 宣言
} NodeKind;

typedef enum {
  NO_INC,
  PRE_INC,
  PRE_DEC,
  POST_INC,
  POST_DEC
} IncKind;

// 抽象構文木のノードの型
struct Node {
  NodeKind kind;  // ノードの型
  Node *lhs;      // 左辺
  Node *rhs;      // 右辺
  Node *cond;     // if, while, forで使う
  Node *init;     // forで使う
  Node *upd;      // forで使う
  int val;        // kindがND_NUMの場合のみ使う
  int offset;     // kindがND_LVARの場合のみ使う
  Vector *vector; // kindがND_BLOCKかND_FUNCの場合のみ使う
  IncKind inckind;// インクリメントの種類
  char *name;     // 関数名
  int len;        // 関数名の長さ
  int argc;       // 関数の引数の個数
  bool is_declaration;  // ND_ASSIGNで使う
};


typedef enum {
  TK_RESERVED,
  TK_RETURN,
  TK_IF,
  TK_ELSE,
  TK_FOR,
  TK_WHILE,
  TK_IDENT,
  TK_NUM,
  TK_EOF,
  TK_TYPE
} TokenKind;


struct Token {
  TokenKind kind; // トークンの型
  Token *next;    // 次の入力トークン
  int val;        // kindがTK_NUMの場合、その数値
  char *str;      // トークン文字列
  int len;        // トークンの長さ
};


struct Vector {
  Vector *next;
  Node *value;
};


struct GVar {
  GVar *next;
  LVar *locals;     // 関数が持つローカル変数
  char *name;     // 関数名
  int len;        // 関数名の長さ
};

// ローカル変数の型
struct LVar {
  LVar *next;
  char *name;
  int len;
  int offset;
  bool is_func;   // 関数か変数か
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
Node *inc();
Node *primary();

// codegen.c
void gen_lval(Node *node);
void gen_global(Node *node);
void gen(Node *);

// io.c
char *read_file(char *path);

extern Token *token;
extern char *user_input;
extern Vector *code;
extern LVar *locals;
extern GVar *globals;
extern int label_num;