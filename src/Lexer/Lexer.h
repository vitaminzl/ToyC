#ifndef LEXER
#define LEXER
#include <iostream>
#include <string>
#include <map>
using std::istream;
using std::string;
using std::map;

/* ���ַ������֡���ʶ��Tag��� */
enum Tag {
    IF = 256, ELSE, DO, WHILE,
    BREAK, TRUE, FALSE, AND,
    OR, NE, EQ, LE,
    GE, BASIC, NUM, REAL,
    ID, TEMP, INDEX, MINUS,
    ERR, SCOPE
};

/* Token���� */
class Token {
public:
    const int tag;                      // Token�ı��
    Token(int t);                       // �����ַ��ı�����г�ʼ��
    ~Token();                           // �հ���������
    virtual string toString() const = 0;// ��tokenת��ΪString����
};


/* ���ַ�Token�� */
class Charactor : public Token {
public:
    static const Charactor AndS;        //��
    static const Charactor OrS;         //��
    static const Charactor Eq;          //���ں�
    static const Charactor Not;         //�����ں�
    static const Charactor Lt;          //С�ں�
    static const Charactor Gt;          //���ں�
    static const Charactor Opcurly;     //�������
    static const Charactor Clcurly;     //�Ҵ�����
    static const Charactor Opbracket;   //��������
    static const Charactor Clbracket;   //��������
    static const Charactor Opparenthese;//��С����
    static const Charactor Clparenthese;//��С����
    static const Charactor Semicolon;   //�ֺ�
    static const Charactor Add;         //�Ӻ�
    static const Charactor Sub;         //����
    static const Charactor Mul;         //�˺�
    static const Charactor Div;         //����
    const char value;                   // ���ַ�token������ֵ
    Charactor(char c, int tag);         // �����ַ��Լ�����ASCII������Ϊtoken������г�ʼ��
    ~Charactor();                       // �հ���������
    virtual string toString() const;    // ��tokenת��ΪString����
};


/* ���ַ��������Լ���ʶ���� */
class Word : public Token {
public:
    static const Word And;              //��
    static const Word Or;               //��
    static const Word eq;               //����
    static const Word ne;               //������
    static const Word le;               //С�ڵ���
    static const Word ge;               //���ڵ���
    static const Word minus;            //��
    static const Word True;             //��
    static const Word False;            //��
    static const Word temp;             //��־
    static const Word err;              //����
    const string value;                 // �����ּ���ʶ��������ֵ
    Word(string s, int tag);            // �����ַ��������Լ�����
    ~Word();                            // �հ���������
    virtual string toString() const;    // ��tokenת��ΪString����
};

/* ������ */
class Number : public Token {
public:
    const int value;                    // ���͵�����ֵ
    Number(int s, int tag);            // �����ַ��������Լ�����
    ~Number();                         // �հ���������
    virtual string toString() const;    // ��tokenת��ΪString����    
};

/* ʵ���� */
class Real : public Token {
public:
    const float value;                  // �����͵�����ֵ
    Real(float s, int tag);            // �����ַ��������Լ�����
    ~Real();                           // �հ���������
    virtual string toString() const;    // ��tokenת��ΪString����
};

/*������*/
class Type : public Word {
public:
    const int width;                    // ������ռ�ֽ���
    static const Type Int;              // ��������
    static const Type Float;            // ����������
    static const Type Bool;             // ��������
    static const Type Char;             // �ַ�����
    Type(string s, int tag, int width); // �����ַ������ԡ������Լ���ռ�ռ��С
    ~Type();                            // �հ���������
    bool isBool(Type* t) const;          //�ж��Ƿ��ǲ�����
    const Type* max(Type* ta, Type* tb) const;   //�ж�����ת������
    virtual string toString() const;     // ��tokenת��ΪString����
};

/*������*/
class Array : public Type {
public:
    Type* p;
    int size = 1;
    Array(int sz, Type* p);
    ~Array();
    virtual string toString() const;
};

/*�ʷ���������*/
class Lexer {
private:
    //��¼��ǰ�к�
    int line = 1;
    //������
    char cache = ' ';
    //�ַ���
    map<string, const Word*> words;
    istream& input;
    void reserve(const Word* w) {
        words.insert(std::pair<string, const Word*>(w->value, w));
    }
public:
    Lexer(istream& in);
    ~Lexer();
    char* readch();
    int getline() { return line; }
    bool readch(char c);
    bool isEOF();
    const Token* scan();
};


#endif