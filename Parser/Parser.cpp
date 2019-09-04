#include "Parser.h"
#include <iostream>
#include <string.h>

using namespace std;

//Default Constructor, should not be used
Parser::Parser()
{
	parseTree = nullptr;
	cout << "No file specified to the Lexer." << endl;
}

Parser::Parser(string filename)
{
	this->filename = filename;
	file.open(filename);
	parseTree = parse();
	file.close();
}
//Parses list of tokens into a ParseTree
ParseTree* Parser::parse()
{
	//libarary
	ParseTree* tree = new ParseTree(filename);
	while (file.peek()) {
		ParseTree* function = parseFunction();
		if (function == nullptr) {
			cout << "Invalid Function" << endl;
			return nullptr;
		}
		tree->addNode(function);
	}
	return tree;
}
//Parses the next function in the file
//NEED TO IMPLEMENT PARAMETERS TO FUNCTIONS
//if nullptr is returned then an error occured
ParseTree* Parser::parseFunction()
{
	ParseTree* function = new ParseTree("function");
	string type, identifier;

	//get function return type
	getline(file, type);
	if (isValidType(type));
	else {
		cout << "Return type not valid" << endl;
		return nullptr;
	}
	function->addNode(new ParseTree(type));

	//get function identifier
	if (getline(file, identifier));
	else {
		cout << "Expected function identifier, reached EOF" << endl;
		return nullptr;
	}
	function->addNode(new ParseTree(identifier));

	//get function parameters
	//(), (int c), (int s, int d, ...)
	ParseTree* parameters = parseParameters();
	if (parameters == nullptr) {
		cout << "Invalid Parameters" << endl;
		return nullptr;
	}

	//get function statement
	ParseTree* statement = new ParseTree("Statement");
	if (file.peek()) {
		ParseTree* _statement = parseStatement();//possible problem code
		if (_statement == nullptr) {
			cout << "Invalid Statement" << endl;
			return nullptr;
		}
		statement->addNode(_statement);
	}
	else {
		cout << "Expected statement, reached EOF" << endl;
		return nullptr;
	}
	function->addNode(statement);
	return nullptr;//replace later
}
//Parses the next set of parameters in the file
//if nullptr is returned then an error occurred
ParseTree* Parser::parseParameters() {
	ParseTree* parameters = new ParseTree("Parameter(s)");
	string line;
	if (getline(file, line) && line == "(") {
		if (!getline(file, line)) {
			cout << "Prematurely reached EOF" << endl;
			return nullptr;
		}
		while (line != ")") {
			parameters->addNode(parseParameter());
			//check if parameter is closed off properly
			if (!file.peek()) {
				cout << "Expected to reach ), reached EOF" << endl;
				return nullptr;
			}
		}
	}
	else {
		cout << "Expected (, reached EOF or ( is missing" << endl;
		return nullptr;
	}
	return nullptr;
}
//Parses the next parameter in the file
//if nullptr is returned then an error occured
ParseTree* Parser::parseParameter() {
	ParseTree* parameter = new ParseTree("Parameter");
	string type, identifier;

	//get parameter type
	if (!getline(file, type)) {
		cout << "Expected type for parameter, reached EOF" << endl;
		return nullptr;
 	}
	parameter->addNode(new ParseTree(type));

	//get parameter identifier
	if (!getline(file, type)) {
		cout << "Expected variable identifier, reached EOF" << endl;
		return nullptr;
	}
	parameter->addNode(new ParseTree(identifier));
	return nullptr;
}

//Parses the next statement in the file
//if nullptr is returned then an error occured
ParseTree* Parser::parseStatement()
{
	return nullptr;
}
//Determines if a given type is a valid type for the language
bool isValidType(string type) {
	if (type == "int")
		return true;
	else if (type == "boolean")
		return true;
	else if (type == "double")
		return true;
	else if (type == "char")
		return true;
	return false;
}

