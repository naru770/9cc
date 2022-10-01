#include "9cc.h"

// 変数を名前で検索する
LVar *find_lvar(Token *tok) {
  for (LVar *var = locals; var; var = var->next) {
    if (tok->len == var->len && memcmp(tok->str, var->name, var->len) == 0)
      return var;
  }
  return NULL;
}

// エラー個所を報告する
void error_at(char *loc, char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);

  int pos = loc - user_input;
  fprintf(stderr, "%s\n", user_input);
  fprintf(stderr, "%*s", pos, " ");   // pos個の空白を入力
  fprintf(stderr, "^ ");
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

// エラーを報告するための関数
// printfと同じ引数をとる
void error(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

// 次のトークンが期待している記号の時は、トークンを1つ読み進めて
// 真を返す。それ以外の場合は偽を返す。
bool consume(char *op) {
  if (token->kind != TK_RESERVED ||
      strlen(op) != token->len ||
      memcmp(token->str, op, token->len))
    return false;
  token = token->next;
  return true;
}

// トークンの種類が一致していれば1つ読み進めて真を返す。
bool consume_kind(TokenKind kind) {
  if (token->kind == kind) {
    token = token->next;
    return true;
  }
  return false;
}

// 変数ならば読み進める
Token *consume_ident() {
  if (token->kind != TK_IDENT)
    return NULL;
  Token *ret = token;
  token = token->next;
  return ret;
}

// 次のトークンが期待している記号の時には、トークンを1つ読み進める。
// それ以外の場合にはエラーを報告する。
void expect(char *op) {
  if (token->kind != TK_RESERVED ||
      strlen(op) != token->len ||
      memcmp(token->str, op, token->len))
    error("'%c'ではありません", op);
  token = token->next;
}

// 次のトークンが数値の場合、トークンを1つ読み進めてその数値を返す。
// それ以外の場合にはエラーを報告する。
int expect_number() {
  if (token->kind != TK_NUM)
    error("数字ではありません token kind: %d", token->kind);
  int val = token->val;
  token = token->next;
  return val;
}

bool at_eof() {
  return token->kind == TK_EOF;
}

Node *new_node(NodeKind kind, Node *lhs, Node *rhs) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = kind;
  node->lhs = lhs;
  node->rhs = rhs;
  return node;
}

Node *new_node_num(int val) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_NUM;
  node->val = val;
  return node;
}

void program() {
  Vector *cur = code = calloc(1, sizeof(Vector));

  while (!at_eof()) {
    cur->value = stmt();
    cur->next = calloc(1, sizeof(Vector));
    cur = cur->next;
  }
  cur->value = NULL;
  cur->next = NULL;
}

Node *stmt() {
  Node *node;

  if (consume_kind(TK_RETURN)) {
    node = calloc(1, sizeof(Node));
    node->kind = ND_RETURN;
    node->rhs = NULL;
    node->lhs = expr();
  } else {
    node = expr();
  }

  expect(";");
  return node;
}

Node *assign() {
  Node *node = equality();
  if (consume("="))
    node = new_node(ND_ASSIGN, node, assign());
  return node;
}

Node *expr() {
  return assign();
}

Node *equality() {
  Node *node = relational();

  for (;;) {
    if (consume("=="))
      node = new_node(ND_EQ, node, relational());
    else if (consume("!="))
      node = new_node(ND_NEQ, node, relational());
    else
      return node;
  }
}

Node *relational() {
  Node *node = add();

  for (;;) {
    if (consume("<="))
      node = new_node(ND_LTE, node, add());
    else if (consume(">="))
      node = new_node(ND_LTE, add(), node);
    else if (consume("<"))
      node = new_node(ND_LT, node, add());
    else if (consume(">"))
      node = new_node(ND_LT, add(), node);
    else
      return node;
  }
}

Node *add() { 
  Node *node = mul();

  for (;;) {
    if (consume("+"))
      node = new_node(ND_ADD, node, mul());
    else if (consume("-"))
      node = new_node(ND_SUB, node, mul());
    else
      return node;
  }
}

Node *mul() {
  Node *node = unary();

  for (;;) {
    if (consume("*"))
      node = new_node(ND_MUL, node, primary());
    else if (consume("/"))
      node = new_node(ND_DIV, node, primary());
    else
      return node;
  }
}

Node *unary() {
  if (consume("+"))
    return primary();
  if (consume("-"))
    return new_node(ND_SUB, new_node_num(0), primary());
  return primary();
}

Node *primary() {
  // 次のトークンが"("なら、"(" expr ")"のはず
  if (consume("(")) {
    Node *node = expr();
    expect(")");
    return node;
  }

  Token *tok = consume_ident();
  if (tok) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_LVAR;
    LVar *lvar = find_lvar(tok);

    // 変数リストになければ追加
    if (lvar) {
      node->offset = lvar->offset;
    } else {
      lvar = calloc(1, sizeof(LVar));
      lvar->next = locals;
      lvar->len = tok->len;
      lvar->name = tok->str;
      lvar->offset = (locals) ? locals->offset + 8 : 8;
      node->offset = lvar->offset;
      locals = lvar;
    }
    return node;
  }

  // そうでなければ数値のはず
  return new_node_num(expect_number());
}