#include "Lexer.h"
#include <utility>
#include <ctype.h>
using namespace std;
/* Token���ʵ�� */
Token::Token(int t) : tag(t) {}
Token::~Token() {}

/* Charactor���ʵ�� */
Charactor::Charactor(char s, int t) : Token(t), value(s) {}
Charactor::~Charactor() {}
const Charactor Charactor::AndS = Charactor('&', '&');
const Charactor Charactor::OrS = Charactor('|', '|');
const Charactor Charactor::Eq = Charactor('=', '=');
const Charactor Charactor::Not = Charactor('!', '!');
const Charactor Charactor::Lt = Charactor('<', '<');
const Charactor Charactor::Gt = Charactor('>', '>');
const Charactor Charactor::Opcurly = Charactor('{', '{');
const Charactor Charactor::Clcurly = Charactor('}', '}');
const Charactor Charactor::Semicolon = Charactor(';', ';');
const Charactor Charactor::Opbracket = Charactor('[', '[');
const Charactor Charactor::Clbracket = Charactor(']', ']');
const Charactor Charactor::Opparenthese = Charactor('(', '(');
const Charactor Charactor::Clparenthese = Charactor(')', ')');
const Charactor Charactor::Add = Charactor('+', '+');
const Charactor Charactor::Sub = Charactor('-', '-');
const Charactor Charactor::Mul = Charactor('*', '*');
const Charactor Charactor::Div = Charactor('/', '/');
string Charactor::toString() const {
    return (string("") + value);
}

/* Word���ʵ�� */
Word::Word(string s, int t) : Token(t), value(s) {}
Word::~Word() {}
const Word Word::And = Word(string("&&"), Tag::AND);
const Word Word::Or = Word(string("||"), Tag::OR);
const Word Word::eq = Word(string("=="), Tag::EQ);
const Word Word::ne = Word(string("!="), Tag::NE);
const Word Word::le = Word(string("<="), Tag::LE);
const Word Word::ge = Word(string(">="), Tag::GE);
const Word Word::minus = Word(string("-"), Tag::MINUS);
const Word Word::True = Word(string("true"), Tag::TRUE);
const Word Word::False = Word(string("false"), Tag::FALSE);
const Word Word::temp = Word(string("t"), Tag::TEMP);
const Word Word::err = Word(string("error"), Tag::ERR);
string Word::toString() const {
    return value;
}

/* Number���ʵ�� */
Number::Number(int s = 0, int t = Tag::NUM) : Token(t), value(s) {}
Number::~Number() {}
string Number::toString() const {
    return std::to_string(value);
}

/* Real���ʵ�� */
Real::Real(float s, int t) : Token(t), value(s) {}
Real::~Real() {}
string Real::toString() const {
    return std::to_string(value);
}

/* Type���ʵ�� */
Type::Type(string s, int t, int w) : Word(s, t), width(w) {}
Type::~Type() {}
const Type Type::Int = Type(string("int"), Tag::BASIC, 4);
const Type Type::Float = Type(string("float"), Tag::BASIC, 4);
const Type Type::Bool = Type(string("bool"), Tag::BASIC, 1);
const Type Type::Char = Type(string("char"), Tag::BASIC, 1);
//�������ж��Ƿ��ǻ�����
bool Type::isBool(Type* t) const {
    if (t == &Type::Int || t == &Type::Float || t == &Type::Char)
        return false;
    else
        return true;
}
//�������ж�ת���ɵ�����
const Type* Type::max(Type* ta, Type* tb) const {
    if (!isBool(ta) || !isBool(tb))
        return nullptr;
    else if (ta == &Type::Float || tb == &Type::Float)
        return &Type::Float;
    else if (ta == &Type::Int || tb == &Type::Int)
        return &Type::Int;
    else
        return &Type::Char;
}
string Type::toString() const {
    return value;
}

