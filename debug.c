#include "9cc.h"

void debug_token() {
  fprintf(stderr, "debug token list:\n");
  Token *cur = token;
  while (cur->kind != TK_EOF) {
    switch (cur->kind) {
    case TK_RESERVED:
    case TK_IDENT:
      if (cur->kind == TK_RESERVED)
        fprintf(stderr, "RESERVED: ");
      else
        fprintf(stderr, "TK_IDENT: ");
      
      for (int i = 0; i < (cur->len); i++)
        fprintf(stderr, "%c", *(cur->str + i));
      fprintf(stderr, "\n");
      break;
    case TK_NUM:
      fprintf(stderr, "TK_NUM:   %d\n", cur->val);
      break;
    case TK_EOF:
      fprintf(stderr, "TK_EOF\n");
      break;
    default:
      fprintf(stderr, "ERR\n");
    }
    cur = cur->next;
  }
  fprintf(stderr, "\n\n");
}


void debug_tree() {
  fprintf(stderr, "reproduce program from syntax tree:\n");
  for (int i = 0; code[i]; i++) {
    print_node(code[i]);
    fprintf(stderr, "\n");
  }
  fprintf(stderr, "\n");
}


void print_node(Node *node) {

  fprintf(stderr, "(");

  if (node->lhs != NULL)
    print_node(node->lhs);
  
  switch (node->kind) {
  case ND_ADD:
    fprintf(stderr, "+");
    break;
  case ND_SUB:
    fprintf(stderr, "-");
    break;
  case ND_MUL:
    fprintf(stderr, "*");
    break;
  case ND_DIV:
    fprintf(stderr, "/");
    break;
  case ND_EQ:
    fprintf(stderr, "==");
    break;
  case ND_NEQ:
    fprintf(stderr, "!=");
    break;
  case ND_LT:
    fprintf(stderr, "<");
    break;
  case ND_LTE:
    fprintf(stderr, "<=");
    break;
  case ND_NUM:
    fprintf(stderr, "%d", node->val);
    break;
  case ND_ASSIGN:
    fprintf(stderr, "=");
    break;
  case ND_LVAR:
   fprintf(stderr, "lval");
    break;
  default:
    fprintf(stderr, "err");
    break;
  }  

  if (node->rhs != NULL)
    print_node(node->rhs);
  
  fprintf(stderr, ")");
}