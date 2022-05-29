#ifndef PARSER
#define PARSER
#include "../Lexer/Lexer.h"
#include "../Inter/Inter.h"
#include <string>
class Parser {
private:
    Lexer* lex;                 //���﷨�������Ĵʷ�������
    const Token* lookahead;     //��ǰ���ʷ���Ԫ
    Scope* top = nullptr;       //��ǰ�򶥲�ķ��ű�
    int used = 0;               //���ڱ��������Ĵ洢λ��
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