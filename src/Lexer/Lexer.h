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
    GE,         INT,        FLOAT,      BOOL,
    CHAR
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
    const char value;                   // 单字符token的属性值
    Charactor(char c, int tag);         // 传入字符以及它的ASCII编码作为token编码进行初始化
    ~Charactor();                       // 空白析构函数
    virtual string toString() const;    // 将token转化为String类型
};


/* 多字符保留字以及标识符类 */
class Word: public Token{
public:
    const string value;                 // 保留字及标识符的属性值
    Word(string s, int tag);            // 传入字符的属性以及编码
    ~Word();                            // 空白析构函数
    virtual string toString() const;    // 将token转化为String类型
};

/* 整数类 */
class Number: public Token{
public:
    const int value;                    // 整型的属性值
    Number(string s, int tag);            // 传入字符的属性以及编码
    ~Number();                         // 空白析构函数
    virtual string toString() const;    // 将token转化为String类型    
};

/* 实数类 */
class Real: public Token{
public:
    const float value;                  // 浮点型的属性值
    Real(string s, int tag);            // 传入字符的属性以及编码
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
};

class Array: public Type{
public:
    Type type;
    int size;
    Array();
    ~Array();
    virtual string toString();
};

class Lexer{
private:
    int line;
    char cache;
    istream& input;
public:
    Lexer(istream& input);
    ~Lexer();
    char readch();
    bool readch(char c);
    bool isEOF();
    Token& scan();
};


#endif