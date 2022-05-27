// PROGRAM -> STMTS
// STMTS	-> STMTS STMT
//     	-> eps
// STMT	-> ASSIGN';'
// ASSIGN  -> id OFFSET = BOOL
// BOOL	-> BOOL "||" JOIN
//         -> JOIN
// JOIN	-> JOIN "&&" EQAULITY
//         -> EQUALITY
// EQUALITY-> EQUALITY "==" CMP
//         -> EQUALITY "!=" CMP
//         -> CMP
// CMP	-> EXPR < EXPR
//         -> EXPR <= EXPR
//         -> EXPR >= EXPR
//         -> EXPR > EXPR
// EXPR	-> EXPR + TERM
//         -> EXPR - TERM
//         -> TERM
// TERM	-> TERM * UNARY
//         -> TERM / UNARY
//         -> UNARY
// UNARY	-> '!' UNARY
//         -> '-' UNARY
//         -> FACTOR
// FACTOR	-> ( BOOL )
//         -> LOC
//         -> number
//         -> real

#include "Parser.h"

/* 初始化记得move一下 */
Parser::Parser(Lexer& l):lex(l){
    move();
}

void Parser::error(string s){ 
    cerr << "near Line" << lex.getLine() << ": "<< s << endl; 
    exit(1);
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
    const Stmt* s = stmt();
    int begin = Node::newLabel();
    int after = Node::newLabel();
    s->gen(begin, after);
    s->printLabel(after);
}

const Stmt* Parser::stmts(){
    if (lex.isEOF()) 
        return &Stmt::Null;
    else 
        return new Seq(stmt(), stmts());
}

const Stmt* Parser::stmt(){
    return assign();
}


const Stmt* Parser::assign(){
    const Id* id = new Id((Word*)lookahead);
    const Stmt* equation = nullptr;
    match(Tag::ID);
    if (lookahead->tag == '['){
        const Expr* ix = offset(id);
        match('=');
        const Expr* expr = bools();
        equation = new SetElem(id, ix, expr);
    }
    else {
        match('=');
        const Expr* expr = bools();
        equation = new Set(id, expr);
    }
    match(';');
    return equation;
}


/*
    OFFSET -> [ BOOL ] OFFSET
           -> eps

*/
const Access* Parser::offset(const Id* id){
    int SIZE = 10; // 模块调试使用
    match('[');
    const Expr* first = bools();
    match(']');
    const Expr* loc = new Arith(&Charactor::Mul, first, new Constant(SIZE));
    while(lookahead->tag == '['){
        match('[');
        const Expr* next = bools();
        match(']');
        const Expr* t = new Arith(&Charactor::Mul, next, new Constant(SIZE));
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
    if (lookahead->tag == Tag::OR){
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
    if (lookahead->tag == Tag::AND){
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
    if (lookahead->tag == Tag::EQ || lookahead->tag == Tag::NE){
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
    switch(lookahead->tag){
        case '<':
        case '>':
        case Tag::LE:
        case Tag::GE:
            const Token* oper = lookahead; 
            move();
            const Expr* second = expr();
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
    if (lookahead->tag == '+' || lookahead->tag == '-'){
        const Token* oper = lookahead;
        move();
        const Expr* second = term();
        first = new Arith(oper, first, second);
    }
    return first;
}


/* 
    EXPR -> EXPR + UNARY
         -> EXPR - UNARY
         -> UNARY
*/
const Expr* Parser::term(){
    const Expr* first = unary();
    const Token* oper = lookahead;
    if (lookahead->tag == '*' || lookahead->tag == '/'){
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
          -> UNARY
*/
const Expr* Parser::term(){
    if (lookahead->tag == '!' || lookahead->tag == '-'){
        const Token* oper = lookahead;
        move();
        return new Unary(oper, factor());
    }
    else{
        return factor();
    }
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
    case '(':
        move();
        const Expr* b = bools(); 
        match(')');
        return b;
    case Tag::ID:
        const Id* id = new Id((Word*)lookahead);
        move();
        return offset(id);
    case Tag::NUM:
        move();
        return new Constant(lookahead, &Type::Int);
    case Tag::REAL:
        move();
        return new Constant(lookahead, &Type::Float);
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




