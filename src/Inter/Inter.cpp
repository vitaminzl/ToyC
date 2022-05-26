#include "Inter.h"
#include <iostream>
#include <string>
using std::string;
using std::to_string;
using std::cout;
using std::endl;
/* ----------- Node实现部分 ------------ */

/* 初始化标号为0 */
int Node::labels = 0;

Node::Node(int l): lexline(l){}

Node::~Node(){}

/* 输出错误信息并换行 */
void Node::error(string s){
    cout << s << endl;
}

/* 增加一个标号并输出 */
int Node::newLabel(){
    return ++ Node::labels;
}

/* 输出标号 */
void Node::printLabel(int l)const{
    cout << "L" << l << ":";
}

/* 打印字符串并换行 */
void Node::print(string s)const{
    cout << s << endl;
}

/* -------------- Expr实现部分 ---------------*/

Expr::Expr(const Token* tok, const Type* ty): op(tok), type(ty){}

/* 返回本身，适用于Temp，Id这种单地址变量 */
const Expr* Expr::gen()const{
    return this;
}

/* 返回本身，适用于Temp，Id这种单地址变量 */
const Expr* Expr::reduce()const{
    return this;
}

string Expr::toString()const{
    return op->toString();
}

/* 基本跳转语句，正确则跳至标号t，错误则跳至标号f
   若出现标号0，则不跳 */
void Expr::jump(int t, int f){
    string strif("if ");
    string strifF("ifFalse ");
    string strgoto(" goto");
    string strT = string(" L") + to_string(t);
    string strF = string(" L") + to_string(f);
    if(t && f){
        print(strif + toString() + strgoto + strT);
        print(strgoto + strF);
    }
    else if(t){
        print(strif + toString() + strgoto + strT);
    }
    else if(f){
        print(strifF + toString() + strgoto + strF);
    }
    else;
}


/* ------------- Id实现部分 ------------ */

Id::Id(const Word* w, const Type* i, int o): Expr(w, i), offset(o){}

int Id::getOffset(){
    return offset;
}

/* ------------- Temp实现部分 ------------ */

int Temp::count = 0;


Temp::Temp(const Type* t): Expr(&Word::temp, t){
    num = ++ count;
}

Temp::Temp(const Word* w, const Type* t): Expr(w, t) {
    num = ++ count;
}

string Temp::toString()const{
    return string("t") + to_string(num);
}

/* ------------ Op实现部分 ------------- */

Op::Op(const Token* tok, const Type* ty): Expr(tok, ty){}

/* 对双目运算、单目运算以及数组进行归约为一个临时变量  */
const Expr* Op::reduce()const{
    const Expr* arith = gen();
    Temp* temp = new Temp(arith->type);
    print(temp->toString() + string(" = ") + arith->toString());
    return temp;
}

/* --------------- 单目运算 ------------- */


Unary::Unary(const Token* t, const Expr* e): Op(t, nullptr), expr(e){}

const Expr* Unary::gen()const{
    return new Unary(op, expr->reduce());
}


