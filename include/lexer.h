#ifndef LEXER_H
#define LEXER_H

#include "token.h"

// Lexical analyzer: takes an input string and fills the token list
void lexer(const char *input, token_list_t *list);

#endif /* LEXER_H */
