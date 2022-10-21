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

// 次のトークンが期待した記号ならば真を返す
bool foresee(char *op) {
  if (token->kind != TK_RESERVED ||
      strlen(op) != token->len ||
      memcmp(token->str, op, token->len))
    return false;
  return true;
}

// 次のトークンが期待している記号の時は
// トークンを1つ読み進めて真を返す
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
      memcmp(token->str, op, token->len)) {
    int eol_cnt = 0;
    for (char *p = user_input; p <= token->str; p++) {
      if (*p == '\n')
        eol_cnt++;
    }

    error("line %d: '%c'ではありません", eol_cnt, *op);
  }
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
  Node *head, *cur;
  head = cur = calloc(1, sizeof(Node));

  while (!at_eof()) {

    // 関数ノード
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_FUNC;
    node->is_declaration = true;

    // 宣言子が無ければエラー
    if (!consume_kind(TK_TYPE))
      error("Declaration was expected\n");

    Token *tok = consume_ident();

    if (tok == NULL)
      error("Every statement should be in a function\n");

    node->name = tok->str;
    node->len = tok->len;

    expect("(");

    locals = calloc(1, sizeof(LVar));
    
    if (consume(")")) {
      node->argv = NULL;
    } else {
      for (;;) {
        if (!consume_kind(TK_TYPE))
          error("Declaration was expected");

        Token *tk = consume_ident();

        LVar *lvar = calloc(1, sizeof(LVar));
        lvar->name = tk->str;
        lvar->len = tk->len;
        lvar->offset = locals->offset + 8;
        lvar->next = locals;
        locals = lvar;
        
        node->argc++;

        if (consume(",")) {
          continue;
        } else if(consume(")")) {
          break;
        } else {
          error("Something went wrong with argument");
        }
      }
    }
    
    if (foresee("{"))
      node->lhs = stmt();
    else
      error("Expected '{'");
    
    cur->next = node;
    cur = cur->next;
  }
  
  cur->next = NULL;
  
  code = head->next;
}

Node *stmt() {
  Node *node;

  // 変数宣言
  if (consume_kind(TK_TYPE)) {
    node = calloc(1, sizeof(Node));
    node->kind = ND_ASSIGN;
    node->is_declaration = true;

    Token *tok = consume_ident();

    if (!tok)
      error("Ident was expected");

    if (find_lvar(tok))
      error("This variable is already declared");
    
    // 変数リストに追加
    LVar *lvar = calloc(1, sizeof(LVar));
    lvar->next = locals;
    lvar->len = tok->len;
    lvar->name = tok->str;
    lvar->offset = locals->offset + 8;
    locals = lvar;
    node->offset = lvar->offset;
    node->name = lvar->name;
    node->len = lvar->len;

    // fprintf(stderr, "===== debug =====\n");
    // fprintf(stderr, "%s\n", token->str);
    // fprintf(stderr, "=================\n\n");

    expect(";");

    return NULL;


  } else if (consume_kind(TK_FOR)) {
    node = calloc(1, sizeof(Node));
    node->kind = ND_FOR;
    expect("(");

    // if init expression exists
    if (!foresee(";"))
      node->init = expr();
    expect(";");

    // if cond expression exists
    if (!foresee(";"))
      node->cond = expr();
    expect(";");

    // if update expression exsits
    if (!foresee(")"))
      node->upd = expr();
    expect(")");

    node->lhs = stmt();
    node->rhs = NULL;
    
    return node;


  } else if (consume_kind(TK_WHILE)) {
    node = calloc(1, sizeof(Node));
    node->kind = ND_WHILE;
    expect("(");
    node->cond = expr();
    expect(")");
    node->lhs = stmt();
    node->rhs = NULL;

    return node;


  } else if (consume_kind(TK_IF)) {
    node = calloc(1, sizeof(Node));
    node->kind = ND_IF;
    expect("(");
    node->cond = expr();
    expect(")");
    node->lhs = stmt();

    if (consume_kind(TK_ELSE)) {
      node->rhs = stmt();
    } else {
      node->rhs = NULL;
    }

    return node;


  // BLOCK
  } else if (consume("{")) {
    node = calloc(1, sizeof(Node)); 
    node->kind = ND_BLOCK;
    node->rhs = NULL;
    node->lhs = NULL;

    if (consume("}"))
      return node;
    
    // ブロック内のstatementのノード列をベクタとして取得
    Node *head, *cur;
    head = cur = calloc(1, sizeof(Node));

    while (!consume("}")) {
      cur->next = stmt();
      if (cur->next != NULL)
        cur = cur->next;
    }

    node->lhs = head->next;

    return node;

  } else if (consume_kind(TK_RETURN)) {
    node = calloc(1, sizeof(Node));
    node->kind = ND_RETURN;
    node->rhs = NULL;
    node->lhs = expr();

  } else {
    node = expr();
  }

  // fprintf(stderr, "===== debug =====\n");
  // fprintf(stderr, "%s\n", token->str);
  // fprintf(stderr, "=================\n\n");

  expect(";");
  return node;
}

Node *expr() {
  return assign();
}

Node *assign() {
  Node *node = equality();

  if (consume("+=")) {
    Node *add_node = new_node(ND_ADD, node, assign());
    node = new_node(ND_ASSIGN, node, add_node);

  } else if (consume("-=")) {
    Node *add_node = new_node(ND_SUB, node, assign());
    node = new_node(ND_ASSIGN, node, add_node);

  } else if (consume("*=")) {
    Node *add_node = new_node(ND_MUL, node, assign());
    node = new_node(ND_ASSIGN, node, add_node);

  } else if (consume("/=")) {
    Node *add_node = new_node(ND_DIV, node, assign());
    node = new_node(ND_ASSIGN, node, add_node);

  } else if (consume("=")) {
    node = new_node(ND_ASSIGN, node, assign());

  }
  return node;
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
    return inc();
  if (consume("-"))
    return new_node(ND_SUB, new_node_num(0), inc());
  if (consume("&")) 
    return new_node(ND_ADDR, inc(), NULL);
  if (consume("*"))
    return new_node(ND_DEREF, inc(), NULL);
  return inc();
}

Node *inc() {
  Node *node;

  if (consume("++")) {
    node = primary();
    node->inckind = PRE_INC;
  } else if (consume("--")) {
    node = primary();
    node->inckind = PRE_DEC;
  } else {
    node = primary();

    if (consume("++"))
      node->inckind = POST_INC;
    else if (consume("--"))
      node->inckind = POST_DEC;
    else
      node->inckind = NO_INC;
  }

  return node;
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
    // 関数呼び出し
    if (consume("(")) {
      Node *node = calloc(1, sizeof(Node));
      node->kind = ND_FUNC;
      node->name = tok->str;
      node->len = tok->len;

      if (consume(")")) {
        return node;
      } else {
        Node *head, *cur;
        head = cur = calloc(1, sizeof(Node));
        
        for (;;) {
          cur->next = expr();
          cur = cur->next;
          node->argc++;

          if (consume(",")) {
            continue;
          } else if (consume(")")) {
            cur->next = NULL;
            break;
          }
        }

        node->argv = head->next;
        return node;
      }

    // 変数
    } else {
      Node *node = calloc(1, sizeof(Node));
      node->kind = ND_LVAR;
      LVar *lvar = find_lvar(tok);

      // 変数リストになければエラー
      if (!lvar)
        error("This variable was not declared yet");
      
      node->offset = lvar->offset;
      return node;
    }
  }

  // そうでなければ数値のはず
  return new_node_num(expect_number());
}