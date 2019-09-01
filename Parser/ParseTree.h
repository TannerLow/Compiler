#pragma once
#include <vector>
#include <string>

class ParseTree
{
	public:
		ParseTree();
		ParseTree(std::string);
		void addNode(ParseTree*);
		std::string getData();
		void printTree();
	private:
		std::vector<ParseTree*> nodes;
		std::string data;
};

