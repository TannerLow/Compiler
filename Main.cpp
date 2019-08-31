// Main.cpp : Driver Code
#include <iostream>
#include "Lexer.h"
#include <string>
using namespace std;

int main() {
	//Creates Lexer, tokenizes test.txt, and prints
	//the list of tokens it made
	Lexer l("test.txt");
	vector<string> tokens = l.tokenize();
	for (string token : tokens)
		cout << token << endl;

	getchar();
	return 0;
}