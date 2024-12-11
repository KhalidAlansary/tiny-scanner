#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "scanner.h"

Token current_token;

static void malloc_error() {
  perror("stop being poor and buy more ram");
  exit(EXIT_FAILURE);
}

static Node* create_node(const NodeType type) {
  Node* node = (Node*)malloc(sizeof(Node));
  if (!node) {
    malloc_error();
  }
  node->type = type;
  node->value = NULL;
  node->left = NULL;
  node->right = NULL;
  node->next = NULL;
  return node;
}

static void match(TokenType expected) {
  if (current_token.type == expected) {
    current_token = getNextToken();
  } else {
    fprintf(stderr, "Error: Expected %d, got %d\n", expected,
            current_token.type);
    exit(EXIT_FAILURE);
  }
}

Node* program() {
  current_token = getNextToken();
  Node* node = stmt_sequence();
  if (current_token.type != END_OF_FILE) {
    fprintf(stderr, "Error: Expected END_OF_FILE, got %d\n",
            current_token.type);
    exit(EXIT_FAILURE);
  }
  return node;
}

Node* stmt_sequence() {
  Node* node = statement();
  Node* temp = node;
  while (current_token.type == SEMI) {
    match(SEMI);
    Node* next = statement();
    if (next != NULL) {
      if (node == NULL) {
        node = next;
        temp = node;
      } else {
        temp->next = next;
        temp = next;
      }
    }
  }
  return node;
}

Node* statement() {
  Node* node = NULL;
  switch (current_token.type) {
    case IF:
      node = if_stmt();
      break;
    case REPEAT:
      node = repeat_stmt();
      break;
    case ID:
      node = assign_stmt();
      break;
    case READ:
      match(READ);
      node = create_node(NODE_READ);
      node->value = current_token.lexeme;
      match(ID);
      break;
    case WRITE:
      match(WRITE);
      node = create_node(NODE_WRITE);
      node->left = expr();
      break;
    default:
      fprintf(stderr, "Error: Expected IF, REPEAT, or IDENTIFIER, got %s\n",
              tokenTypeToString(current_token.type));
      exit(EXIT_FAILURE);
  }
  return node;
}

Node* if_stmt() {
  Node* node = create_node(NODE_IF);
  match(IF);
  node->left = expr();
  match(THEN);
  node->right = stmt_sequence();
  if (current_token.type == ELSE) {
    match(ELSE);
    node->next = stmt_sequence();
  }
  match(END);
  return node;
}

Node* repeat_stmt() {
  Node* node = create_node(NODE_REPEAT);
  match(REPEAT);
  node->left = stmt_sequence();
  match(UNTIL);
  node->right = expr();
  return node;
}

Node* assign_stmt() {
  Node* node = create_node(NODE_ASSIGN);
  node->value = current_token.lexeme;
  match(ID);
  match(ASSIGN);
  node->left = expr();
  return node;
}

Node* expr() {
  Node* node = simple_expr();
  if (current_token.type == LT || current_token.type == EQ) {
    Node* temp = create_node(NODE_OP);
    temp->value = current_token.lexeme;
    temp->left = node;
    node = temp;
    match(current_token.type);
    temp->right = simple_expr();
  }
  return node;
}

Node* simple_expr() {
  Node* node = term();
  while (current_token.type == ADD || current_token.type == SUB) {
    Node* temp = create_node(NODE_OP);
    temp->value = current_token.lexeme;
    temp->left = node;
    node = temp;
    match(current_token.type);
    temp->right = term();
  }
  return node;
}

Node* term() {
  Node* node = factor();
  while (current_token.type == MUL || current_token.type == DIV) {
    Node* temp = create_node(NODE_OP);
    temp->value = current_token.lexeme;
    temp->left = node;
    node = temp;
    match(current_token.type);
    temp->right = factor();
  }
  return node;
}

Node* factor() {
  Node* node = NULL;
  switch (current_token.type) {
    case NUM:
      node = create_node(NODE_CONST);
      node->value = current_token.lexeme;
      match(NUM);
      break;
    case ID:
      node = create_node(NODE_ID);
      node->value = current_token.lexeme;
      match(ID);
      break;
    case LPAREN:
      match(LPAREN);
      node = expr();
      match(RPAREN);
      break;
    default:
      fprintf(stderr, "Error: Expected NUMBER, IDENTIFIER, or LPAREN, got %d\n",
              current_token.type);
      exit(EXIT_FAILURE);
  }
  return node;
}

Node* parse() {
  current_token = getNextToken();
  return program();
}

char* nodeTypeToString(NodeType type) {
  switch (type) {
    case NODE_ASSIGN:
      return "ASSIGN";
    case NODE_CONST:
      return "CONST";
    case NODE_ID:
      return "ID";
    case NODE_IF:
      return "IF";
    case NODE_OP:
      return "OP";
    case NODE_READ:
      return "READ";
    case NODE_REPEAT:
      return "REPEAT";
    case NODE_WRITE:
      return "WRITE";
    default:
      return "UNKNOWN";
  }
}
