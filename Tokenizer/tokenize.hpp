#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "getStringAt.hpp"
#include "tokenizeString.hpp"
#include "tokenizeSpecialCharacters.hpp"
#include "removeWhitespace.hpp"
#include "token.hpp"
#include "characterCounter.hpp"

void resetTemp(std::vector<token*>* output, std::string* temp, characterCounter* counter, std::string filename){
	if(*temp != ""){
		std::string tempNoWhitespace = removeWhitespace(*temp);
		output->push_back(new token(tempNoWhitespace, filename, counter->c, counter->l));
		*temp = "";
	}
}

std::vector<token*> tokenize(std::string file, std::string filename){
	std::vector<token*> output;
	std::string temp;
	int templ;//a bit messy, but get's the job done
	int tempc;
	int i = 0;
	characterCounter counter;//also pretty bad
	while(i<file.size()){
		if(getStringAt(file, i)== "\""){
			resetTemp(&output, &temp, &counter, filename);
			std::string str = tokenizeString(&i, &file, &counter);
			output.push_back(new token(str, filename, counter.c, counter.l));
			continue;//keeps the program in line
		}
		
		std::string specialChars = tokenizeSpecialCharacters(&i, &file);
		if(specialChars != ""){
			resetTemp(&output, &temp, &counter, filename);
			output.push_back(new token(specialChars, filename, counter.c, counter.l));
			i++;
			counter.count(file);
			continue;//keeps the program in line
		}

		if(removeWhitespace(getStringAt(file, i)) == ""){//should both function to remove whitespace, but also find the end of a real token.
			resetTemp(&output, &temp, &counter, filename);
		}else{
			
			temp = temp + removeWhitespace(getStringAt(file, i));
			templ = counter.l;
			tempc = counter.c;
			
		}
		i++;
		counter.count(file);
	}
	if(temp != "" && removeWhitespace(temp) != ""){
		output.push_back(new token(temp, filename, tempc, templ));
	}
	//last pass to fish out whitespace.  shouldn't be nessecary, but a good percausion
	for(int i=0;i<output.size();i++){
		if(removeWhitespace(output[i]->value) == ""){
			output.erase(output.begin()+i);
			i--;
		}
	}
	return(output);
}
