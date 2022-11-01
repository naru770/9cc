#include "9cc.h"

Token *token;
char *user_input;
Node *code;
LVar *locals;
int label_num;

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
  
  return 0;
}