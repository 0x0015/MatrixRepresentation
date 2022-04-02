#include "bracketType.hpp"


void bracketType::print(){
			if(type != 0){
				for(int i=0;i<lvalue.size();i++){
					std::cout<<type;
					lvalue[i].print();
				}
			}else{
				std::cout<<svalue->value;
			}
		}
bracketType::bracketType(){}
bracketType::bracketType(token* v){
			svalue = v;
			type = 0;
		}
bracketType::bracketType(int t, token* v, std::vector<bracketType> l){
			lvalue = l;
			type = t;
			svalue = v;
		}

