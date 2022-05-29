#include <string>
#include <iostream>
#include "Inter.h"
using namespace std;
/*Node类的定义*/
Node::Node(int t = 1) :lexline(t) {}
Node::~Node() {}
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
/*Expr类的定义*/
Expr::Expr(Token* tok, Type* p) :Op(tok), type(p) {}
Expr::~Expr() {}
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
/*Id类的定义*/
Id::Id(Word* id, Type* p, int b) :Expr(id, p), offset(b) {}
Id::~Id() {}
int Id::getOffset()
{
	return offset;
}
/* Scope类的定义*/
Scope::Scope(Scope* n=nullptr) {
	prev = n;
}
Scope:: ~Scope() {}
void Scope::put(const Token* w, Id* i) {
	table.insert(std::pair<const Token*, Id*>(w, i));
}
Id* Scope::get(const Token* w) {
	/*调试用
	printscope();
	cout << "***************************************" << endl;
	cout << endl;
	*/
	for (Scope* sc = this; sc != nullptr; sc = sc->prev) {
		if (sc->table.count(w)) {
			Id* found = (Id*)(sc->table[w]);
			return found;
		}
	}
	return nullptr;
}
/*打印符号表层级*/
void Scope::printscope() {
	int i = 0;
	for (Scope* sc = this; sc != nullptr; sc = sc->prev) {
		cout << "----------------------------------------------" << endl;
		cout << "符号表第" << i++ << "层：" << endl;
		map<const Token*, Id*>::iterator it;
		for (it = sc->table.begin(); it != sc->table.end(); it++) {
			Type* tp = (Type*)(it->first);
			Id* id = (Id*)(it->second);
			cout << tp->toString() <<" " << id->type->toString() << endl;
		}
		cout<<"----------------------------------------------" << endl;
	}
}
