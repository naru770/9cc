#include "9cc.h"

void gen_lval(Node *node) {
  if (node->kind != ND_LVAR)
    error("代入の左辺値が変数ではありません");
  
  printf("  mov rax, rbp\n");
  printf("  sub rax, %d\n", node->offset);
  printf("  push rax\n");
}

void gen(Node *node) {
  if (node == NULL)
    return;
  
  switch (node->kind) {
  case ND_FOR:
    {
      int begin_num = label_num++;
      int end_num = label_num++;

      gen(node->init);
      printf(".Lbegin%d:\n", begin_num);
      gen(node->cond);
      printf("  pop rax\n");
      printf("  cmp rax, 0\n");
      printf("  je .Lend%d\n", end_num);
      gen(node->lhs);
      gen(node->upd);
      printf("  jmp .Lbegin%d\n", begin_num);
      printf(".Lend%d:\n",end_num);

      return;
    }

  case ND_WHILE:
    {
      int begin_num = label_num++;
      int end_num = label_num++;
      printf(".Lbegin%d:\n", begin_num);
      gen(node->cond);
      printf("  pop rax\n");
      printf("  cmp rax, 0\n");
      printf("  je .Lend%d\n", end_num);
      gen(node->lhs);
      printf("  jmp .Lbegin%d\n", begin_num);
      printf(".Lend%d:\n", end_num);
      return;
    }

  case ND_IF:
    gen(node->cond);
    printf("  pop rax\n");
    printf("  cmp rax, 0\n");

    if (node->rhs != NULL) {
      int end_num = label_num++;
      int else_num = label_num++;
      printf("  je .Lelse%d\n", end_num);
      gen(node->lhs);
      printf("  jmp .Lend%d\n", else_num);
      printf(".Lelse%d:\n", end_num);
      gen(node->rhs);
      printf(".Lend%d:\n", else_num);

    } else {
      int end_num = label_num++;
      printf("  je .Lend%d\n", end_num);
      gen(node->lhs);
      printf(".Lend%d:\n", end_num);
    }
    return;

  case ND_BLOCK:
    for (Vector *cur = node->vector; cur->value != NULL; cur = cur->next) {
      gen(cur->value);

      printf("  pop rax\n");
    }
    return;

  case ND_RETURN:
    gen(node->lhs);
    printf("  pop rax\n");
    printf("  mov rsp, rbp\n");
    printf("  pop rbp\n");
    printf("  ret\n");
    return;

  case ND_NUM:
    printf("  push %d\n", node->val);
    return;

  case ND_LVAR:
    gen_lval(node);
    if (node->inckind == NO_INC) {
      printf("  pop rax\n");
      printf("  mov rax, [rax]\n");
      printf("  push rax\n");
    } else {
      printf("  pop rax\n");
      printf("  mov rbx, rax\n");  // アドレス保管
      printf("  mov rax, [rax]\n");

      if (node->inckind == PRE_INC)
        printf("  add rax, 1\n");
      else if (node->inckind == PRE_DEC)
        printf("  sub rax, 1\n");

      printf("  push rax\n");

      if (node->inckind == POST_INC)
        printf("  add rax, 1\n");
      else if (node->inckind == POST_DEC)
        printf("  sub rax, 1\n");
      
      printf("  mov [rbx], rax\n");
    }

    return;

  case ND_ASSIGN:
    gen_lval(node->lhs);
    gen(node->rhs);

    printf("  pop rdi\n");
    printf("  pop rax\n");
    printf("  mov [rax], rdi\n");
    printf("  push rdi\n");
    return;
  }

  gen(node->lhs);
  gen(node->rhs);

  printf("  pop rdi\n");
  printf("  pop rax\n");

  switch (node->kind) {
  case ND_FUNC:

    // 引数があれば
    if (node->vector != NULL) {
      for (Vector *cur = node->vector; cur != NULL; cur = cur->next) {
        gen(cur->value);
      }
      for (int i = 0; i < node->argc; i++) {
        if (i == 0)
          printf("  pop rdi\n");
        else if (i == 1)
          printf("  pop rsi\n");
        else if (i == 2)
          printf("  pop rdx\n");
        else if (i == 3)
          printf("  pop r10\n");
        else if (i == 3)
          printf("  pop r8\n");
        else
          printf("  pop r9\n");
      }
    }

    // 関数呼び出し
    printf("  call ");
    for (int i = 0; i < node->len; i++)
      printf("%c", *(node->name + i));
    printf("\n");

    break;
  case ND_ADD:
    printf("  add rax, rdi\n");
    break;
  case ND_SUB:
    printf("  sub rax, rdi\n");
    break;
  case ND_MUL:
    printf("  imul rax, rdi\n");
    break;
  case ND_DIV:
    printf("  cqo\n");
    printf("  idiv rdi\n");
    break;
  case ND_EQ:
    printf("  cmp rax, rdi\n");
    printf("  sete al\n");
    printf("  movzb rax, al\n");
    break;
  case ND_NEQ:
    printf("  cmp rax, rdi\n");
    printf("  setne al\n");
    printf("  movzb rax, al\n");
    break;
  case ND_LT:
    printf("  cmp rax, rdi\n");
    printf("  setl al\n");
    printf("  movzb rax, al\n");
    break;
  case ND_LTE:
    printf("  cmp rax, rdi\n");
    printf("  setle al\n");
    printf("  movzb rax, al\n");
    break;
  }

  printf("  push rax\n");
}
