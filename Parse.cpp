#include "Parse.hpp"
#include "Transformation.hpp"

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
