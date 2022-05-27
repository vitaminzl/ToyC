#ifndef INTER
#define INTER
#include "../Lexer/Lexer.h"
#include <string>
using std::string;
class Node{
protected:
    int lexline = 0;
public:
    static const int labels;
    Node(int);
    ~Node();
    void error(string s);
    int newLabel();
    void printLabel(int);
    void print(string s);
};

class Expr: public Node{
public: 
    Token* Op;
    Type* type;
    Expr(Token*, Type*);
    virtual ~Expr();
    void printJumps();
    virtual Expr* gen();
    virtual Expr* reduce();
    virtual void jump(int t, int f);
    virtual string tostring();
};

class Id: public Expr{
private:
    int offset;
public:
    Id(Word* id, Type* p, int b);
    int getOffset();
};

class Op: public Expr{

};


class Constant: public Expr{
public:
    static const Constant True;
    static const Constant False;
    Constant(Token* tok, Type* p);
    Constant(int i);
    ~Constant();
    virtual void jump();
};

/* 符号表 */
class Scope: public Type{
private:
    map<Token*, Id*> table;
    Scope* prev;
public:
    Scope(Scope* n);
    ~Scope();
    void put(Token* , Id*);
    Id* get(Token*);
};


#endif