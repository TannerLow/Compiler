#include <iostream>
#include <string.h>
#include <fstream>

using namespace std;


int main() {
	string s = "hi";
	char* c = new char[s.size()+1];
	s.copy(c, s.size() + 1);
	c[s.size()] = '\0';
	cout << c << " " << (c == string("hi")) << endl;

	string s1 = "int";
	cout << (s1 == "it") << endl;

	ifstream file;
	file.open("test.txt");
	cout << char(file.peek()) << endl;
	file.close();
}