#include <string>
#include <iostream>
#include "../Inter/Inter.h"
using namespace std;
/*Node���ʵ��*/
Node::Node(int t=1):lexline(t) {}
Node::~Node(){}
void Node::error(string s)
{
}
int Node::newLabel()
{
	return 0;
}
void Node::printLabel(int)
{
}
void Node::print(string s)
{
}
/*Expr���ʵ��*/
Expr::Expr(Token* tok, Type* p):Op(tok),type(p) {}
Expr::~Expr(){}
void Expr::printJumps()
{
}
Expr* Expr::gen()
{
	return nullptr;
}
Expr* Expr::reduce()
{
	return nullptr;
}
void Expr::jump(int t, int f)
{
}
string Expr::tostring()
{
	return string();
}
/*Id���ʵ��*/
Id::Id(Word* id, Type* p, int b):Expr(id,p),offset(b) {}
Id::~Id(){}
int Id::getOffset()
{
	return offset;
}
/*Op���ʵ��*/
/*
Op::Op(Word* id, Type* p) :Expr(id, p) {}
Op::~Op(){}
*/
/* Scope���ʵ�� */
Scope::Scope(Scope* n){
	prev = n;
}
Scope:: ~Scope(){}
void Scope::put(const Token* w, Id* i) {
	table.insert(std::pair<const Token*, Id*>(w, i));
}
Id* Scope::get(const Token* w) {
	for (Scope* sc = this; sc != nullptr; sc = sc->prev) {
		if (sc->table.count(w)) {
			Id* found = (Id*)(sc->table[w]);
			return found;
		}
	}
	return nullptr;
}
