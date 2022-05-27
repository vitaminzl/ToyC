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
    output << "L" << l << ":";
}

/* 打印字符串并换行 */
void Node::print(string s)const{
    output << s << endl;
} 

ostream& Node::output = cout;

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
void Expr::jump(int t, int f)const{
    printJumps(toString(), t, f);
}

void Expr::printJumps(string s, int t, int f)const{
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
    delete arith;
    return temp;
}

/* --------------- 单目运算 ------------- */


Unary::Unary(const Token* t, const Expr* e): Op(t, nullptr), expr(e){}

const Expr* Unary::gen()const{
    return new Unary(op, expr->reduce());
}

string Unary::toString()const{
    return op->toString() + expr->toString();
}

/* --------------- 双目运算 ----------------*/

Arith::Arith(const Token* t, const Expr* e1, const Expr* e2): 
Op(t, nullptr), expr1(e1), expr2(e2){}

const Expr* Arith::gen()const{
    return new Arith(op, expr1->reduce(), expr2->reduce());
}

string Arith::toString()const {
    string sp(" ");
    return expr1->toString() + sp + op->toString() + sp + expr2->toString();
}

/* -------------- Access实现 ----------------- */

Access::Access(const Id* i, const Expr* ix, const Type* t):
array(i), index(ix){
    const Token* w = new Word(string("[]"), Tag::INDEX);
    Op(w, t);
}

const Expr* Access::gen()const{
    return new Access(array, index->reduce(), type);
}

void Access::jump()const{

}

string Access::toString()const{
    string l("["), r("]");
    return  array->toString() + l + index->toString() + r;  
}

/* ------------- Constant的实现 ------------ */

const Constant Constant::True = Constant(&Word::True, &Type::Bool);
const Constant Constant::False = Constant(&Word::False, &Type::Bool);

Constant::Constant(const Token* tok, const Type* ty): Expr(tok, ty){}

Constant::Constant(int i): Expr(new Number(i, Tag::NUM), &Type::Int){}

/* 常量jump函数，用于True、False */
void Constant::jump(int t, int f)const{
    if(this != &Constant::True && !t) 
        print(string("goto L") + to_string(t));
    else if(this != &Constant::False && !f)
        print(string("goto L") + to_string(f));
}

/* --------------- Logical的实现 ---------------- */

Logical::Logical(const Token* o, const Expr* e1, const Expr* e2):
Constant(o, &Type::Bool), expr1(e1), expr2(e2){}

const Expr* Logical::gen()const{
    return new Logical(op, expr1, expr2);
}

string Logical::toString()const{
    string sp = string(" ");
    return expr1->toString() + sp + op->toString() + sp + expr2->toString();
}


/* -------------- Or的实现 --------------- */

Or::Or(const Expr* expr1, const Expr* expr2):
Logical(&Word::Or, expr1, expr2) {}


/* Or的短路原则，如果第一个表达式正确则直接跳转到t */
void Or::jump(int t, int f)const{
    int label = (t != 0? t : newLabel());
    expr1->jump(t, 0);
    expr2->jump(t, f);
    if (!t) printLabel(label);
}

/* ---------------- And的实现 ------------------- */

And::And(const Expr* e1, const Expr* e2): 
Logical(&Word::And, e1, e2){}

/* And的短路原则，如果第一个表达式错误则直接跳转 */
void And::jump(int t, int f)const {
    int label = (f != 0? f : newLabel());
    expr1->jump(0, f);
    expr2->jump(t, f);
    if (!f) printLabel(label);
}

/* ------------------- Not的实现 -------------------- */

Not::Not(const Expr* e): Logical(&Word::And, e, nullptr){}

void Not::jump(int t, int f)const{
    expr1->jump(f, t);
}

string Not::toString()const{
    return op->toString() + expr1->toString();
}

/* --------------------- Cmp的实现 ----------------------- */

Cmp::Cmp(const Token* tok, const Expr* e1, const Expr* e2): 
Logical(tok, e1, e2){}


/* 规约化2个表达式 */
void Cmp::jump(int t, int f)const {
    const Expr* e1 = expr1->reduce();
    const Expr* e2 = expr2->reduce();
    string s = e1->toString() + " " + op->toString() + " " + e2->toString();
    printJumps(s, t, f);
}

/* --------------------- Stmt的实现 ------------------------ */

const Stmt Stmt::Null = Stmt();


/* ---------------------- Set的实现 -------------------------- */

Set::Set(const Id* i, const Expr* e): id(i), expr(e){}

const Expr* Set::gen(int b, int a)const{
    const Expr* e = expr->gen();
    print(id->toString() + string(" = ") + e->toString());
    return id;
}

/* ---------------------- SetElem的实现 -------------------------- */

SetElem::SetElem(const Id* i, const Expr* e1, const Expr* e2):
array(i), index(e1), expr(e2){}


const Expr* SetElem::gen(int b, int a)const {
    const Expr* ix = index->reduce();
    const Expr* ex = expr->reduce();
    print(array->toString() + string("[") + ix->toString() + string("] = ") + ex->toString());
    return array;
}


/* ------------------------ Seq的实现 --------------------------- */

Seq::Seq(const Stmt* s1, const Stmt* s2): stmt1(s1), stmt2(s2){}

/* 一个语句块生成一个Label */
const Expr* Seq::gen(int b, int a)const{
    if (stmt1 == &Stmt::Null) 
        stmt2->gen(b, a);
    else if (stmt2 == &Stmt::Null)
        stmt1->gen(b, a);
    else {
        int label = newLabel();
        stmt1->gen(b, label);
        printLabel(label);
        stmt2->gen(b, a);
    }
}

