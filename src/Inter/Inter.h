#ifndef INTER
#define INTER
#include "../Lexer/Lexer.h"
#include <string>
using std::string;
class Node{
protected:
    int lexline = 0;
public:
    static int labels;
    Node(){}
    Node(int);
    ~Node();
    void error(string s);
    int newLabel();
    void printLabel(int)const;
    void print(string s)const;
};

class Expr: public Node{
public: 
    const Token* op;
    const Type* type;
    Expr(): op(nullptr), type(nullptr){}
    Expr(const Token*, const Type*);
    ~Expr(){}
    void printJumps();
    virtual const Expr* gen()const;
    virtual const Expr* reduce()const;
    virtual void jump(int t, int f);
    virtual string toString()const;
};

class Id: public Expr{
private:
    int offset;
public:
    Id(const Word* id, const Type* p, int b);
    int getOffset();
};

class Temp: public Expr{
public:
    static int count;
    int num;
    Temp(const Type* );
    Temp(const Expr* );
    Temp(const Word* , const Type* );
    virtual string toString()const;
};

class Op: public Expr{
public:
    Op(){}
    Op(const Token* , const Type* ){}
    virtual const Expr* reduce()const;
};

class Unary: public Op{
public:
    const Expr* expr;
    Unary(const Token* tok, const Expr* );
    virtual const Expr* gen()const;
    virtual string toString()const;
};

class Arith: public Op{
public:
    Expr* expr1;
    Expr* expr2;
    virtual const Expr* gen()const;
    virtual string toString()const;
};

class Constant: public Expr{
public:
    static const Constant True;
    static const Constant False;
    Constant(Token* tok, Type* p);
    Constant(int i);
    ~Constant();
    virtual void jump(int t, int f);
};

class Logical: public Constant{
public:
    Expr* expr1;
    Expr* epxr2;
    virtual Type* check(Type* , Type* ){}
    virtual Expr* gen();
    virtual string toString();
};

class Or: public Logical{
public:
    virtual void jump(int t, int f);
};

class And: public Logical{
public:
    virtual void jump(int t, int f);
};

class Not: public Logical{
public:
    virtual void jump(int t, int f);
    virtual string toString();
};

class Cmp: public Logical{
public:
    virtual Type* check(Type*, Type*){}
    virtual void jump(int t, int f);
};


class Stmt: public Node{
public:
    static Stmt* Null;
    int after;
    virtual Expr* gen(int b, int a);
};

class Set: public Stmt{
public:
    Id* id;
    Expr* expr;
    virtual Type* check(Type*, Type*){}
    virtual Expr* gen(int b, int a);
};


class SetElem: public Stmt{
public:
    Id* array;
    Expr* index;
    Expr* expr;
    virtual Type* check(Type*, Type*){}
    virtual Expr* gen(int b, int a);
};

class Seq: public Stmt{
public:
    const Stmt* stmt1;
    const Stmt* stmt2;
    virtual Expr* gen(int b, int a);
};


/* 符号表 */
// class Scope: public Type{
// private:
//     map<Token, Id> table;
//     Scope* prev;
// public:
//     Scope();
//     ~Scope();
//     void put(Type, Id);
//     void get(Id);
// };


#endif