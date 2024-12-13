#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

extern FILE* json_file;

void free_tree(Node* node) {
  if (node == NULL) {
    return;
  }
  free_tree(node->left);
  free_tree(node->right);
  free(node);
}
#ifdef __cplusplus
}
#endif
