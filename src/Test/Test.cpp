#include "Test.h"
#include <fstream>
#include <iostream>
using namespace std;



/* 开发阶段模块3测试函数，请勿使用在最终成品上 */
void Test::testExpr() {
    ifstream input("testExpr.txt");
    ofstream output("testExpr-intercode.txt");
    Lexer lexer(input);
    Parser parser(lexer, output);
    parser.program();
}

/* 开发阶段模块4测试函数，请勿使用在最终成品上 */
void Test::testControls() {
    ifstream input("testControls.txt");
    ofstream output("testControls-intercode.txt");
    Lexer lexer(input);
    Parser parser(lexer, output);
    parser.program();
}