#include <iostream>
#include <fstream>
#include <string>
#include <vector>
//#include "Lexer/Lexer.h"
#include "Test.h"
using namespace std;

//读取txt文件
std::vector<std::string> read_txt(const std::string& path)
{
	std::ifstream ifile(path);//读取文件
	std::streampos len = ifile.tellg();//获取文件长度
	std::vector<std::string> data(len);//保存文件
	std::string str1; //temp
	if (ifile) //文件打开是否成功
	{
		std::cout << "test_case.txt open scessful" << std::endl;
		while (std::getline(ifile, str1)) {
			//std::cout << str1 << std::endl;
			data.push_back(str1);
		}
		ifile.close();
		return data;
	}
	else
	{
		std::cout << "test_case.txt doesn't exist" << std::endl;
		std::system("pause");
	}
}
//显示数据
void vector_print(const std::vector<std::string>& data)
{
	for (auto s : data)
	{
		std::cout << s << "\n";

	}
}
/*
int main()
{
	std::string path = "D:\\vs-reps\\ToyC_ZMJ\\test01.txt";
	auto data = read_txt(path);//读取数据

	vector_print(data);//显示数据

}
*/


int main() {
	Test t;
	string path = "testsymbol.txt";
	string path2 = "test02.txt";
	string path3 = "test01.txt";
	try {	
		t.testScan(path3);
		//t.testSymbol(path);
	}
	catch(string s) {
		cout << "ERROR:" << s << endl;
	}
	system("pause");
	return 0;
}
