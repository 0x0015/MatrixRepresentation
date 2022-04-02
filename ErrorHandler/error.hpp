#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "../Tokenizer/token.hpp"

class error{
	public:
		token* location;
		std::string message;
		std::string print(){
			//for now 3 cases both location and message, only location, and only message.  for the case of neither it's assumed the error shouldn't exist and will print nothing.
			if(message != ""){
				if(location != 0){
					//both location and message
					return("Error:  " + message + " at \"" + location->value + "\", line:" + std::to_string(location->l) + " char:" + std::to_string(location->c) + " in file: \"" + location->filename + "\"");
				}else{
					//has message but not location
					return("Error:  " + message);
				}
			}else{
				if(location != 0){
					//has location but not message
					return("Error:  found error at \"" + location->value + "\", line:" + std::to_string(location->l) + " char:" + std::to_string(location->c) + " in file: \"" + location->filename + "\"");
				}else{
					//has neither location nor message
					return("");
				}
			}
			return("");//all cases should be taken care of but just in case
		}
		error(token* l, std::string m){
			location = l;
			message = m;
		}
		//just because
		error(std::string m, token* l){
			location = l;
			message = m;
		}
		error(token* l){
			location = l;
			message = "";
		}
		error(std::string m){
			location = 0;
			message = m;
		}
};
