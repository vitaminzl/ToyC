#include "Test/Test.h"
#include <iostream>
#include <fstream>
#include <string>
using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::ifstream;


int main() {
    try{
        ifstream input("testFinal.txt");
        // ofstream output("testFinal-intercode.txt");
        Lexer lexer(input);
        Parser parser(lexer, cout);
        parser.program();
    }
    catch(const string msg){
        cout << msg << endl;
    }
    system("pause");
    return 0;
}