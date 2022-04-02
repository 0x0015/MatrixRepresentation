#pragma once
#include <iostream>
#include <vector>
#include "tokenize.hpp"
#include "token.hpp"

std::vector<token*> tokenizeFromString(std::string file){
	return(tokenize(file, ""));
}


