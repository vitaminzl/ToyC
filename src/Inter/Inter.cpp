#include "Inter.h"
#include <iostream>
#include <fstream>
#include <string>
using std::string;
using std::to_string;
using std::cout;
using std::endl;
using std::ofstream;
/* ----------- Node实现部分 ------------ */

/* 初始化标号为0 */
int Node::labels = 0;

Print* Node::p = new Print(cout);

Node::Node(int l): lexline(l){}

void Node::setOutput(ostream& o){
    delete p;
    p = new Print(o);
}

/* 输出错误信息并换行 */
void Node::error(string s){
    cout << s << endl;
}

/* 增加一个标号并输出 */
int Node::newLabel(){
    return ++ Node::labels;
}

/* 输出标号 */
void Node::printLabel(int l){
    p->output << "L" << l << ":";
}

/* 打印字符串并换行 */
void Node::print(string s){
    p->output << "\t" << s << endl;
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
void Expr::jump(int t, int f)const{
    printJumps(toString(), t, f);
}

void Expr::printJumps(string s, int t, int f)const{
    string strif("if ");
    string strifF("if False ");
    string strgoto("goto");
    string sp(" ");
    string strT = string(" L") + to_string(t);
    string strF = string(" L") + to_string(f);
    if(t && f){
        print(strif + s + sp + strgoto + strT);
        print(strgoto + strF);
    }
    else if(t){
        print(strif + s + sp + strgoto + strT);
    }
    else if(f){
        print(strifF + s + sp + strgoto + strF);
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

void Access::jump(int t, int f)const{
    const Expr* e = this->reduce();
    string s = e->toString();
    delete e;
    printJumps(s, t, f);
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


/*
    逻辑表达式生成中间代码，由Set或SetElem调用
    需要生成2个临时标号一个临时变量：
    并且将计算值存到临时变量中
            if False this goto L1
            t = true
            goto L2
    L1:     t = false
    L2:     x = t
*/
const Expr* Logical::gen()const{
    int f = newLabel();
    int t = newLabel();
    this->jump(0, f);
    const Expr* temp = new Temp(this->type);
    print(temp->toString() + string(" = ") + Constant::True.toString());
    print(string("goto ") + "L" + to_string(t));
    printLabel(f);
    print(temp->toString() + string(" = ") + Constant::False.toString());
    printLabel(t);
    return temp;
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
    expr1->jump(label, 0);
    expr2->jump(label, f);
    if (!t) printLabel(label);
}

/* ---------------- And的实现 ------------------- */

And::And(const Expr* e1, const Expr* e2): 
Logical(&Word::And, e1, e2){}

/* And的短路原则，如果第一个表达式错误则直接跳转 */
void And::jump(int t, int f)const {
    int label = (f != 0? f : newLabel());
    expr1->jump(0, label);
    expr2->jump(t, label);
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
    delete e1;
    delete e2;
    printJumps(s, t, f);
}

/* --------------------- Stmt的实现 ------------------------ */

Stmt Stmt::Null = Stmt();
Stmt* Stmt::Enclosure = &Stmt::Null;

/* ---------------------- Set的实现 -------------------------- */

Set::Set(const Id* i, const Expr* e): id(i), expr(e){}

void Set::gen(int b, int a){
    const Expr* e = expr->gen();
    print(id->toString() + string(" = ") + e->toString());
    delete e;
}

/* ---------------------- SetElem的实现 -------------------------- */

SetElem::SetElem(const Id* i, const Expr* e1, const Expr* e2):
array(i), index(e1), expr(e2){}


void SetElem::gen(int b, int a){
    const Expr* ix = index->reduce();
    const Expr* ex = expr->reduce();
    print(array->toString() + string("[") + ix->toString() + string("] = ") + ex->toString());
    delete ix;
    delete ex;
}


/* ------------------------ Seq的实现 --------------------------- */

Seq::Seq(Stmt* s1, Stmt* s2): stmt1(s1), stmt2(s2){}

/* 一个语句块生成一个Label */
void Seq::gen(int b, int a){
    if (stmt1 == &Stmt::Null) 
        stmt2->gen(b, a);
    else if (stmt2 == &Stmt::Null)
        stmt1->gen(b, a);
    else {
        int label = newLabel();
        stmt1->gen(b, label);
        printLabel(label);
        stmt2->gen(label, a);
    }
}

/* --------------------- If的实现 ---------------------- */

If::If(const Expr* e, Stmt* s): expr(e), stmt(s){
    if (!Type::isBool(e->type))
        error("Need Bool in If");
}


/*  对于If的代码生成来说，b和a表示入口和出口
    先对表达式进行短路性跳转 
    begin:  If False expr goto Label2    
    Label1: Stmt(entry: Label1, exit: after)
*/
void If::gen(int begin, int after){
    int label = newLabel();
    expr->jump(0, label);
    printLabel(label);
    stmt->gen(label, after);
}

/* --------------------- Else的实现 ------------------------ */

Else::Else(const Expr* e, Stmt* s1, Stmt* s2):
expr(e), stmt1(s1), stmt2(s2){
    if (!Type::isBool(e->type))
        error("Need Bool in IF-Else");
}


/*  begin:  If False expr goto label2    
    label1: Stmt(entry: label1, exit: label2)
    label2: Stmt(entry: label2, exit: after)
*/
void Else::gen(int begin, int after){
    int label1 = newLabel();
    int label2 = newLabel();
    expr->jump(0, label2);
    printLabel(label1);
    stmt1->gen(label1, label2);
    printLabel(label2);
    stmt2->gen(label2, after);
}


/* -------------------- While的实现 ----------------------- */

void While::init(const Expr* e, Stmt* s){
    if (!Type::isBool(e->type))
        error("Need Bool in While");
    expr = e;
    stmt = s;
}

/*  begin:  If expr goto label  else goto after
    label:  Stmt(entry: label, exit: begin)
    ------------
    begin:  If False expr goto after
    label:  Stmt(entry: label, exit: begin)
            goto label
*/
void While::gen(int begin, int after){
    int label = newLabel();
    savedAfter = after;
    expr->jump(label, after);
    printLabel(label);
    stmt->gen(label, begin);
}

/* ------------------- Do的实现 ---------------------- */

void Do::init(const Expr* e, Stmt* s){
    if (!Type::isBool(e->type))
        error("Need Bool in While");
    expr = e;
    stmt = s;
}

/*  begin:  Stmt(entry: begin, exit: label)
    label:  if expr goto begin else goto after
*/
void Do::gen(int begin, int after){
    int label = newLabel();
    savedAfter = after;
    stmt->gen(begin, label);
    printLabel(label);
    expr->jump(begin, after);
}



/* ------------------ break的实现 ---------------------*/

Break::Break(){
    if(Stmt::Enclosure == &Stmt::Null)
        error("Not Enclosed");
    stmt = Stmt::Enclosure;
}


void Break::gen(int begin, int after){
    print(string("goto L") + to_string(after));
}