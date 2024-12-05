#include <cjson/cJSON.h>
#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

FILE* file;

static void free_tree(Node* node) {
  if (node == NULL) {
    return;
  }
  free_tree(node->left);
  free_tree(node->right);
  free(node);
}

static cJSON* node_to_json(Node* node) {
  if (node == NULL) {
    return cJSON_CreateNull();
  }
  cJSON* json_node = cJSON_CreateObject();
  cJSON_AddStringToObject(json_node, "type", nodeTypeToString(node->type));
  cJSON_AddStringToObject(json_node, "value", node->value);
  cJSON_AddItemToObject(json_node, "left", node_to_json(node->left));
  cJSON_AddItemToObject(json_node, "right", node_to_json(node->right));
  cJSON_AddItemToObject(json_node, "next", node_to_json(node->next));
  return json_node;
}

int main(int argc, char* argv[]) {
  char* filename = "../tests/test.tiny";

  if (argc > 1) {
    filename = argv[1];
  }

  file = fopen(filename, "r");
  if (file == NULL) {
    perror("Error opening file");
    return EXIT_FAILURE;
  }

  Node* root = program();

  printf("root.type = %d\n", root->type);

  cJSON* json_root = node_to_json(root);
  char* json_string = cJSON_Print(json_root);
  FILE* json_file = fopen("parse_tree.json", "w");
  if (json_file != NULL) {
    fputs(json_string, json_file);
    fclose(json_file);
  }
  cJSON_Delete(json_root);
  free(json_string);

  fclose(file);
  free_tree(root);

  return EXIT_SUCCESS;
}
