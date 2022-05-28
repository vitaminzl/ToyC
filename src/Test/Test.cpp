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