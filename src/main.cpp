#include "Test/Test.h"
#include <iostream>
#include <string>
using std::string;
using std::cin;
using std::cout;
using std::endl;


int main() {
    try{
        Test::testControls();
    }
    catch(const string msg){
        cout << msg << endl;
    }
    system("pause");
    return 0;
}