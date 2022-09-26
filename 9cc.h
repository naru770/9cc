#include <stdbool.h>

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
} NodeKind;

typedef struct Node Node;

// 抽象構文木のノードの型
struct Node {
  NodeKind kind;  // ノードの型
  Node *lhs;      // 左辺
  Node *rhs;      // 右辺
  int val;        // kindがND_NUMの場合のみ使う
  int offset;     // kindがND_LVARの場合のみ使う
};

typedef enum {
  TK_RESERVED,
  TK_IDENT,
  TK_NUM,
  TK_EOF,
} TokenKind;

typedef struct Token Token;

struct Token {
  TokenKind kind; // トークンの型
  Token *next;    // 次の入力トークン
  int val;        // kindがTK_NUMの場合、その数値
  char *str;      // トークン文字列
  int len;        // トークンの長さ
};


void error_at(char *, char *, ...);
void error(char *, ...);
bool consume(char *);
void expect(char *);
int expect_number();
bool at_eof();
Token *new_token(TokenKind, Token *, char *, int);
void tokenize();
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
void gen(Node *);

void debug_token();
void print_node();
void debug_tree();

extern Token *token;
extern char *user_input;
extern Node *code[100];