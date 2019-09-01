#pragma once
#include <string>
#include <fstream>
#include "ParseTree.h"

class Parser
{
	public:
		Parser();
		Parser(std::string filename);
		ParseTree* parse();
	private:
		std::ifstream file;
};

