#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "error.hpp"

class errorHandler{
	public:
	static inline std::vector<error*> errors;
	static inline void raiseError(error* e){
		errors.push_back(e);
	}
	static inline void printErrors(){
		for(int i=0;i<errors.size();i++){
			std::cout<<errors[i]->print()<<std::endl;
		}
	}
};
