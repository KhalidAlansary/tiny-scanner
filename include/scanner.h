#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>

#define MAX_LEXEME_SIZE 100

/**
 * @brief Enumeration of possible token types.
 */
typedef enum {
  ADD,        /**< Addition operator */
  SUB,        /**< Subtraction operator */
  MUL,        /**< Multiplication operator */
  DIV,        /**< Division operator */
  EQ,         /**< Equality operator */
  LT,         /**< Less than operator */
  LPAREN,     /**< Left parenthesis */
  RPAREN,     /**< Right parenthesis */
  SEMI,       /**< Semicolon */
  ASSIGN,     /**< Assignment operator */
  ID,         /**< Identifier */
  NUM,        /**< Number */
  ERROR,      /**< Error token */
  IF,         /**< If keyword */
  THEN,       /**< Then keyword */
  ELSE,       /**< Else keyword */
  END,        /**< End keyword */
  REPEAT,     /**< Repeat keyword */
  UNTIL,      /**< Until keyword */
  READ,       /**< Read keyword */
  WRITE,      /**< Write keyword */
  END_OF_FILE /**< End of file */
} TokenType;

/**
 * @brief Structure representing a token.
 */
typedef struct {
  TokenType type; /**< Type of the token */
  char* lexeme;   /**< Lexeme of the token */
} Token;

/**
 * @brief Enumeration of possible states in the scanner.
 */
typedef enum {
  START,     /**< Start state */
  INNUM,     /**< Inside a number */
  INID,      /**< Inside an identifier */
  INASSIGN,  /**< Inside an assignment */
  INCOMMENT, /**< Inside a comment */
  DONE       /**< Done state */
} State;

/**
 * @brief Get the next token from the file.
 *
 * @param file The file to read from.
 * @return Token The next token.
 */
Token getNextToken(FILE* file);

#endif
