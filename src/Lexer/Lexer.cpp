#include "Lexer.h"
#include <utility>
#include <ctype.h>
/* Token类的实现 */
Token::Token(int t): tag(t){} 
Token::~Token(){}

/* Charactor类的实现 */
Charactor::Charactor(char s, int t) : Token(t), value(s) {}
Charactor::~Charactor() {}
string Charactor::toString() const {
    return std::to_string(value);
}

/* Word类的实现 */
Word::Word(string s, int t): Token(t), value(s){}
Word::~Word(){}
Word:: and = Word(string("&&"), Tag::AND);
Word:: or = Word(string("||", Tag::OR));
Word:: eq = Word(string("==", Tag::EQ));
Word:: ne = Word(string("!=", Tag::NE));
Word:: le = Word(string("<=", Tag::LE));
Word:: ge = Word(string(">=", Tag::GE));
Word:: minus = Word(string("-", Tag::MINUS));
Word:: True = Word(string("true", Tag::TRUE));
Word:: False = Word(string("false", Tag::FALSE));
Word:: temp = Word(string("t", Tag::TEMP));
string Word::toString() const{
    return value;
}

/* Number类的实现 */
Number::Number(int s, int t) : Token(t), value(s) {}
Number::~Number() {}
string Number::toString() const {
    return std::to_string(value);
}

/* Real类的实现 */
Real::Real(float s, int t) : Token(t), value(s) {}
Real::~Real() {}
string Real::toString() const {
    return std::to_string(value);
}

/* Type类的实现 */
Type::Type(string s, int t,int w) : Word(s,t), width(w) {}
Type::~Type() {}
Type::Int = Type(string("int"), Tag::BASIC, 4);
Type::Float = Type(string("float"), Tag::BASIC, 4);
Type::Bool = Type(string("bool"), Tag::BASIC, 1);
Type::Char = Type(string("char"), Tag::BASIC, 1);
//函数：判断是否是基本型
bool Type::isBool(Type t) const{
    if (t == Type::Int || t == Type::Float || t == Type::Bool || t == Type::Char)
        return true;
    else
        return false;
}
//函数：判断转换成的类型
Type Type::max(Type ta, Type tb) const {
    if (!isBool(ta) || !isBool(tb))
        return null;
    else if (ta == Type::Float || tb == Type::Float)
        return Type::Float;
    else if (ta == Type::Int || tb == Type::Int)
        return Type::Int;
    else
        return Type::Char;
}

/* Array类的实现 */
Array::Array(int se,Type p) : Type(string(""), Tag::INDEX, sz*p.width), size(sz),type(p) {}
Array::~Array() {}
string Array::toString() const {
    return (string"["+std::to_string(size)+string"]"+type.tostring());
}

/* Lexer类的实现 */
Lexer::Lexer(istream& in) {
    reserve(Word::and);
    reserve(Word::or);
    reserve(Word::eq);
    reserve(Word::ne);
    reserve(Word::le);
    reserve(Word::ge);
    reserve(Word::minus);
    reserve(Word::True);
    reserve(Word::False);
    reserve(Word::temp);
    reserve(Type::Int);
    reserve(Type::Float);
    reserve(Type::Bool);
    reserve(Type::Char);
    input = in;
}
Lexer::~Lexer(){}
char Lexer::readch() {
    cache = input.get();
}
bool Lexer::readch(char c) {
    readch();
    if (cache != c)
        return false;
    cache = ' ';
    return true;
}
bool Lexer::isEOF() {
    if (input.peek() == EOF)
        return true;
    else
        return false;
}
Token* Lexer::scan() {
    for (;; readch) {
        if (cache == ' ' || cache == '\t')
            continue;
        else if (cache == '\n')
            line = line + 1;
        else
            break;
    }
    switch (cache) {
    case '&':
        if (readch('&'))
            return Word::and;
        else {
            Token* t = new Token('&');
            return t;
        }
    case '|':
        if (readch('|'))
            return Word:: or ;
        else {
            Token* t = new Token('|');
            return t;
        }
    case '=':
        if (readch('='))
            return Word::eq;
        else {
            Token* t = new Token('=');
            return t;
        }
    case '!':
        if (readch('='))
            return Word::ne;
        else {
            Token* t = new Token('!');
            return t;
        }
    case '<':
        if (readch('='))
            return Word::le;
        else {
            Token* t = new Token('<');
            return t;
        }
    case '>':
        if (readch('='))
            return Word::ge;
        else {
            Token* t = new Token('>');
            return t;
        }
    }
    if (std::isdigit(cache)) {
        Number* v(0);
        do {
            v->value = 10 * v->value + (int)cache;
            readch();
        } while (std::isdigit(cache));
        if (cache != '.')
            return v;
        else {
            Real* x(v->value);
            float d = 10;
            for (;;) {
                readch();
                if (!std::isdigit(cache))
                    break;
                x->value = x->value + (int)cache / d;
                d = d * 1 0;
            }
            return x;
        }
    }
    if (std::isalpha(cache)) {
        string b;
        do {
            b += cache;
            readch();
        } while (std::isalpha(cache) || std::isdigit(cache));
        Word w = (Word)words.get(d);
        if (w != null)
            return w;
        Word* nw = Word(b, Tag::ID);
        words.insert(pair<string, Word>(nw.value, nw));
        return nw;
    }
}