/* Array���ʵ�� */
Array::Array(int sz, Type* tp) : Type(string(""), Tag::INDEX, sz* tp->width), size(sz), p(tp) {}
Array::~Array() {}
string Array::toString() const {
    Type* tp = p;
    if (tp->tag != Tag::BASIC)
        tp = ((Array* )tp)->p;
    //cout << size << "+" << tp->width << endl;
    return (string("[") + std::to_string(width/tp->width) + string("]") + tp->toString());
}

/* Lexer���ʵ�� */
Lexer::Lexer(istream& in) :input(in) {
    reserve(new Word("if", Tag::IF));
    reserve(new Word("else", Tag::ELSE));
    reserve(new Word("while", Tag::WHILE));
    reserve(new Word("do", Tag::DO));
    reserve(new Word("break", Tag::BREAK));
    reserve(&Word::True);
    reserve(&Word::False);
    reserve(&Type::Int);
    reserve(&Type::Char);
    reserve(&Type::Bool);
    reserve(&Type::Float);
}
Lexer::~Lexer() {}
char* Lexer::readch() {
    cache = input.get();
    //cout << "cache:" << cache << endl;
    return &cache;
}
bool Lexer::readch(char c) {
    readch();
    if (cache != c)
        return false;
    cache = ' ';
    return true;
}
bool Lexer::isEOF() {
    if (input.eof())
        return true;
    else
        return false;
}
const Token* Lexer::scan() {
    for (;; readch()) {
        if (cache == ' ' || cache == '\t')
            continue;
        else if (cache == '\n')
            line = line + 1;
        else
            break;
    }
    //cout << cache << endl;
    switch (cache) {
    case '&':
        if (readch('&'))
            return &Word::And;
        else {
            return &Charactor::AndS;
        }
    case '|':
        if (readch('|'))
            return &Word::Or;
        else {
            return &Charactor::OrS;
        }
    case '=':
        if (readch('='))
            return &Word::eq;
        else {
            return &Charactor::Eq;
        }
    case '!':
        if (readch('='))
            return &Word::ne;
        else {
            return &Charactor::Not;
        }
    case '<':
        if (readch('='))
            return &Word::le;
        else {
            return &Charactor::Lt;
        }
    case '>':
        if (readch('='))
            return &Word::ge;
        else {
            return &Charactor::Gt;
        }
    case '{':
        readch();
        return &Charactor::Opcurly;
    case '}':
        readch();
        return &Charactor::Clcurly;
    case '[':
        readch();
        return &Charactor::Opbracket;
    case ']':
        readch();
        return &Charactor::Clbracket;
    case '(':
        readch();
        return &Charactor::Opparenthese;
    case ')':
        readch();
        return &Charactor::Clparenthese;
    case ';':
        readch();
        return &Charactor::Semicolon;
    case '+':
        readch();
        return &Charactor::Add;
    case '-':
        readch();
        return &Charactor::Sub;
    case '*':
        readch();
        return &Charactor::Mul;
    case '/':
        readch();
        return &Charactor::Div;
    }
    if (std::isdigit(cache)) {
        int v = 0;
        do {
            v = 10 * v + (int)cache - 48;
            readch();
        } while (std::isdigit(cache));
        Number* num = new Number(v, Tag::NUM);
        if (cache != '.')
            return num;
        else {
            float x = v;
            float d = 10;
            for (;;) {
                readch();
                if (!std::isdigit(cache))
                    break;
                x = x + ((int)cache - 48) / d;
                d = d * 10;
            }
            Real* re = new Real(x, Tag::REAL);
            return re;
        }
    }
    if (std::isalpha(cache)) {
        string b;
        do {
            b += cache;
            readch();
        } while (std::isalpha(cache) || std::isdigit(cache));
        if (words.count(b)) {
            const Word* w = words[b];
            return w;
        }
        else {
            const Word* nw = new Word(b, Tag::ID);
            words.insert(std::pair<string, const Word*>(nw->value, nw));
            return nw;
        }
    }
    readch();
    return &Word::err;
}
