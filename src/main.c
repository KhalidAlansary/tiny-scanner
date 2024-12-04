#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

FILE* file;

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

  return EXIT_SUCCESS;
}
