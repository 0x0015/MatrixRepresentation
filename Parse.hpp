#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <map>
#include <type_traits>
#include "Spaces/Rn.hpp"
#include "Spaces/MnxmR.hpp"
#include "Transformation.hpp"
#include "Runner.hpp"

class Trs_base;
class bracketType;

std::vector<std::string> SplitString(std::string str,std::string delimeter);

class Parser{
public:
	std::shared_ptr<Trs_base> parseFromStr(const std::string& s);
	std::shared_ptr<Trs_base> determineType1(const std::string& name, const std::string& ty1, const std::string& ty2, const std::string& body);
	void generateSteps(const std::string& body, const std::map<std::string, uint8_t>& args);
	void parseBrackets(std::vector<bracketType>* brackets, const std::map<std::string, uint8_t>& args);
	Func_runner runner;
	template<class T> T createSpace(const std::string& ty){
		if constexpr (std::is_same_v<T, Real>){
			if(ty.find("x") != std::string::npos){
				return(T());
			}else{
				//it's a vector
				unsigned int n = std::stoi(ty);
				return(T(n));
			}
		}else if constexpr (std::is_same_v<T, MatR>){
			if(ty.find("x") != std::string::npos){
				//it's a matrix
				std::vector<std::string> n_spl = SplitString(ty, "x");
				unsigned int n_0 = std::stoi(n_spl[0]);
				unsigned int n_1 = std::stoi(n_spl[1]);
				return(T(n_0, n_1));
			}else{
				return(T());
			}
		}else{
			return(T());
		}
	}
	template<class T1, class T2> T2 callParsedFunc(T1 x){//Expand this
			if constexpr(std::is_same_v<T1, T2>){
				return(x);
			}
			return(T2());
	}
	template<class T1, class T2> std::shared_ptr<Trs_base> parseBody(const std::string& name, const std::string& ty1, const std::string& ty2, const std::string& body){
		//std::cout<<"Parsing body:  ty1: "<<ty1<<", ty2:  "<<ty2<<", name: "<<name<<std::endl;
		std::vector<std::string> eq_spl = SplitString(body, "=");
		if(eq_spl.size() != 2){
			std::cerr<<"Unable to determine lhs and rhs of equation."<<std::endl;
			return(std::make_shared<Trs_base>());
		}
		//lhs
		if(!(eq_spl[0].substr(0, name.length()) == name)){
			std::cerr<<"Failed to match function name: "<<eq_spl[0].substr(0, name.length())<<" and "<<name<<std::endl;
		}
		std::string args = eq_spl[0].substr(name.length()+1, eq_spl[0].length()-name.length()-2);
		std::map<std::string, uint8_t> arg_types;
		bool split_args = false;
		if(args.find(",") == std::string::npos){
			//arg is a vector or matrix type
			uint8_t t1n = 0;
			uint8_t t2n = 0;
			if constexpr(std::is_same_v<T1, double>){
				t1n = 1;
			}else if constexpr(std::is_same_v<T1, Vector>){
				t1n = 2;
			}else if constexpr(std::is_same_v<T1, Matrix>){
				t1n = 3;
			}
			if constexpr(std::is_same_v<T2, double>){
				t2n = 1;
			}else if constexpr(std::is_same_v<T2, Vector>){
				t2n = 2;
			}else if constexpr(std::is_same_v<T2, Matrix>){
				t2n = 3;
			}
			arg_types[args] = t1n;
		}else{
			std::vector<std::string> args_spl = SplitString(args, ",");
			//multiple args, presumably of scalar type
			for(auto& o : args_spl){
				arg_types[o] = 1;
			}
			split_args = true;
		}
		//rhs
		//basic plan: tokenize, parse brackets, loop through recursively.
		generateSteps(eq_spl[1], arg_types);
		//function construction
		T1 V = createSpace<T1>(ty1);
		T2 W = createSpace<T2>(ty2);
		std::function<typename T2::element(typename T1::element)> func = [this, arg_types, args, split_args](typename T1::element x){
			if(arg_types.size() == 0){
				std::cerr<<"Transformation arguments were found to be empty"<<std::endl;
				return(typename T2::element());
			}
			if(split_args){
				//I'll have to get the items out of the vector(or matrix).  I'll do this later
			}else{
				runner.setVar<typename T1::element>(args, x);
			}
			runner.run();
			if constexpr(std::is_same_v<typename T2::element, double>){
				return(runner.tempScalar);
			}else if constexpr(std::is_same_v<typename T2::element, Vector>){
				return(runner.tempVector);
			}else if constexpr(std::is_same_v<typename T2::element, Matrix>){
				return(runner.tempMatrix);
			}else{
				return(typename T2::element());
			}
		};
		std::shared_ptr<Transformation<T1, T2>> tr = std::make_shared<Transformation<T1, T2>>(V, W, func);
		//std::cout<<"Successfuly parsed function"<<std::endl;
		return(tr);
	}
	template<class T1> std::shared_ptr<Trs_base> determineType2(const std::string& name, const std::string& ty1, const std::string& ty2, const std::string& body){
		if(ty2.substr(0, 1) == "R"){
			if(ty2 == "R"){
				return(parseBody<T1, Real>(name, ty1, "1", body));
			}else if(ty2.substr(0, 2) == "R^"){
				std::string rNum = ty2.substr(2, ty2.length()-2);
				return(parseBody<T1, Real>(name, ty1, rNum, body));
			}else{
				std::cerr<<"Unable to determine transformation type 2: \""<<ty2<<"\""<<std::endl;
				return(std::make_shared<Trs_base>());
			}
		}else if(ty2.substr(0, 1) == "M"){
			std::string dimension = ty2.substr(1, ty2.length()-1);
			return(parseBody<T1, MatR>(name, ty1, dimension, body));
		}else{
			std::cerr<<"Unable to determine transformation type 2: \""<<ty2<<"\""<<std::endl;
			return(std::make_shared<Trs_base>());
		}
	}
};
