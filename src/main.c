#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "util.h"

FILE* file;
FILE* json_file;

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

  fclose(file);

  json_file = fopen("parse_tree.json", "w");
  if (json_file == NULL) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }

  print_tree_to_json(root);

  fclose(json_file);

  free_tree(root);

  return EXIT_SUCCESS;
}
