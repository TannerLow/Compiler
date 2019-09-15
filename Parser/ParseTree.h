#pragma once
#include <vector>
#include <string>

class ParseTree
{
	public:
		ParseTree();
		ParseTree(std::string);
		void addNode(ParseTree*);
		int nodeCount();
		std::string getData();
		void setData(std::string);
		void printTree();
	private:
		std::vector<ParseTree*> nodes;
		std::string data;
};

