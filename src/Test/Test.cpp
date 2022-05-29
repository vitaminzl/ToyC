#include "Test.h"
#include <fstream>
#include <iostream>
using namespace std;


void Test::testExpr() {
    ifstream input("testExpr.txt");
    ofstream output("testExpr-intercode.txt");
    Lexer lexer(input);
    Parser parser(lexer, output);
    parser.program();
}

void Test::testControls() {
    ifstream input("testControls.txt");
    ofstream output("testControls-intercode.txt");
    Lexer lexer(input);
    Parser parser(lexer, cout);
    parser.program();
}