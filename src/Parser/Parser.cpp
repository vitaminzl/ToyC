#include "Parser.h"
#include "../Inter/Inter.h"
#include <iostream>
using namespace std;
/*Parser类的定义*/
Parser::Parser(Lexer* l):lex(l) {}
Parser::~Parser(){}
void Parser::move(){
	lookahead = lex->scan();
}
void Parser::error(string s){
	throw (string("near line") + to_string(lex->getline()) + ":" + s);
}
void Parser::match(int t){
	if (lookahead->tag == t)
		move();
	else
		error("error");
}
void Parser::program() {
	block();
}
void Parser::block() {
	//block->{ decls stmts }
	match('{');
	cout << "{";
	Scope* saved = top;
	top = new Scope(top);
	decls();
	stmts();
	match('}');
	cout << "}";
	top = saved;
	delete saved;
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
	if(lookahead->tag=='}')
		return;
	else {
		stmt();
		stmts();
	}
}
void Parser::stmt() {
	if (lookahead->tag == '}')
		block();
	else {
		factor();
		match(';');
	}

}
void Parser::factor() {
	match(Tag::ID);
	Id* id = top->get(lookahead);
	cout << lookahead->toString() << ":" << id->type->toString();
}
Type* Parser::dims(Type* p) {
	match('[');
	const Token* tok = lookahead;
	match(Tag::NUM);
	match(']');
	if (lookahead->tag == '[')
		p = dims(p);
	Number* num = new Number(0, tok->tag);
	return new Array(num->value, p);
}