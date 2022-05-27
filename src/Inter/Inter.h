#ifndef INTER
#define INTER
#include "../Lexer/Lexer.h"
#include <string>
#include <iostream>
// using namespace std;
using std::ostream;
using std::string;
using std::cout;


class Node{
protected:
    int lexline = 0;
public:
    static ostream& output;
    static int labels;
    static int newLabel();
    Node(){}
    Node(int);
    ~Node(){}
    void error(string s);
    void SetOutput(const ostream&);
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
    void printJumps(string, int, int)const;
    virtual const Expr* gen()const;
    virtual const Expr* reduce()const;
    virtual void jump(int t, int f)const;
    virtual string toString()const;
};

class Id: public Expr{
private:
    int offset;
public:
    Id(const Word* id): Expr(id, &Type::Int), offset(0){}   // 模块调试使用
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
    Op(const Token* , const Type* );
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
    const Expr* expr1;
    const Expr* expr2;
    Arith(const Token* , const Expr* , const Expr*);
    virtual const Expr* gen()const;
    virtual string toString()const;
};

class Access: public Op{
public:
    const Id* array;
    const Expr* index;
    Access(const Id* , const Expr*, const Type* );
    virtual const Expr* gen()const;
    virtual void jump()const;
    virtual string toString()const;
};

class Constant: public Expr{
public:
    static const Constant True;
    static const Constant False;
    Constant(const Token* tok, const Type* p);
    Constant(int i);
    ~Constant(){}
    virtual void jump(int t, int f)const;
};

class Logical: public Constant{
public:
    const Expr* expr1;
    const Expr* expr2;
    Logical(const Token* , const Expr* , const Expr* );
    virtual Type* check(Type* , Type* ){}
    virtual const Expr* gen()const;
    virtual string toString()const;
};

class Or: public Logical{
public:
    Or(const Expr* , const Expr* );
    virtual void jump(int t, int f)const;
};

class And: public Logical{
public:
    And(const Expr* , const Expr* );
    virtual void jump(int t, int f)const;
};

class Not: public Logical{
public:
    Not(const Expr*);
    virtual void jump(int t, int f)const;
    virtual string toString()const;
};

class Cmp: public Logical{
public:
    Cmp(const Token* , const Expr* , const Expr* );
    virtual Type* check(Type*, Type*){}
    virtual void jump(int t, int f)const;
};


class Stmt: public Node{
public:
    static const Stmt Null;
    Stmt(){}
    int after = 0;
    virtual const Expr* gen(int b, int a)const{}
};

class Set: public Stmt{
public:
    const Id* id;
    const Expr* expr;
    Set(const Id* ,const Expr* );
    virtual Type* check(Type*, Type*){}
    virtual const Expr* gen(int b, int a)const;
};


class SetElem: public Stmt{
public:
    const Id* array;
    const Expr* index;
    const Expr* expr;
    SetElem(const Id* , const Expr* , const Expr*);
    virtual Type* check(Type*, Type*){}
    virtual const Expr* gen(int b, int a)const;
};

class Seq: public Stmt{
public:
    const Stmt* stmt1;
    const Stmt* stmt2;
    Seq(const Stmt* , const Stmt* );
    virtual const Expr* gen(int b, int a)const;
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