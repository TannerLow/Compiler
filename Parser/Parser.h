#pragma once
#include <string>
#include "ParseTree.h"
#include <vector>
#include <set>

class Parser
{
public:
	Parser();
	Parser(std::vector<std::string> tokens, std::string filename);
	ParseTree* parse();
private:
	//Pre parse functions
	void scrapeClassIdentifiers();

	//Parse fucntions
	ParseTree* parseFunction();
	ParseTree* parseParameters();
	ParseTree* parseParameter();
	ParseTree* parseStatement();
	ParseTree* parseExpression(std::string stopToken);

	//Statement Paths
	ParseTree* parseCompound();
	ParseTree* parseIf();
	ParseTree* parseWhile();
	ParseTree* parseFor();
	ParseTree* parseReturn();
	ParseTree* parseDeclaration(std::string);
	ParseTree* parseExpressionStatement();

	//Variables
	std::string filename;
	ParseTree* parseTree;
	std::set<std::string> classes;
	std::vector<std::string> tokens;
	std::vector<std::string>::iterator it;
	std::string precedingType;
};