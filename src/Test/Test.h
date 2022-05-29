#ifndef TEST
#define TEST
#include "../Inter/Inter.h"
#include "../Lexer/Lexer.h"
#include "../Parser/Parser.h"

/* 开发阶段测试函数，请勿使用在最终成品上 */
class Test{
public:
    static void testScan();
    static void testSymbol();
    static void testExpr();
    static void testControls();
};


#endif