#include "test.h"
#include <fstream>
#include <iostream>
using std::cout;
using std::endl;
using namespace std;

void Test::testScan(){
    std::ifstream  input;
    //input.open("testScan.txt");
    input.open("D:\\junior\\compiler\\ToyC\\src\\Test\\test01.txt");
    Lexer* lex = new Lexer(input);
    while (!lex->isEOF()) {
        const Token* tok = lex->scan();
        cout << tok->tag << "\t";
        cout << tok->toString() << endl;
    }


}

void Test::testSymbol(){
    
}