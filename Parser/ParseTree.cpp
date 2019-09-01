#include "ParseTree.h"
#include <iostream>

using namespace std;

//special ascii chars
char vert = char(179), vertRight = char(195), Right = char(192), horiz = char(196);
//Default Constructor, Should not be used
ParseTree::ParseTree() {}
//Overloaded Constructor, takes string and sets it as the new root's data
ParseTree::ParseTree(string rootData) {
	data = rootData;
}
//Add node, or child, to the tree root
void ParseTree::addNode(ParseTree* node) {
	nodes.push_back(node);
}

std::string ParseTree::getData()
{
	return data;
}
//print tree with visual format
void ParseTree::printTree()
{
	cout << data << endl;
	for (int i = 0; i < int(nodes.size() - 1); i++) {
		cout << vertRight << horiz;
		nodes[i]->printTree();
	}
	if (!nodes.empty()) {
		cout << Right << horiz;
		nodes.back()->printTree();
	}
}
