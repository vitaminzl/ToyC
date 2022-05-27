#ifndef LEXER
#define LEXER
#include <iostream>
#include <string>
#include <map>
using std::istream;
using std::string;
using std::map;

/* 多字符保留字、标识符Tag编号 */
enum Tag{
    IF = 256,   ELSE,       DO,         WHILE,  
    BREAK,      TRUE,       FALSE,      AND,
    OR,         NE,         EQ,         LE,
    GE,         BASIC,      NUM,        REAL,
    ID,         TEMP,       INDEX,      MINUS,
    ERR
};

/* Token基类 */
class Token{
public:
    const int tag;                      // Token的编号
    Token(int t);                       // 传入字符的编码进行初始化
    ~Token();                           // 空白析构函数
    virtual string toString() const = 0;// 将token转化为String类型
};


/* 单字符Token类 */
class Charactor: public Token {
public:
    static const Charactor AndS;        //与
    static const Charactor OrS;         //或
    static const Charactor Eq;          //等于号
    static const Charactor Not;         //不等于号
    static const Charactor Lt;          //小于号
    static const Charactor Gt;          //大于号
    static const Charactor Opcurly;     //左大括号
    static const Charactor Clcurly;     //右大括号
    static const Charactor Opbracket;   //左中括号
    static const Charactor Clbracket;   //右中括号
    static const Charactor Opparenthese;//左小括号
    static const Charactor Clparenthese;//右小括号
    static const Charactor Semicolon;   //分号
    static const Charactor Add;         //加号
    static const Charactor Sub;         //减号
    static const Charactor Mul;         //乘号
    static const Charactor Div;         //除号
    const char value;                   // 单字符token的属性值
    Charactor(char c, int tag);         // 传入字符以及它的ASCII编码作为token编码进行初始化
    ~Charactor();                       // 空白析构函数
    virtual string toString() const;    // 将token转化为String类型
};


/* 多字符保留字以及标识符类 */
class Word: public Token{
public:
    static const Word And;              //与
    static const Word Or;               //或
    static const Word eq;               //等于
    static const Word ne;               //不等于
    static const Word le;               //小于等于
    static const Word ge;               //大于等于
    static const Word minus;            //负
    static const Word True;             //真
    static const Word False;            //假
    static const Word temp;             //标志
    static const Word err;              //报错
    const string value;                 // 保留字及标识符的属性值
    Word(string s, int tag);            // 传入字符的属性以及编码
    ~Word();                            // 空白析构函数
    virtual string toString() const;    // 将token转化为String类型
};

/* 整数类 */
class Number: public Token{
public:
    const int value;                    // 整型的属性值
    Number(int s, int tag);            // 传入字符的属性以及编码
    ~Number();                         // 空白析构函数
    virtual string toString() const;    // 将token转化为String类型    
};

/* 实数类 */
class Real: public Token{
public:
    const float value;                  // 浮点型的属性值
    Real(float s, int tag);            // 传入字符的属性以及编码
    ~Real();                           // 空白析构函数
    virtual string toString() const;    // 将token转化为String类型
};

class Type: public Word{
public:
    const int width;                    // 变量所占字节数
    static const Type Int;              // 整数类型
    static const Type Float;            // 浮点数类型
    static const Type Bool;             // 布尔类型
    static const Type Char;             // 字符类型
    Type(string s, int tag, int width); // 传入字符的属性、编码以及所占空间大小
    ~Type();                            // 空白析构函数
    bool isBool(Type* t) const;          //判断是否是基本型（basic）
    const Type* max(Type* ta, Type* tb) const;   //判断类型转换函数
    virtual string toString() const;     // 将token转化为String类型
};

class Array: public Type{
public:
    const Type* next;
    int size = 1;
    Array(int sz, const Type* p);
    ~Array();
    virtual string toString() const;
};

class Lexer{
private:
    //记录当前行号
    int line = 1;
    //缓冲区
    char cache = ' ';
    //字符表
    map<string, const Word*> words;
    istream &input;
    void reserve(const Word* w) {
        words.insert(std::pair<string, const Word*>(w->value, w));
    }
public:
    Lexer(istream& in);
    ~Lexer();
    char* readch();
    bool readch(char c);
    bool isEOF();
    int getLine(){return line;}
    const Token* scan();
};


#endif