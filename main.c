#include "9cc.h"

Token *token;
char *user_input;
Vector *code;
LVar *locals;
int label_num;

int main(int argc, char **argv) {

  locals = NULL;
  label_num = 0;
  
  user_input = read_file(argv[1]);

  // トークナイズする
  tokenize();

  // debug_token();
  
  // 構文木生成
  program();

  // debug_tree();

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