#include <string>
#include <iostream>
#include "../Inter/Inter.h"
using namespace std;
/*Node类的实现*/
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
/*Expr类的实现*/
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
/*Id类的实现*/
Id::Id(Word* id, Type* p, int b):Expr(id,p),offset(b) {}
Id::~Id(){}
int Id::getOffset()
{
	return offset;
}
/*Op类的实现*/
/*
Op::Op(Word* id, Type* p) :Expr(id, p) {}
Op::~Op(){}
*/
/* Scope类的实现 */
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
