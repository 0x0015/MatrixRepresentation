#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "getStringAt.hpp"

std::string removeWhitespace(std::string input){
	std::vector<std::string> whitespace {" ", "\n", "\r", "\t", "\f", "\v"};
	std::string output = "";
	for(int i=0;i<input.size();i++){
		bool foundWhitespace = false;
		for(int i2=0;i2<whitespace.size();i2++){
			if(getStringAt(input, i) == whitespace[i2]){
				foundWhitespace = true;
			}
		}
		if(!foundWhitespace){
			output = output + getStringAt(input, i);
		}
	}
	return(output);
}
