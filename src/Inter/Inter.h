#ifndef INTER
#define INTER
#include <string>
#include <map>
#include "Lexer.h"
using std::string;
// using namespace std;
/*抽象语法树中的结点*/
class Node {
protected:
    int lexline = 0;             //源程序中的行号
public:
    static const int labels;
    Node(int);
    ~Node();
    void error(string s);
    int newLabel();
    void printLabel(int);
    void print(string s);
};

/*表达式类：Node的子类*/
class Expr : public Node {
public:
    Token* Op;                    //结点上的运算符
    Type* type;                   //结点上的类型
    Expr(Token*, Type*);
    ~Expr();
    void printJumps();
    virtual Expr* gen();
    virtual Expr* reduce();
    virtual void jump(int t, int f);
    virtual string tostring();
};

/*标识符类：Expr的子类
对应于一个标识符的类Id的结点是一个叶子结点*/
class Id : public Expr {
private:
    int offset;                         //标识符的相对地址
public:
    Id(Word* id, Type* p, int b);
    ~Id();
    int getOffset();
};



/* 符号表 */
class Scope {
private:
    map<const Token*, Id*> table;
    Scope* prev;
public:
    Scope(Scope* n);
    ~Scope();
    void put(const Token*, Id*);
    Id* get(const Token*);
    void printscope();
};


#endif