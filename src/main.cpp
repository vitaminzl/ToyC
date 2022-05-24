#include <iostream>
#include <string>
// #include "Lexer/Lexer.h"
using std::string;
using std::cin;
using std::cout;
using std::endl;

/* Token基类 */
class Token{
public:
    const int tag;                      // Token的编号
    Token(int t);                       // 传入字符的编码进行初始化
    ~Token();                           // 空白析构函数
    virtual string toString() const = 0;// 将token转化为String类型
};
/* 多字符保留字以及标识符类 */
class Word: public Token{
public:
    const string value;                 // 保留字及标识符的属性值
    int bbb = 1;
    static const Word a;
    Word(string s, int tag);            // 传入字符的属性以及编码
    ~Word();                            // 空白析构函数
    virtual string toString() const;    // 将token转化为String类型
};


/* Token类的实现 */
Token::Token(int t): tag(t){} 
Token::~Token(){}
/* Word类的实现 */
Word::Word(string s, int t): Token(t), value(s){ bbb+=3; }
Word::~Word(){}
string Word::toString() const{
    return value;
}
const Word Word::a = Word("1", 4);

int main() {
    Word word(string("abc"), 2);
    Token &tok = word;
    cout << ((Word&)word).value << endl;
    cout << word.bbb << endl;
    system("pause");
    return 0;
}