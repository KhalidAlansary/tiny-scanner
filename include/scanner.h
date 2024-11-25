#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>

#define MAX_LEXEME_SIZE 100

typedef enum {
  ADD,
  SUB,
  MUL,
  DIV,
  EQ,
  LT,
  LPAREN,
  RPAREN,
  SEMI,
  ASSIGN,
  ID,
  NUM,
  ERROR,
  IF,
  THEN,
  ELSE,
  END,
  REPEAT,
  UNTIL,
  READ,
  WRITE,
  END_OF_FILE,
} TokenType;

typedef struct {
  TokenType type;
  char* lexeme;
} Token;

typedef enum { START, INNUM, INID, INASSIGN, INCOMMENT, DONE } State;

Token getNextToken(FILE* file);

#endif
