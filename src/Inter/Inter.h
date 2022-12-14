#ifndef INTER
#define INTER
#include "../Lexer/Lexer.h"
#include <string>
#include <iostream>
using std::ostream;
using std::string;
using std::cout;

class Print{
public: 
    ostream& output;
    Print():output(cout) {}
    Print(ostream& o):output(o) {}
};

class Node{
protected:
    static Print* p;
    static int labels;
public:
    Node(){}
    ~Node(){}
    static int newLabel();
    static void error(string s);
    static void setOutput(ostream& o);
    static void printLabel(int);
    static void printLine(int);
    static void print(string s);
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
    // virtual string toString()const;
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
    virtual void jump(int t, int f)const;
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
protected:
    int lexline = 0;
    int savedAfter = 0;
public:
    static Stmt Null;
    static Stmt* Enclosure;
    Stmt(){}
    Stmt(int );
    int getsavedAfter(){ return savedAfter; }
    int getLextline(){ return lexline; }
    virtual void gen(int b, int a){}
};

class Set: public Stmt{
public:
    const Id* id;
    const Expr* expr;
    Set(const Id* ,const Expr* );
    Set(const Id* ,const Expr* , int );
    virtual Type* check(Type*, Type*){}
    virtual void gen(int b, int a);
};


class SetElem: public Stmt{
public:
    const Id* array;
    const Expr* index;
    const Expr* expr;
    SetElem(const Id* , const Expr* , const Expr*);
    SetElem(const Id* , const Expr* , const Expr*, int );
    virtual Type* check(Type*, Type*){}
    virtual void gen(int b, int a);
};

class Seq: public Stmt{
private:
    int line1 = 0;
    int line2 = 0;
public:
    Stmt* stmt1;
    Stmt* stmt2;
    Seq(Stmt* , Stmt* );
    Seq(Stmt* , Stmt* , int);
    virtual void gen(int b, int a);
};

class If: public Stmt{
public:
    const Expr* expr;
    Stmt* stmt;
    If(const Expr* , Stmt* );
    If(const Expr* , Stmt* , int );
    virtual void gen(int b, int a);
};

class Else: public Stmt{
public:
    const Expr* expr;
    Stmt* stmt1;
    Stmt* stmt2;
    Else(const Expr* , Stmt* , Stmt* );
    Else(const Expr* , Stmt* , Stmt* , int );
    virtual void gen(int b, int a);
};

class While: public Stmt{
public:
    const Expr* expr;
    Stmt* stmt;
    While():expr(nullptr), stmt(nullptr){}
    void init(const Expr* , Stmt*);
    void init(const Expr* , Stmt*, int );
    virtual void gen(int b, int a);
};

class Do: public Stmt{
public:
    const Expr* expr;
    Stmt* stmt;
    Do(): expr(nullptr), stmt(nullptr){}
    void init(const Expr* , Stmt* );
    void init(const Expr* , Stmt* , int );
    virtual void gen(int b, int a);
};

class Break: public Stmt{
public:
    Stmt* stmt;
    Break();
    Break(int l);
    virtual void gen(int b, int a);
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