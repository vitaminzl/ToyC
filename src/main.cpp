#include <iostream>
#include <fstream>
#include <string>
#include <vector>
//#include "Lexer/Lexer.h"
#include "Test.h"
using namespace std;

//��ȡtxt�ļ�
std::vector<std::string> read_txt(const std::string& path)
{
	std::ifstream ifile(path);//��ȡ�ļ�
	std::streampos len = ifile.tellg();//��ȡ�ļ�����
	std::vector<std::string> data(len);//�����ļ�
	std::string str1; //temp
	if (ifile) //�ļ����Ƿ�ɹ�
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
//��ʾ����
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
	auto data = read_txt(path);//��ȡ����

	vector_print(data);//��ʾ����

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
