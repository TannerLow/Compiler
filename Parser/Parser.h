#pragma once
#include <string>
//#include <fstream>
#include "ParseTree.h"
#include <vector>

class Parser
{
public:
	Parser();
	Parser(std::vector<std::string> tokens, std::string filename);
	ParseTree* parse();
private:
	ParseTree* parseFunction();
	ParseTree* parseParameters();
	ParseTree* parseParameter();
	ParseTree* parseStatement();
	ParseTree* parseExpression();

	//Statement Paths
	ParseTree* parseCompound();
	ParseTree* parseIf();
	ParseTree* parseWhile();
	ParseTree* parseFor();
	ParseTree* parseReturn();

	//Variables
	std::string filename;
	ParseTree* parseTree;
	std::vector<std::string> tokens;
	std::vector<std::string>::iterator it;
	std::string precedingType;
};