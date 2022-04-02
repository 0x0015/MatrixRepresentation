#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "getStringAt.hpp"

std::string tokenizeSpecialCharacters(int* i, std::string* file){
	std::vector<std::string> specialCharacters {"{", "}", "#", "<", ">", "(", ")", "[", "]", "+", "-", "*", "/", "&", "",";",":", ","};//list of strings to check for

	for(int i2=0;i2<specialCharacters.size();i2++){
		if(file->size() >  specialCharacters[i2].size() + *i){//check to make sure the file is long enough to get the substr
			if(string(file->substr(*i, specialCharacters[i2].size())) == specialCharacters[i2]){
				return(specialCharacters[i2]);
			}
		}
	}
	return("");
}
