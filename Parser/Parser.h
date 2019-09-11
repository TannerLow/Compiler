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
	//Variables
	////std::ifstream file;
	std::string filename;
	ParseTree* parseTree;

	std::vector<std::string> tokens;
	std::vector<std::string>::iterator it;
};