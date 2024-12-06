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

void print_tree_to_json(Node* root) {
  if (root == NULL) {
    fprintf(json_file, "null");
    return;
  }

  fprintf(json_file, "{\"type\":\"%s\",\"value\":\"%s\",\"left\":",
          nodeTypeToString(root->type), root->value);

  print_tree_to_json(root->left);

  fprintf(json_file, ",\"right\":");

  print_tree_to_json(root->right);

  fprintf(json_file, ",\"next\":");

  print_tree_to_json(root->next);

  fprintf(json_file, "}");
}
