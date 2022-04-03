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

bool argsContains(const std::vector<std::pair<std::string, uint8_t>>& args, const std::string& val){
	for(auto& i : args){
		if(i.first == val){
			return(true);
		}
	}
	return(false);
}

std::pair<std::string, uint8_t> argsFind(const std::vector<std::pair<std::string, uint8_t>>& args, const std::string& val){
	for(auto& i : args){
		if(i.first == val){
			return(i);
		}
	}
	return(std::pair<std::string, uint8_t>());
}
//step 1: loop through more single value creation funcs (parenthases, vector creation), and create code to put them into vars in memory.  output the new code with them replaced by said vars
//step 2: loop through the more simple representation, which only has values and +-*/

uint8_t Parser::parseBrackets(std::vector<bracketType>* source, const std::vector<std::pair<std::string, uint8_t>>& args, unsigned int& tempCount, std::vector<std::pair<std::string, uint8_t>>& tempVars){
	std::vector<std::string> postBr;
	if(source->size() == 0){
		return 0;
	}
	for(unsigned int i=0;i<source->size();i++){
		if((*source)[i].type != 0){
			bool isVectorC = false;
			std::vector<bracketType>& itemLval = (*source)[i].lvalue;
			if(itemLval.size() >= 3){
				for(auto& o : itemLval){
					if(o.type == 0 && o.svalue->value == ","){
						isVectorC = true;
					}
				}
			}
			if(isVectorC){
				std::vector<bracketType> tempVals;
				std::vector<std::string> vectorCompNames;
				for(auto& o : itemLval){
					if(o.type == 0 && o.svalue->value == ","){
						//seperate
						uint8_t retType = parseBrackets(&tempVals, args, tempCount, tempVars);
						std::string retName = "__temp" + std::to_string((int)retType) + "_" + std::to_string(tempCount++);
						variableIdentifier var1(retType);
						std::shared_ptr<Func_step_loadFromTemp> ts = std::make_shared<Func_step_loadFromTemp>(var1, retName);
						ts->runner = &runner;
						runner.steps.push_back(ts);
						vectorCompNames.push_back(retName);
						tempVars.push_back(std::make_pair(retName, retType));
						tempVals.clear();
					}else{
						tempVals.push_back(o);
					}
				}
				uint8_t retType = parseBrackets(&tempVals, args, tempCount, tempVars);
				std::string retName = "__temp" + std::to_string((int)retType) + "_" + std::to_string(tempCount++);
				variableIdentifier var1(retType);
				std::shared_ptr<Func_step_loadFromTemp> ts = std::make_shared<Func_step_loadFromTemp>(var1, retName);
				ts->runner = &runner;
				runner.steps.push_back(ts);
				vectorCompNames.push_back(retName);
				tempVars.push_back(std::make_pair(retName, retType));
				//create vector composition object
				std::vector<variableIdentifier> varIdentities;
				for(auto& o : vectorCompNames){
					varIdentities.push_back(variableIdentifier(o));
				}
				std::string compName = "__temp" + std::to_string(2) + "_" + std::to_string(tempCount++);
				std::shared_ptr<Func_step_createVector> cts = std::make_shared<Func_step_createVector>(varIdentities);
				cts->runner = &runner;
				runner.steps.push_back(cts);

				std::shared_ptr<Func_step_loadFromTemp> clts = std::make_shared<Func_step_loadFromTemp>(variableIdentifier((uint8_t)2), compName);
				clts->runner = &runner;
				runner.steps.push_back(clts);
				postBr.push_back(compName);
				tempVars.push_back(std::make_pair(compName, (uint8_t)2));
			}else{
				uint8_t retType = parseBrackets(&itemLval, args, tempCount, tempVars);
				std::string retName = "__temp" + std::to_string((int)retType) + "_" + std::to_string(tempCount++);
				variableIdentifier var1(retType);
				std::shared_ptr<Func_step_loadFromTemp> ts = std::make_shared<Func_step_loadFromTemp>(var1, retName);
				ts->runner = &runner;
				runner.steps.push_back(ts);
				postBr.push_back(retName);
				tempVars.push_back(std::make_pair(retName, retType));
			}
		}else{
			postBr.push_back((*source)[i].svalue->value);
		}
	}
	return(parseCode(postBr, args, tempCount, tempVars));
}

uint8_t Parser::parseCode(const std::vector<std::string>& code, const std::vector<std::pair<std::string, uint8_t>>& args, unsigned int& tempCount, std::vector<std::pair<std::string, uint8_t>>& tempVars){
	//std::cout<<"Parsing code: "<<std::endl;
	uint8_t lastType = 0;
	if(code.size() == 1){
		std::string tval = code[0];
		variableIdentifier varI(tval);
		if(isNumber(tval)){
			lastType = 1;
			varI = variableIdentifier(std::stod(tval));
		}else if(argsContains(tempVars, tval)){
			lastType = argsFind(tempVars, tval).second;
		}else if(argsContains(args, tval)){
			lastType = argsFind(args, tval).second;
		}else{
			std::cerr<<"Error failed to find multiplication second arg type."<<std::endl;
		}
		std::shared_ptr<Func_step_loadToReg> ts = std::make_shared<Func_step_loadToReg>(varI);
		ts->runner = &runner;
		runner.steps.push_back(ts);
		//std::cout<<"Returning lastType: "<<(int)lastType<<std::endl;
		return(lastType);
	}
	for(unsigned int i=0;i<code.size();i++){
		std::string tval = code[i];
		if(tval == "+"){
			if(i >= 1 && i+1<code.size()){
				std::string lval = code[i-1];
				std::string nval = code[i+1];
				if(i >= 2){
					variableIdentifier var1;
					if(argsContains(args, lval)){
						uint8_t type = argsFind(args, lval).second;
						var1 = variableIdentifier(type);
						lastType = type;
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
						lastType = 1;//double
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
			if(i >= 1 && i+1<code.size()){
				std::string lval = code[i-1];
				std::string nval = code[i+1];
				if(i >= 2){
					//std::cout<<"Found multiplication"<<std::endl;
					variableIdentifier var1;
					
					if(argsContains(args, lval)){
						uint8_t type = argsFind(args, lval).second;
						var1 = variableIdentifier(type);
						lastType = type;
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
					if(argsContains(tempVars, nval)){
						lastType = argsFind(tempVars, nval).second;
					}else if(argsContains(args, nval)){
						lastType = argsFind(args, nval).second;
					}else{
						std::cerr<<"Error failed to find multiplication second arg type."<<std::endl;
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
	}
	//std::cout<<"Returning lastType: "<<(int)lastType<<std::endl;
	return(lastType);
}

void Parser::generateSteps(const std::string& body, const std::vector<std::pair<std::string, uint8_t>>& args){
	std::cout<<"Generating steps for: "<<body<<std::endl;
	std::vector<token*> tokens = tokenizeFromString(body);
	/*for(auto& i : tokens){
		std::cout<<i->value<<std::endl;
	}*/
	std::vector<bracketType> brackets = ::parseBrackets(tokens);
	errorHandler::printErrors();
	runner.steps.clear();
	unsigned int tempCount = 0;
	std::vector<std::pair<std::string, uint8_t>> tempVars;
	parseBrackets(&brackets, args, tempCount, tempVars);
}
