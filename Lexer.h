#pragma once
#include <string>
#include <vector>
#include <fstream>

class Lexer
{
	public:
		Lexer();
		Lexer(std::string);
		std::vector<std::string> tokenize();
	private:
		std::ifstream file;
};