#ifndef PARSER_H
#define PARSER_H

/**
 * @brief Node type
 *
 * This enumeration represents the type of a node in the abstract syntax tree.
 */
typedef enum {
  NODE_IF,     /**< If node */
  NODE_REPEAT, /**< Repeat node */
  NODE_ASSIGN, /**< Assignment node */
  NODE_READ,   /**< Read node */
  NODE_WRITE,  /**< Write node */
  NODE_OP,     /**< Operator node */
  NODE_CONST,  /**< Number literal node */
  NODE_ID,     /**< Identifier node */
} NodeType;

/**
 * @brief Node structure
 *
 * This structure represents a node in the abstract syntax tree.
 */
typedef struct Node {
  NodeType type;      /**< Type of the node */
  struct Node* left;  /**< Left child */
  struct Node* right; /**< Right child */
  struct Node* next;  /**< Next node */
  char* value;        /**< Value of the node */
} Node;

/**
 * @brief Parse the program.
 *
 * This function parses the program.
 *
 * @return Node* The root node of the abstract syntax tree.
 */
Node* program();

/**
 * @brief Parse a statement sequence.
 *
 * This function parses a statement sequence.
 *
 * @return Node* The root node of the abstract syntax tree.
 */
Node* stmt_sequence();

/**
 * @brief Parse a statement.
 *
 * This function parses a statement.
 *
 * @return Node* The root node of the abstract syntax tree.
 */
Node* statement();

/**
 * @brief Parse an if statement.
 *
 * This function parses an if statement.
 *
 * @return Node* The root node of the abstract syntax tree.
 */
Node* if_stmt();

/**
 * @brief Parse a repeat statement.
 *
 * This function parses a repeat statement.
 *
 * @return Node* The root node of the abstract syntax tree.
 */
Node* repeat_stmt();

/**
 * @brief Parse an assign statement.
 *
 * This function parses an assign statement.
 *
 * @return Node* The root node of the abstract syntax tree.
 */
Node* assign_stmt();

/**
 * @brief Parse an expression.
 *
 * This function parses an expression.
 *
 * @return Node* The root node of the abstract syntax tree.
 */
Node* expr();

/**
 * @brief Parse a simple expression.
 *
 * This function parses a simple expression.
 *
 * @return Node* The root node of the abstract syntax tree.
 */
Node* simple_expr();

/**
 * @brief Parse a term.
 *
 * This function parses a term.
 *
 * @return Node* The root node of the abstract syntax tree.
 */
Node* term();

/**
 * @brief Parse a factor.
 *
 * This function parses a factor.
 *
 * @return Node* The root node of the abstract syntax tree.
 */
Node* factor();

#endif
