#ifndef PARSER
#define PARSER
#include "../Lexer/Lexer.h"
#include "../Inter/Inter.h"

class Parser {
private:
    Lexer lex;
    Token* lookahead;
    Scope* top = nullptr;
    int used = 0;
public:
    Parser();
    ~Parser();

};


#endif