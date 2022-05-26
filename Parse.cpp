#include "Parse.hpp"
#include "Transformation.hpp"
#include "Tokenizer/Tokenizer.hpp"
#include "Parser/stage1/brackets.hpp"
#include <unordered_map>
#include <cmath>

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
	std::vector<std::string> sec_spl_t = SplitString(s, " ");
	std::vector<std::string> sec_spl;
	if(sec_spl_t.size() == 2){
		sec_spl = sec_spl_t;
	}else if(sec_spl_t.size() > 2){
		sec_spl.push_back(sec_spl_t[0]);
		std::string sec_spl_1;
		for(unsigned int i=1;i<sec_spl_t.size();i++){
			sec_spl_1 += sec_spl_t[i];
			if(i+1 < sec_spl_t.size()){
				sec_spl_1 += " ";
			}
		}
		sec_spl.push_back(sec_spl_1);
	}else{
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

std::unordered_map<std::string, std::pair<std::function<void(Func_runner*, const variableIdentifier&)>, uint8_t>> builtins = {
	{"sin", {[](Func_runner* f, const variableIdentifier& v){
									auto val = v.getValue<double>(f);
									if(val){
										f->tempScalar = std::sin(val.value());
									}
								}, 1}},
	{"cos", {[](Func_runner* f, const variableIdentifier& v){
									auto val = v.getValue<double>(f);
									if(val){
										f->tempScalar = std::cos(val.value());
									}
								}, 1}},
	{"tan", {[](Func_runner* f, const variableIdentifier& v){
									auto val = v.getValue<double>(f);
									if(val){
										f->tempScalar = std::tan(val.value());
									}
								}, 1}},
	{"tr", {[](Func_runner* f, const variableIdentifier& v){
									auto val = v.getValue<Matrix>(f);
									if(val){
										f->tempMatrix = val.value().transpose();
									}
								}, 3}}
};

std::pair<variableIdentifier, uint8_t> Parser::getVar(const std::vector<std::string>& code, const std::vector<std::pair<std::string, uint8_t>>& args, unsigned int& tempCount, std::vector<std::pair<std::string, uint8_t>>& tempVars, uint8_t& lastType, unsigned int& i){
	std::string tok = code[i];
	if(tok == ""){
		std::cerr<<"Fount blank token"<<std::endl;
	}
	variableIdentifier varI;
	uint8_t type = 0;
	if(argsContains(args, tok)){
		type = argsFind(args, tok).second;
		varI = variableIdentifier(tok);
	}else if(isNumber(tok)){
		type = 1;
		varI = variableIdentifier(std::stod(tok));
	}else if(argsContains(tempVars, tok)){
		type = argsFind(tempVars, tok).second;
		varI = variableIdentifier(tok);
	}else if(builtins.count(tok) != 0 && i+1 < code.size()){//std::unordered_map.contains is c++20
		i++;
		std::string nval = code[i];
		auto [varP, pt] = getVar(code, args, tempCount, tempVars, lastType, i);
		auto [func, t] = builtins[tok];
		std::shared_ptr<Func_step_func> ts = std::make_shared<Func_step_func>(func, varP);
		ts->runner = &runner;
		runner.steps.push_back(ts);
		type = t;
		varI = variableIdentifier(type);//FINALLY A SOLUTION
	}else{
		std::cerr<<"Unable to determine type.  defaulting to last type."<<std::endl;
		varI = variableIdentifier(lastType);
		type = lastType;
	}
	//std::cout<<"returning type: "<<(int)type<<std::endl;
	//std::cout<<"returning variableIdentifier: name: "<<varI.name<<", tempType: "<<(int)varI.tempType<<", literalType: "<<(int)varI.literalType<<std::endl;
	lastType = type;
	return(std::make_pair(varI, type));
}

uint8_t Parser::parseCode(const std::vector<std::string>& code, const std::vector<std::pair<std::string, uint8_t>>& args, unsigned int& tempCount, std::vector<std::pair<std::string, uint8_t>>& tempVars){
	//std::cout<<"Parsing code: "<<std::endl;
	uint8_t lastType = 0;
	for(unsigned int i=0;i<code.size();i++){
		if(i == 0){
			auto [varI, type] = getVar(code, args, tempCount, tempVars, lastType, i);
			//std::cout<<"varI name: "<<varI.name<<std::endl;
			std::shared_ptr<Func_step_loadToReg> ts = std::make_shared<Func_step_loadToReg>(varI);
			ts->runner = &runner;
			runner.steps.push_back(ts);
		}else{
			std::string tval = code[i];
			if(tval == "+"){
				if(i+1 < code.size()){
					i++;
					std::string nval = code[i];
					auto [varN, type] = getVar(code, args, tempCount, tempVars, lastType, i);
					variableIdentifier varP = variableIdentifier(type);
					std::shared_ptr<Func_step_add> ts = std::make_shared<Func_step_add>(varP, varN);
					ts->runner = &runner;
					runner.steps.push_back(ts);
				}
			}else if(tval == "-"){
				if(i+1 < code.size()){
					i++;
					std::string nval = code[i];
					auto [varN, type] = getVar(code, args, tempCount, tempVars, lastType, i);
					variableIdentifier varP = variableIdentifier(type);
					std::shared_ptr<Func_step_sub> ts = std::make_shared<Func_step_sub>(varP, varN);
					ts->runner = &runner;
					runner.steps.push_back(ts);
				}

			}else if(tval == "*"){
				if(i+1 < code.size()){
					i++;
					std::string nval = code[i];
					auto [varN, type] = getVar(code, args, tempCount, tempVars, lastType, i);
					uint8_t pType;
					if(type == 1){
						pType = lastType;
					}else{
						pType = 1;
					}
					variableIdentifier varP = variableIdentifier(pType);
					std::shared_ptr<Func_step_mul> ts = std::make_shared<Func_step_mul>(varP, varN);
					ts->runner = &runner;
					runner.steps.push_back(ts);
				}
			}else if(tval == "/"){
				if(i+1 < code.size()){
					i++;
					std::string nval = code[i];
					auto [varN, type] = getVar(code, args, tempCount, tempVars, lastType, i);
					uint8_t pType;
					if(type == 1){
						pType = lastType;
					}else{
						pType = 1;
					}
					variableIdentifier varP = variableIdentifier(pType);
					std::shared_ptr<Func_step_div> ts = std::make_shared<Func_step_div>(varP, varN);
					ts->runner = &runner;
					runner.steps.push_back(ts);
				}

			}
		}
	}
	return(lastType);
}

void Parser::generateSteps(const std::string& body, const std::vector<std::pair<std::string, uint8_t>>& args){
	//std::cout<<"Generating steps for: "<<body<<std::endl;
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
