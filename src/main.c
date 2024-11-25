#include <stdio.h>
#include <stdlib.h>

#include "scanner.h"

const char* tokenTypeToString(TokenType type) {
  switch (type) {
    case ADD:
      return "ADD";
    case SUB:
      return "SUB";
    case MUL:
      return "MUL";
    case DIV:
      return "DIV";
    case EQ:
      return "EQ";
    case LT:
      return "LT";
    case LPAREN:
      return "LPAREN";
    case RPAREN:
      return "RPAREN";
    case SEMI:
      return "SEMI";
    case ID:
      return "ID";
    case NUM:
      return "NUM";
    case ERROR:
      return "ERROR";
    case IF:
      return "IF";
    case THEN:
      return "THEN";
    case ELSE:
      return "ELSE";
    case END:
      return "END";
    case REPEAT:
      return "REPEAT";
    case UNTIL:
      return "UNTIL";
    case READ:
      return "READ";
    case WRITE:
      return "WRITE";
    case END_OF_FILE:
      return "END_OF_FILE";
    default:
      return "UNKNOWN";
  }
}

int main(int argc, char* argv[]) {
  char* filename = "../tests/test.tiny";

  if (argc > 1) {
    filename = argv[1];
  }

  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    perror("Error opening file");
    return EXIT_FAILURE;
  }

  Token token = getNextToken(file);
  while (token.type != END_OF_FILE) {
    printf("Token: %s, Lexeme: %s\n", tokenTypeToString(token.type),
           token.lexeme);
    token = getNextToken(file);
  }

  fclose(file);

  return EXIT_SUCCESS;
}
