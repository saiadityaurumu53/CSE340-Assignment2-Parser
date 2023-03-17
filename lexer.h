/*
 * Copyright (C) Rida Bazzi, 2016
 *
 * Do not share this file with anyone
 */
#ifndef __LEXER__H__
#define __LEXER__H__

#include <vector>
#include <string>

#include "inputbuf.h"

// ------- token types -------------------

// typedef enum { END_OF_FILE = 0,
//     IF, WHILE, DO, THEN, PRINT,
//     PLUS, MINUS, DIV, MULT,
//     EQUAL, COLON, COMMA, SEMICOLON,
//     LBRAC, RBRAC, LPAREN, RPAREN,
//     NOTEQUAL, GREATER, LESS, LTEQ, GTEQ,
//     DOT, NUM, ID, ERROR, REALNUM, BASE08NUM, BASE16NUM // TODO: Add labels for new token types here
// } TokenType;

typedef enum { END_OF_FILE = 0, PUBLIC, PRIVATE, EQUAL, COLON, COMMA, SEMICOLON, LBRACE, RBRACE, ID, ERROR, COMMENT
} TokenType;

class Token {
  public:
    void Print();

    std::string lexeme;
    TokenType token_type;
    int line_no;
};

class LexicalAnalyzer {
  public:
    Token GetTokenMain();
    TokenType UngetToken(Token);
    LexicalAnalyzer();
    std::vector<Token> tokens;
    Token peek(int);
    Token GetToken();


  private:
    
    int line_no;
    int index;
    Token tmp;
    InputBuffer input;

    void SkipSpace();
    bool IsKeyword(std::string);
    TokenType FindKeywordIndex(std::string);
    Token ScanIdOrKeyword();
    Token ScanNumber();
};

#endif  //__LEXER__H__