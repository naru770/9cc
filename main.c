#include "9cc.h"

Token *token;
char *user_input;
Node *code;
LVar *locals;
GVar *globals;
int label_num;

void free_node(Node *node);
void free_locals(LVar *lvar);
void free_globals(GVar *gvar);
void free_all();

void free_node(Node *node) {
  if (node == NULL)
    return;
  
  free_node(node->lhs);
  free_node(node->rhs);
  free_node(node->cond);
  free_node(node->init);
  free_node(node->upd);
  free_node(node->next);
  free_node(node->argv);
  free(node);
}

void free_locals(LVar *lvar) {
  if (lvar == NULL)
    return;
  
  free_locals(lvar->next);
  free(lvar);
}

void free_globals(GVar *gvar) {
  if (gvar == NULL)
    return;
  
  free_globals(gvar->next);
  free_locals(gvar->locals);
  free(gvar);
}


int main(int argc, char **argv) {

  locals = NULL;
  label_num = 0;
  
  user_input = read_file(argv[1]);

  // トークナイズする
  tokenize();

  fprintf(stderr, "1: tokenize pass\n");
  
  // 構文木生成
  program();

  fprintf(stderr, "2: parse pass\n");

  // アセンブリの前半部分を出力
  printf(".intel_syntax noprefix\n");
  printf(".globl main\n");

  gen_global();

  fprintf(stderr, "3: codegen pass\n");

  // free_all();
  
  return 0;
}