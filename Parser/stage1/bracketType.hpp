#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "../../Tokenizer/token.hpp"

class bracketType{
	public:
		token* svalue;
		std::vector<bracketType> lvalue;
		int type;//0 for token, 1 for parenthases, 2 for square, 3 for curly
		void print();
		bracketType();
		bracketType(token* v);
		bracketType(int t, token* v, std::vector<bracketType> l);
};
