#include "Parser.h"
#include <iostream>

using namespace std;

//Default Constructor, should not be used
Parser::Parser()
{
	cout << "No file specified to the Lexer." << endl;
}

Parser::Parser(string filename)
{
	file.open(filename);
	ParseTree bin("Binary Operation");
	string temp;
	while (getline(file,temp)) {
		ParseTree* ptr = new ParseTree(temp);
		bin.addNode(ptr);
	}
	bin.printTree();
	file.close();
}
//Parses list of tokens into a ParseTree
ParseTree* Parser::parse()
{
	return nullptr;
}

