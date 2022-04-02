#include "Parse.hpp"
#include "Transformation.hpp"
#include "Tokenizer/Tokenizer.hpp"
#include "Parser/stage1/brackets.hpp"

std::vector<std::string> SplitString(std::string str,std::string delimeter){
    std::vector<std::string> splittedStrings = {};
    size_t pos = 0;

    while ((pos = str.find(delimeter)) != std::string::npos)
    {
        std::string token = str.substr(0, pos);
        if (token.length() > 0)
            splittedStrings.push_back(token);
        str.erase(0, pos + delimeter.length());
    }

    if (str.length() > 0)
        splittedStrings.push_back(str);
    return splittedStrings;
}

std::shared_ptr<Trs_base> Parser::parseFromStr(const std::string& s){
	std::vector<std::string> sec_spl = SplitString(s, " ");
	if(sec_spl.size() != 2){
		std::cerr<<"Transformation does not have valid sections."<<std::endl;
		return(std::make_shared<Trs_base>());
	}
	//parse transformation name, type
	std::vector<std::string> trsName_spl = SplitString(sec_spl[0], ":");
	if(trsName_spl.size() != 2){
		std::cerr<<"Unable to determine transformation name."<<std::endl;
		return(std::make_shared<Trs_base>());
	}
	std::string trsName = trsName_spl[0];
	std::vector<std::string> trsType_spl = SplitString(trsName_spl[1], "->");
	if(trsType_spl.size() != 2){
		std::cerr<<"Unable to determine transformation type (?->?)."<<std::endl;
		return(std::make_shared<Trs_base>());
	}
	return(determineType1(trsName, trsType_spl[0], trsType_spl[1], sec_spl[1]));
}

std::shared_ptr<Trs_base> Parser::determineType1(const std::string& name, const std::string& ty1, const std::string& ty2, const std::string& body){
	if(ty1.substr(0, 1) == "R"){
		if(ty1 == "R"){
			return(determineType2<Real>(name, "1", ty2, body));
		}else if(ty1.substr(0, 2) == "R^"){
			std::string rNum = ty1.substr(2, ty1.length()-2);
			return(determineType2<Real>(name, rNum, ty2, body));
		}else{
			std::cerr<<"Unable to determine transformation type 1: \""<<ty1<<"\""<<std::endl;
			return(std::make_shared<Trs_base>());
		}
	}else if(ty1.substr(0, 1) == "M"){
		std::string dimension = ty1.substr(1, ty1.length()-1);
		return(determineType2<MatR>(name, dimension, ty2, body));
	}else{
		std::cerr<<"Unable to determine transformation type 1: \""<<ty1<<"\""<<std::endl;
		return(std::make_shared<Trs_base>());
	}
}

bool isNumber(const std::string& s){
	if(s == ""){
		return(false);
	}
	for(char c : s){
		if(!(std::isdigit(c) || c == '.')){
			return(false);
		}
	}
	return(true);
}

void Parser::parseBrackets(std::vector<bracketType>* source, const std::map<std::string, uint8_t>& args){
	if(source->size() == 0){
		return;
	}
	for(unsigned int i=0;i<source->size();i++){
		if((*source)[i].type == 0){
			std::string tval = (*source)[i].svalue->value;
			if(tval == "+"){
				if(i >= 1 && i+1<source->size()){
					std::string lval = (*source)[i-1].svalue->value;
					std::string nval = (*source)[i+1].svalue->value;
					if(i >= 2){
						variableIdentifier var1;
						if(args.count(lval) != 0){
							uint8_t type = (*args.find(lval)).second;
							var1 = variableIdentifier(type);
						}else{
							continue;
						}
						variableIdentifier var2(nval);
						std::shared_ptr<Func_step_add> ts = std::make_shared<Func_step_add>(var1, var2);
						ts->runner = &runner;
						runner.steps.push_back(ts);
						i++;
					}else{
						variableIdentifier var1;
						variableIdentifier var2;
						//i == 1
						if(isNumber(lval)){
							double n = std::stod(lval);
							var1 = variableIdentifier((double)n);
						}else{
							var1 = variableIdentifier(lval);
						}
						var2 = variableIdentifier(nval);
						std::shared_ptr<Func_step_add> ts = std::make_shared<Func_step_add>(var1, var2);
						ts->runner = &runner;
						runner.steps.push_back(ts);
						i++;
					}
				}else{
					std::cerr<<"Error unable to parse +.  unable to find both lval and rval"<<std::endl;
				}
			}else if(tval == "-"){

			}else if(tval == "*"){
				if(i >= 1 && i+1<source->size()){
					std::string lval = (*source)[i-1].svalue->value;
					std::string nval = (*source)[i+1].svalue->value;
					if(i >= 2){
						//std::cout<<"Found multiplication"<<std::endl;
						variableIdentifier var1;
						if(args.count(lval) != 0){
							uint8_t type = (*args.find(lval)).second;
							var1 = variableIdentifier(type);
						}else{
							continue;
						}
						variableIdentifier var2(nval);
						std::shared_ptr<Func_step_mul> ts = std::make_shared<Func_step_mul>(var1, var2);
						ts->runner = &runner;
						runner.steps.push_back(ts);
						i++;
					}else{
						//std::cout<<"Found initial mul"<<std::endl;
						variableIdentifier var1;
						variableIdentifier var2;
						//i == 1
						if(isNumber(lval)){
							double n = std::stod(lval);
							var1 = variableIdentifier((double)n);
							//std::cout<<"var literal type: "<<(int)var1.literalType<<std::endl;
							//std::cout<<"var temp type: "<<(int)var1.tempType<<std::endl;
						}else{
							var1 = variableIdentifier(lval);
						}
						var2 = variableIdentifier(nval);
						std::shared_ptr<Func_step_mul> ts = std::make_shared<Func_step_mul>(var1, var2);
						ts->runner = &runner;
						runner.steps.push_back(ts);
						i++;
					}
				}else{
					std::cerr<<"Error unable to parse +.  unable to find both lval and rval"<<std::endl;
				}

			}else if(tval == "/"){

			}
		}else{
			parseBrackets(&(*source)[i].lvalue, args);
		}
	}
}

void Parser::generateSteps(const std::string& body, const std::map<std::string, uint8_t>& args){
	//std::cout<<"Generating steps for: "<<body<<std::endl;
	std::vector<token*> tokens = tokenizeFromString(body);
	std::vector<bracketType> brackets = ::parseBrackets(tokens);
	errorHandler::printErrors();
	/*
	for(auto& i : brackets){
		i.print();
		std::cout<<std::endl;
	}*/
	runner.steps.clear();
	parseBrackets(&brackets, args);
}
