#ifndef PARSER
#define PARSER
#include "../Lexer/Lexer.h"
#include "../Inter/Inter.h"
#include <string>
class Parser {
private:
    Lexer* lex;                 //该语法分析器的词法分析器
    const Token* lookahead;     //向前看词法单元
    Scope* top = nullptr;       //当前或顶层的符号表
    int used = 0;               //用于变量声明的存储位置
public:
    Parser(Lexer* lex);
    ~Parser();
    void move();
    void error(string);
    void match(int);
    void program();
    void block();
    void decls();
    Type* type();
    Type* dims(Type*);
    void stmt();
    void stmts();
    void factor();
    /*
    void assign();
    Expr join();
    Expr equality();
    Expr cmp();
    Expr expr();
    Expr term();
    Expr unary();
    void offset(Id);
    */

};


#endif