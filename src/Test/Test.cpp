#include "test.h"
#include <fstream>
#include <iostream>
using std::cout;
using std::endl;

void testScan(){
    std::ifstream input("test.txt");
    Lexer lexer(input);
    while(!lexer.isEOF()){
        Token& tok = lexer.scan();
        cout << tok.tag << "    ";
        cout << tok.toString() << endl;
    }
}

void testSymbol(){
    
}