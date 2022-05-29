#ifndef INTER
#define INTER
#include <string>
#include <map>
#include "Lexer.h"
using std::string;
// using namespace std;
/*�����﷨���еĽ��*/
class Node {
protected:
    int lexline = 0;             //Դ�����е��к�
public:
    static const int labels;
    Node(int);
    ~Node();
    void error(string s);
    int newLabel();
    void printLabel(int);
    void print(string s);
};

/*���ʽ�ࣺNode������*/
class Expr : public Node {
public:
    Token* Op;                    //����ϵ������
    Type* type;                   //����ϵ�����
    Expr(Token*, Type*);
    ~Expr();
    void printJumps();
    virtual Expr* gen();
    virtual Expr* reduce();
    virtual void jump(int t, int f);
    virtual string tostring();
};

/*��ʶ���ࣺExpr������
��Ӧ��һ����ʶ������Id�Ľ����һ��Ҷ�ӽ��*/
class Id : public Expr {
private:
    int offset;                         //��ʶ������Ե�ַ
public:
    Id(Word* id, Type* p, int b);
    ~Id();
    int getOffset();
};



/* ���ű� */
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