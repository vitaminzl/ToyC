#include "Test.h"
#include <fstream>
#include <iostream>
using namespace std;


void Test::testExpr() {
    ifstream input("D:\\ZJUT\\Compilers Principles\\Make a Simple Compiler\\code\\src\\Test\\testExpr.txt");
    Lexer lexer(input);
    Parser parser(lexer);
    parser.program();
}