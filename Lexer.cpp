#include "Lexer.h"
#include <iostream>
using namespace std;

//Tokens List
string reservedWord[] = {"int", "print"};
char reservedChar[] = {'(', ')', '{', '}', '\"', ';' };

string vectorToString(vector<char>);
bool isReservedChar(char);

//Default Constructor, should not be used
Lexer::Lexer()
{
	cout << "No file specified to the Lexer." << endl;
}

//Overloaded Constructor takes filename to work with specified file 
Lexer::Lexer(string filename)
{
	file.open(filename);
}

//Reads the contents of a file and compiles a vector of tokens
vector<string> Lexer::tokenize()
{
	vector<string> tokens;
	vector<char> token;
	char temp;
	//parse file by char
	while (file.get(temp)) {
		//decide how to treat each new char in file
		switch (temp) {
		case ' ' :
			if (!token.empty()) {
				tokens.push_back(vectorToString(token));
				token.clear();
			}
			continue;
		case '\t' :
			if (!token.empty()) {
				tokens.push_back(vectorToString(token));
				token.clear();
			}
			continue;
		case '\n' :
			if (!token.empty()) {
				tokens.push_back(vectorToString(token));
				token.clear();
			}
			continue;
		default :
			if (isReservedChar(temp)) {
				//current char is reserved
				if (!token.empty()) {
					tokens.push_back(vectorToString(token));
					token.clear();
				}
				tokens.push_back({temp});
			}
			else {
				//current char is valid
				goto letter;
			}
			continue;
		}
		letter:
		token.push_back(temp);
	}
	file.close();
	return tokens;
}

//Converts a vector of chars into a string
string vectorToString(vector<char> chars) 
{
	int size = chars.size();
	char* arr = new char[size + 1];
	copy(chars.begin(), chars.end(), arr);
	arr[size] = '\0';
	return arr;
}

//Determines is char c is in the reservedChar array
bool isReservedChar(char c) {
	for (int i = 0; i < sizeof(reservedChar) / sizeof(reservedChar[0]); i++) {
		if (c == reservedChar[i]) {
			return true;
		}
	}
	return false;
}
