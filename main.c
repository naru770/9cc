#include "9cc.h"

Token *token;
char *user_input;
Vector *code;
LVar *locals;

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "引数の個数が正しくありません\n");
    return 1;
  }

  locals = NULL;

  // トークナイズする
  user_input = argv[1];
  tokenize();

  debug_token();
  
  // 構文木生成
  program();

  debug_tree();

  // アセンブリの前半部分を出力
  printf(".intel_syntax noprefix\n");
  printf(".globl main\n");
  printf("main:\n");

  // プロローグ
  // 変数26個分の領域を確保する
  printf("  push rbp\n");
  printf("  mov rbp, rsp\n");
  printf("  sub rsp, 208\n");

  for (Vector *cur = code; cur->value != NULL; cur = cur->next) {
    gen(cur->value);

    printf("  pop rax\n");
  }

  // エピローグ
  printf("  mov rsp, rbp\n");
  printf("  pop rbp\n");
  printf("  ret\n");
  return 0;
}