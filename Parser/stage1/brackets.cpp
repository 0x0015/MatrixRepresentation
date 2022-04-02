#include "brackets.hpp"

bracketType followBracket(std::vector<token*> tokens, token* type, int* i){
	bracketType bracket;
	if(type->value == "("){
		bracket.type = 1;
	}
	if(type->value == "["){
		bracket.type = 2;
	}
	if(type->value == "{"){
		bracket.type = 3;
	}
	bracket.svalue = type;
	while(*i < tokens.size()){
		if(type->value == "[" && tokens[*i]->value == "]"){
			return(bracket);	
		}
		if(type->value == "(" && tokens[*i]->value == ")"){
			return(bracket);
		}
		if(type->value == "{" && tokens[*i]->value == "}"){
			return(bracket);
		}
		if(type->value != "[" && tokens[*i]->value == "]"){
			errorHandler::raiseError(new error("found closing bracket of type " + tokens[*i]->value + " with no opening bracket", tokens[*i]));
		}
		if(type->value != "(" && tokens[*i]->value == ")"){
			errorHandler::raiseError(new error("found closing bracket of type " + tokens[*i]->value + " with no opening bracket", tokens[*i]));
		}
		if(type->value != "{" && tokens[*i]->value == "}"){
			errorHandler::raiseError(new error("found closing bracket of type " + tokens[*i]->value + " with no opening bracket", tokens[*i]));
		}

		if(tokens[*i]->value != "[" && tokens[*i]->value != "(" && tokens[*i]->value != "{"){
			bracket.lvalue.push_back(bracketType(tokens[*i]));
		}else{
			(*i)++;
			bracket.lvalue.push_back(followBracket(tokens, tokens[(*i)-1], i));
		}
		(*i)++;
	}
	//std::cout<<"ERROR: did not find closing " + type<<std::endl;
	errorHandler::raiseError(new error("did not find closing " + type->value, type));
	return(bracketType());
}

std::vector<bracketType> parseBrackets(std::vector<token*> tokens){
	std::vector<bracketType> output;
	for(int i=0;i<tokens.size();i++){
		if(tokens[i]->value != "[" && tokens[i]->value != "(" && tokens[i]->value != "{"){
			
		 	if(tokens[i]->value == "]" || tokens[i]->value == "}" || tokens[i]->value == ")"){
				//you have to open a bracket before you can close it.
				errorHandler::raiseError(new error("found closing bracket of type " + tokens[i]->value + " with no opening bracket", tokens[i]));
			}else{
				output.push_back(bracketType(tokens[i]));
			}

		}else{
			i++;
			output.push_back(followBracket(tokens, tokens[i-1], &i));
		}
	}
	return(output);
}
