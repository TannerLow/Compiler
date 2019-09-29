#include "Parser.h"
#include <iostream>
#include <string.h>

using namespace std;
//prototypes


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
	scrapeClassIdentifiers();
	it = tokens.begin();
	parseTree = parse();
}
//Create a list of class identifiers for later reference
void Parser::scrapeClassIdentifiers() {
	while (it + 1 < tokens.end()) {
		if (*it == "class") 
			classes.insert(*(++it));
	}
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
	if (it + 1 < tokens.end()) 
		token = *it++;
	else {
		cout << "Expected statement, reached EOF" << endl;
		return nullptr;
	}

	//Determine the type of statement and proceed accordingly
	if (token == "{") {
		return parseCompound();
	}
	else if (token == "if") {
		return parseIf();
	}
	else if (token == "while") {
		return parseWhile();
	}
	else if (token == "for") {
		return parseFor();
	}
	else if (token == "return") {
		return parseReturn();
	}
	else if (token == ";") {
		return new ParseTree("empty statement");
	}
	else {
		if (classes.find(token) != classes.end()) 
			if (*it != ";" && *it != ".")
				return parseDeclaration(token);
		return parseExpressionStatement();
	}
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
	//remove ( token
	if (it >= tokens.end()) {
		cout << "Expected (, reached EOF" << endl;
		return nullptr;
	}
	if (*it++ != "(") {
		cout << "Expected ( after 'if'" << endl;
		return nullptr;
	}
	//parse expression
	ParseTree* expression = parseExpression(")");
	if (expression == nullptr) {
		cout << "Invalid expression inside if statement" << endl;
		return nullptr;
	}
	ifStatement->addNode(expression);
	//remove ) token
	if (it >= tokens.end()) {
		cout << "Expected ), reached EOF" << endl;
		return nullptr;
	}
	if (*it++ != ")") {
		cout << "Expected ) to close if statement expression" << endl;
		return nullptr;
	}
	//parse statement
	ParseTree* statement = parseStatement();
	if (statement == nullptr) {
		cout << "Invalid statement attached to if statement" << endl;
		return nullptr;
	}
	ifStatement->addNode(statement);

	return ifStatement;
}

ParseTree* Parser::parseWhile() {
	ParseTree* whileLoop = new ParseTree("while");
	//remove ( token
	if (it >= tokens.end()) {
		cout << "Expected (, reached EOF" << endl;
		return nullptr;
	}
	if (*it++ != "(") {
		cout << "Expected ( after 'while'" << endl;
		return nullptr;
	}
	//parse expression
	ParseTree* expression = parseExpression(")");
	if (expression == nullptr) {
		cout << "Invalid expression inside while loop" << endl;
		return nullptr;
	}
	whileLoop->addNode(expression);
	//remove ) token
	if (it >= tokens.end()) {
		cout << "Expected ), reached EOF" << endl;
		return nullptr;
	}
	if (*it++ != ")") {
		cout << "Expected ) to close while loop expression" << endl;
		return nullptr;
	}
	//parse statement
	ParseTree* statement = parseStatement();
	if (statement == nullptr) {
		cout << "Invalid statement attached to while loop" << endl;
		return nullptr;
	}
	whileLoop->addNode(statement);
	return whileLoop;
}

ParseTree* Parser::parseFor() {
	ParseTree* forLoop = new ParseTree("for");
	//remove ( token
	if (it >= tokens.end()) {
		cout << "Expected (, reached EOF" << endl;
		return nullptr;
	}
	if (*it++ != "(") {
		cout << "Expected ( after 'for'" << endl;
		return nullptr;
	}
	//parse statement
	ParseTree* statement = parseStatement();
	if (statement == nullptr) {
		cout << "Invalid statement inside for loop" << endl;
		return nullptr;
	}
	forLoop->addNode(statement);
	//parse expression
	ParseTree* expression = parseExpression(";");
	if (expression == nullptr) {
		cout << "Invalid primary expression inside for loop" << endl;
		return nullptr;
	}
	forLoop->addNode(expression);
	//remove ; token
	if (++it >= tokens.end()) {
		cout << "Invalid primary expression in for loop" << endl;
		return nullptr;
	}
	//parse expression
	expression = parseExpression(")");
	if (expression == nullptr) {
		cout << "Invalid secondary expression inside for loop" << endl;
		return nullptr;
	}
	forLoop->addNode(expression);
	//remove ) token
	if (it >= tokens.end()) {
		cout << "Expected ), reached EOF" << endl;
		return nullptr;
	}
	if (*it++ != ")") {
		cout << "Expected ) to close for loop expressions" << endl;
		return nullptr;
	}
	//parse statement
	statement = parseStatement();
	if (statement == nullptr) {
		cout << "Invalid statement after for loop" << endl;
		return nullptr;
	}
	forLoop->addNode(statement);

	return forLoop;
}

ParseTree* Parser::parseReturn() {
	ParseTree* returnStatement = new ParseTree("return");
	//parse statement
	ParseTree* expression = parseExpression(";");
	if (expression == nullptr) {
		cout << "Invalid statement after for loop" << endl;
		return nullptr;
	}
	returnStatement->addNode(expression);

	return returnStatement;
}

ParseTree* Parser::parseDeclaration(string token) {
	ParseTree* declaration = new ParseTree(token);
	while (*it != ";") {
		//get identifier
		ParseTree* identifier = new ParseTree(*it++);
		declaration->addNode(identifier);
		//equals token
		if (it >= tokens.end() || *it != "=") {
			cout << "Invalid variable definition, missing = or EOF reached prematurely" << endl;
			return nullptr;
		}
		//get expression
		ParseTree* expression = parseExpression(",");
		if (expression == nullptr) {
			cout << "Invalid expression in variable definition" << endl;
			return nullptr;
		}
		//remove comma
		if (*it == ",")
			if (it + 1 < tokens.end())
				it++;
			else {
				cout << "Expected to reach ; in variable definition, reached EOF" << endl;
				return nullptr;
			}
	}
	return nullptr;
}

ParseTree* Parser::parseExpressionStatement() {
	return nullptr;
}
//Expressions
//stop if stopToken is reached or ;
ParseTree* Parser::parseExpression(string stopToken) {
	
	return nullptr;
}