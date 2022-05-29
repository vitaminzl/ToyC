#include "Parser.h"
#include <iostream>
#include <string>
using std::string;
using std::endl;
using std::to_string;

/* 初始化记得move一下 */
Parser::Parser(Lexer& l, ostream& o):lex(l), output(o){
    move();
    Node::setOutput(o);
}

void Parser::error(string s){ 
    throw string("near Line") + to_string(lex.getLine()) + string(": ") + s; 
}

void Parser::move(){
    lookahead = lex.scan();
}

void Parser::match(int t){
    if (t == lookahead->tag)
        move();
    else 
        error("Syntax Error");
}


void Parser::program(){
    Stmt* s = block();
    int begin = Node::newLabel();
    int after = Node::newLabel();
    s->printLabel(begin);
    s->gen(begin, after);
    s->printLabel(after);
    output << endl;
}

/*
    BLOCK -> { stmts }
*/
Stmt* Parser::block() {
	//block->{ decls stmts }
	match('{');
	Scope* saved = top;
	// cout << "{";
	top = new Scope(saved);
	decls();
    Stmt* s = stmts();
	match('}');
	// cout << "}";
	delete top;
	top = saved;
    return s;
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
        // cout << lookahead->toString() << p->toString() << p->tag << endl;
		const Token* tok = lookahead;
		Word* w = new Word(((Word*)lookahead)->value, lookahead->tag);
		match(Tag::ID);
		match(';');
		Id* id = new Id(w, p, used);
		top->put(tok, id);
        used += p->width;
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

Stmt* Parser::stmts(){
    if (lex.isEOF() || lookahead->tag == '}') 
        return &Stmt::Null;
    else 
        return new Seq(stmt(), stmts());
}

/*
    STMT    -> ASSIGN';'
            -> if ( BOOL ) STMT
            -> if ( BOOL ) STMT else STMT
            -> while ( BOOL ) STMT
            -> do STMT while ( BOOL )
            -> break';'
            -> BLOCK
*/
Stmt* Parser::stmt(){
    Stmt* savedStmt = nullptr;
    switch(lookahead->tag){
        case Tag::IF: {
            int line = lex.getLine();
            move();
            match('(');
            const Expr* e = bools();
            match(')');
            Stmt* s = stmt();
            if (lookahead->tag == Tag::ELSE){
                move();
                return new Else(e, s, stmt(), line);
            }
            else 
                return new If(e, s, line);
        }
        case Tag::WHILE: {
            int line = lex.getLine();
            move();
            While* wNode = new While();
            savedStmt = Stmt::Enclosure;
            Stmt::Enclosure = wNode;
            match('(');
            const Expr* e = bools();
            match(')');
            wNode->init(e, stmt(), line);
            Stmt::Enclosure = savedStmt;
            return wNode;
        }
        case Tag::DO: {
            move();
            Do* dNode = new Do();
            savedStmt = Stmt::Enclosure;
            Stmt::Enclosure = dNode;
            // match(Tag::DO);
            Stmt* s = stmt();
            int line = lex.getLine();
            match(Tag::WHILE);
            match('(');
            const Expr* e = bools();
            match(')');
            match(';');
            dNode->init(e, s, line);            
            Stmt::Enclosure = savedStmt;
            return dNode;
        }
        case Tag::BREAK: {
            int line = lex.getLine();
            move();
            match(';');
            return new Break(line);
        }
        case '{':
            return block();
        default: {
            return assign();
        }
    }
}


Stmt* Parser::assign(){
    const Id* id = top->get(lookahead);
    Stmt* equation = &Stmt::Null;
    match(Tag::ID);
    if (lookahead->tag == '['){
        const Expr* ix = offset(id);
        move();
        const Expr* expr = bools();
        equation = new SetElem(id, ix, expr, lex.getLine());
    }
    else if (lookahead->tag == '='){
        move();
        const Expr* expr = bools();
        equation = new Set(id, expr, lex.getLine());
    }
    match(';');
    return equation;
}


/*
    OFFSET -> [ BOOL ] OFFSET
           -> eps

*/
const Access* Parser::offset(const Id* id){
    /* 指向下一维度数组的width */
    // cout << id->toString() << id->type->toString()<< endl;
    if(id->type->tag != Tag::INDEX)
        error("Not an array type!");
    const Type* tp = ((Array* )id->type)->next; 
    match('[');
    const Expr* first = bools();
    match(']');
    // cout << lex.getLine() << endl;
    const Expr* loc = new Arith(&Charactor::Mul, first, new Constant(tp->width));
    while(lookahead->tag == '['){
        match('[');
        if (tp->tag == Tag::BASIC){
            error("Out of Array Dims;");
        }
        const Expr* next = bools();
        match(']');
        tp = ((Array*) tp)->next;
        const Expr* t = new Arith(&Charactor::Mul, next, new Constant(tp->width));
        loc = new Arith(&Charactor::Add, loc, t);     
    }
    return new Access(id, loc, &Type::Int);
}

/*  
    BOOLS -> BOOLS || JOIN
          -> JOIN
*/
const Expr* Parser::bools(){
    const Expr* first = join();
    while (lookahead->tag == Tag::OR){
        move();
        const Expr* second = join();
        first = new Or(first, second);
    }
    return first;
}

/*
    JOIN -> JOIN "&&" EQUALITY
         -> EQUALITY
*/
const Expr* Parser::join(){
    const Expr* first = equality();
    while (lookahead->tag == Tag::AND){
        move();
        const Expr* second = equality();
        first = new And(first, second);
    }
    return first;
}


/*
    EQUALITY -> EQUALITY == CMP
             -> EQUALITY != CMP
             -> CMP
*/
const Expr* Parser::equality(){
    const Expr* first = cmp();
    while (lookahead->tag == Tag::EQ || lookahead->tag == Tag::NE){
        const Token* oper = lookahead;
        move();
        const Expr* second = cmp();
        first = new Cmp(oper, first, second);
    }
    return first;
}

/*
    CMP -> EXPR < EXPR
        -> EXPR >= EXPR
        -> EXPR <= EXPR
        -> EXPR > EXPR
        -> EXPR
*/
const Expr* Parser::cmp(){
    const Expr* first = expr();
    const Token* oper = nullptr;
    const Expr* second = nullptr;
    switch(lookahead->tag){
        case '<':
        case '>':
        case Tag::LE:
        case Tag::GE:
            oper = lookahead; 
            move();
            second = expr();
            first = new Cmp(oper, first, second);
            break;
        default:
            break;
    }
    return first;
}


/* 
    EXPR -> EXPR + TERM
         -> EXPR - TERM
         -> TERM
*/
const Expr* Parser::expr(){
    const Expr* first = term();
    const Token* oper = lookahead;
    while (lookahead->tag == '+' || lookahead->tag == '-'){
        const Token* oper = lookahead;
        move();
        const Expr* second = term();
        first = new Arith(oper, first, second);
    }
    return first;
}


/* 
    TERM -> TERM * UNARY
         -> TERM / UNARY
         -> UNARY
*/
const Expr* Parser::term(){
    const Expr* first = unary();
    const Token* oper = lookahead;
    while (lookahead->tag == '*' || lookahead->tag == '/'){
        const Token* oper = lookahead;
        move();
        const Expr* second = unary();
        first = new Arith(oper, first, second);
    }
    return first;
}


/*
    UNARY -> ! UNARY
          -> - UNARY
          -> Factor
*/
const Expr* Parser::unary(){
    if (lookahead->tag == '!' ){
        move();
        return new Not(unary());
    }
    else if (lookahead->tag == '-'){
        const Token* oper = lookahead;
        move();
        return new Unary(oper, unary());        
    }
    return factor();
}

/*
    FACTOR -> ( BOOL )
           -> number
           -> real
           -> true
           -> false
           -> id OFFSET
*/
const Expr* Parser::factor(){
    switch (lookahead->tag) {
    case '(': {
        move();
        const Expr* b = bools(); 
        match(')');
        return b;
        }
    case Tag::ID: {
        const Id* id = top->get(lookahead);
        move();
        if (id == nullptr){
            error("Undeclared");
        }
        if(lookahead->tag == '[')
            return offset(id);
        else 
            return id;
        }
    case Tag::NUM:{
        const Token* num = lookahead;
        move();
        return new Constant(num, &Type::Int);
        }
    case Tag::REAL:{
        const Token* real = lookahead;
        move();
        return new Constant(real, &Type::Float);
        }
    case Tag::TRUE:
        move();
        return &Constant::True;
    case Tag::FALSE:
        move();
        return &Constant::False;
    default:
        error("Wrong Type");
    }
}




