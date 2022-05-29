#include "Parser.h"
#include "Inter.h"
#include <iostream>
using namespace std;
/*Parser类的定义*/
Parser::Parser(Lexer* l) :lex(l), lookahead(l->scan()), top(nullptr), used(0) {}
Parser::~Parser() {}
void Parser::move() {
	lookahead = lex->scan();
}
void Parser::error(string s) {
	throw (string("near line") + to_string(lex->getline()) + ":" + s);
}
void Parser::match(int t) {
	if (lookahead->tag == t) {
		//cout <<"match：" << lookahead->toString() << endl;
		move();
	}
	else
		error("error:unmatch");
}
void Parser::program() {
	block();
	cout << endl;
}
void Parser::block() {
	//block->{ decls stmts }
	match('{');
	cout << "{";
	Scope* saved = top;
	top = new Scope(saved);
	decls();
	stmts();
	match('}');
	cout << "}";
	delete top;
	top = saved;
}
Type* Parser::type() {
	Type* p = (Type*)lookahead;
	match(Tag::BASIC);
	if (lookahead->tag != '[')
		return p;
	else
		return dims(p);
}
void Parser::decls() {
	//decls->type id; type id; type id;type id; ...
	while (lookahead->tag == Tag::BASIC) {
		Type* p = type();
		const Token* tok = lookahead;
		Word* w = new Word(p->value, lookahead->tag);
		match(Tag::ID);
		match(';');
		Id* id = new Id(w, p, used);
		top->put(tok, id);
		used = used + p->width;
	}
}
void Parser::stmts() {
	if (lookahead->tag == '}')
		return;
	else {

		stmt();
		stmts();
	}
}
void Parser::stmt() {
	if (lookahead->tag == Tag::ID) {
		factor();
		match(';');
	}
	else{
		block();
	}

}
void Parser::factor() {
	const Token* tok = lookahead;
	match(Tag::ID);
	if (top->get(tok) == nullptr)
		error("error:undefine");
	else {
		Id* id = top->get(tok);
		cout << tok->toString() << ":" << id->type->toString() << "; ";
	}
}
Type* Parser::dims(Type* p) {
	match('[');
	const Number* num = (Number*)lookahead;
	match(Tag::NUM);
	match(']');
	p = new Array(num->value,p);
	if (lookahead->tag == '[')
		p = dims(p);
	//cout << p->toString() << endl;
	return p;
	//return new Array(num->value, p);
}