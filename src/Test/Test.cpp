#include "test.h"
#include <fstream>
#include <iostream>
using std::cout;
using std::endl;
using namespace std;

void Test::testScan(string path= "testsymbol.txt") {
    std::ifstream  input;
    //input.open("testScan.txt");
    //input.open("D:\\junior\\compiler\\ToyC\\src\\Test\\test01.txt");
    //input.open("test01.txt");
    input.open(path);
    if (!input.is_open())
        cout << "ERROR:CAN NOT OPEN FILE." << endl;
    Lexer* lex = new Lexer(input);
    while (!lex->isEOF()) {
        const Token* tok = lex->scan();
        cout << tok->tag << "\t";
        cout << tok->toString() << endl;
    }
}

void Test::testSymbol(string path= "testsymbol.txt") {
    std::ifstream  input;
    input.open(path);
    if(!input.is_open())
        cout << "ERROR:CAN NOT OPEN FILE." << endl;

    Lexer* lex = new Lexer(input);
    Parser* par = new Parser(lex);
    par->program();
}