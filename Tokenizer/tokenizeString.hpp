#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "getStringAt.hpp"
#include "characterCounter.hpp"

std::string tokenizeString(int* i, std::string* file, characterCounter* counter){
	std::string output;
	
	if(getStringAt(*file, *i) == "\""){
		(*i)++;
		counter->count(*file);
	}
	output = output + "\"";

	int backslashCounter = 0;
	while(*i < file->size()){
		if(getStringAt(*file, *i) == "\\"){
			backslashCounter++;
		}else{
			backslashCounter = 0;
		}
		output = output + getStringAt(*file, *i);

		if(getStringAt(*file,  *i) == "\""){
			if(backslashCounter%2 != 0 || backslashCounter == 0){
				(*i)++;
				counter->count(*file);
				//std::cout<<"string returning " + output<<std::endl; // for debugging
				return(output);
			}
		}
		*i+=1;
		counter->count(*file);
	}
	std::cout<<"error:  never found end of string"<<std::endl;
	return(output);
}
