#ifndef PARSER
#define PAESER
#include "../Lexer/Lexer.h"
#include "../Inter/Inter.h"
#include <iostream>
using namespace std;

class Parser{
private:
    Lexer& lex;
    const Token* lookahead;
    ostream& output;
public:
    Parser(Lexer& lex, ostream&);
    ~Parser(){}
    void error(string s);
    void move();
    void match(int t);
    void program();
    const Stmt* stmts();
    const Stmt* stmt();
    const Stmt* assign();
    const Access* offset(const Id* );
    const Expr* bools();
    const Expr* join();
    const Expr* equality();
    const Expr* cmp();
    const Expr* expr();
    const Expr* term();
    const Expr* unary();
    const Expr* factor();
};

#endif