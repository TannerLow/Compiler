#include "Parser.h"
#include <iostream>
#include <string.h>

using namespace std;
//prototypes
bool isValidType(string);

//Default Constructor, should not be used
Parser::Parser()
{
	parseTree = nullptr;
	cout << "No file specified to the Lexer." << endl;
}

Parser::Parser(vector<string> tokens, string filename)
{
	this->filename = filename;
	this->tokens = tokens;
	it = tokens.begin();
	parseTree = parse();
}
//Parses list of tokens into a ParseTree
ParseTree* Parser::parse()
{
	//libarary
	ParseTree* tree = new ParseTree(filename);
	while (it < tokens.end()) {
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
	type = *it++;
	//type correctness will be handled in the parseTree optimizer
	function->addNode(new ParseTree(type));

	//get function identifier
	if (it < tokens.end())
		identifier = *it++;
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
	if (it >= tokens.end()) {
		cout << "Expected statement, reach EOF" << endl;
		return nullptr;
	}
	ParseTree* statement = parseStatement();
	if (statement == nullptr) {
		cout << "Invalid statement for function" << endl;
		return nullptr;
	}
	function->addNode(statement);

	return nullptr;//replace later
}
//Parses the next set of parameters in the file
//if nullptr is returned then an error occurred
ParseTree* Parser::parseParameters() {
	precedingType = "";
	ParseTree* parameters = new ParseTree("Parameter(s)");
	string line;
	if (it < tokens.end())
		line = *it++;
	if (line == "(") {
		if (it >= tokens.end()) {
			cout << "Prematurely reached EOF" << endl;
			return nullptr;
		}
		line = *it;//No ++, just reads the line to see if it is ')'

		while (line != ")") {
			parameters->addNode(parseParameter());
			//check if parameter is closed off properly
			if (it >= tokens.end()) {
				cout << "Expected to reach ), reached EOF" << endl;
				return nullptr;
			}
			line = *it;
		}
		//check if no parameters were found and modify accordingly
		if (parameters->nodeCount() == 0) 
			parameters->setData("No Parameters");

		//return the constructed parameters tree
		it++;
		return parameters;
	}
	else {
		cout << "Expected (, reached EOF or ( is missing" << endl;
		return nullptr;
	}
	//Leaves 'it' on the token after )
}
//Parses the next parameter in the file
//if nullptr is returned then an error occured
ParseTree* Parser::parseParameter() {
	ParseTree* parameter = new ParseTree("parameter");
	string type, identifier;
	vector<string> parameterTokens;

	//get tokens of parameter
	while (it < tokens.end() && *it != "," && *it != ")") {
		parameterTokens.push_back(*it++);
	}
	//handle preceeding types and invalid parameters
	switch (parameterTokens.size()) {
	case 2: {
		precedingType = parameterTokens[0];
		type = precedingType;
		identifier = parameterTokens[1];
		break;
	}
	case 1: {
		if (precedingType != "")
			type = precedingType;
		else {
			cout << "Invalid parameter, missing type" << endl;
			return nullptr;
		}
		identifier = parameterTokens[0];
		break;
	}
	case 0:
		//error (,)
		cout << "Invalid parameter" << endl;
		return nullptr;
	default:
		//error more than 2 parameter tokens
		cout << "Invalid parameter, more than 2 tokens found" << endl;
		return nullptr;
	}
	//build parameter and return
	parameter->addNode(new ParseTree(type));
	parameter->addNode(new ParseTree(identifier));
	//skip ',' tokens
	if (*it == ",")
		it++;
	return parameter;
	//Leaves 'it' on the token after the parsed parameter
}

//Parses the next statement in the file
//if nullptr is returned then an error occured
ParseTree* Parser::parseStatement()
{
	string token;
	if (it < tokens.end()) 
		token = *it++;
	else {
		cout << "Missing statement" << endl;
		return nullptr;
	}

	//Determine the type of statement and act accordingly
	if (token == "{") {
		return parseCompound();
	}
	else if (token == "if") {

	}
	else if (token == "while") {

	}
	else if (token == "for") {

	}
	else if (token == "return") {

	}

	//determine is type or identifier
	return nullptr;
}

ParseTree* Parser::parseCompound() {
	ParseTree* compound = new ParseTree("{ ... }");
	while (*it != "}") {
		compound->addNode(parseStatement());//NEEDS to leave it on token after parsed Statement
		if (it >= tokens.end()) {
			cout << "Expected to reach }, reached EOF" << endl;
			return nullptr;
		}
	}
	return compound;
}

ParseTree* Parser::parseIf() {
	ParseTree* ifStatement = new ParseTree("if");
	//parse expression
	ParseTree* expression = parseExpression();
	if (expression == nullptr) {
		cout << "Invalid expression inside if statement" << endl;
		return nullptr;
	}
	ifStatement->addNode(parseExpression());
	//parse statement
	ParseTree* statement = parseStatement();
	if (expression == nullptr) {
		cout << "Invalid statement attached to if statement" << endl;
		return nullptr;
	}
	ifStatement->addNode(parseStatement());

	return ifStatement;
}
//Expressions
ParseTree* Parser::parseExpression() {
	//need to implement
	return nullptr;
}


