#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "scanner.h"

extern FILE* file;

static TokenType checkReservedWord(const char* lexeme) {
  if (strcmp(lexeme, "if") == 0)
    return IF;
  if (strcmp(lexeme, "then") == 0)
    return THEN;
  if (strcmp(lexeme, "else") == 0)
    return ELSE;
  if (strcmp(lexeme, "end") == 0)
    return END;
  if (strcmp(lexeme, "repeat") == 0)
    return REPEAT;
  if (strcmp(lexeme, "until") == 0)
    return UNTIL;
  if (strcmp(lexeme, "read") == 0)
    return READ;
  if (strcmp(lexeme, "write") == 0)
    return WRITE;
  return ID;
}

Token getNextToken() {
  char current_char;
  State current_state = START;

  while ((current_char = fgetc(file)) != EOF) {
    // Skip whitespace
    if (isspace(current_char)) {
      continue;
    }

    char buffer[MAX_LEXEME_SIZE] = {current_char};
    int index = 1;

    switch (current_char) {
      case '+':
        return (Token){ADD, strdup(buffer)};
      case '-':
        return (Token){SUB, strdup(buffer)};
      case '*':
        return (Token){MUL, strdup(buffer)};
      case '/':
        return (Token){DIV, strdup(buffer)};
      case '=':
        return (Token){EQ, strdup(buffer)};
      case '<':
        return (Token){LT, strdup(buffer)};
      case '(':
        return (Token){LPAREN, strdup(buffer)};
      case ')':
        return (Token){RPAREN, strdup(buffer)};
      case ';':
        return (Token){SEMI, strdup(buffer)};
      case ':': {
        current_state = INASSIGN;
        break;
      }
      default:
        if (isdigit(current_char)) {
          current_state = INNUM;
        } else if (isalpha(current_char)) {
          current_state = INID;
        } else if (current_char == '{') {
          current_state = INCOMMENT;
          while ((current_char = fgetc(file)) != EOF) {
            if (current_char == '}') {
              break;
            }
          }
          continue;
        } else {
          fprintf(stderr, "Invalid character: %c\n", current_char);
          return (Token){ERROR, "Invalid character"};
        }
    }

    switch (current_state) {
      case INNUM: {
        while (isdigit(current_char = fgetc(file))) {
          buffer[index++] = current_char;
        }
        ungetc(current_char, file);
        return (Token){NUM, strdup(buffer)};
      }

      case INID: {
        while (isalpha(current_char = fgetc(file))) {
          buffer[index++] = current_char;
        }
        ungetc(current_char, file);
        TokenType type = checkReservedWord(buffer);
        return (Token){type, strdup(buffer)};
      }

      case INASSIGN: {
        buffer[index++] = fgetc(file);
        if (strcmp(buffer, ":=") == 0) {
          return (Token){ASSIGN, strdup(buffer)};
        } else {
          return (Token){ERROR, "Invalid character"};
        }
      }

      default:
        return (Token){ERROR, "Invalid character"};
    }
  }

  return (Token){END_OF_FILE, "EOF"};
}

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
    case ASSIGN:
      return "ASSIGN";
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
