#pragma once
#include <iostream>
#include <vector>
#include <optional>
#include <memory>
#include <functional>
#include <unordered_map>
#include "Datatypes/Vector.hpp"
#include "Datatypes/Matrix.hpp"

class Func_runner;

class Func_step{
public:
	Func_runner* runner;
	virtual void apply();
	Func_step();
};





class Func_runner{
public:
	std::unordered_map<std::string, double> scalars;
	std::unordered_map<std::string, Vector> vectors;
	std::unordered_map<std::string, Matrix> matrices;
	double tempScalar;
	Vector tempVector;
	Matrix tempMatrix;//effectively registers for unnamed rets
	std::vector<std::shared_ptr<Func_step>> steps;
	template<class T> void setVar(const std::string& name, const T& val){
		//std::cout<<"Setting var:  "<<name<<" typeid: "<<typeid(T).name()<<std::endl;
		if constexpr(std::is_same_v<T, double>){
			if(vectors.count(name) != 0){
				std::cerr<<"Cannot create variable with already used name as vector. (sca)"<<std::endl;
			}
			if(matrices.count(name) != 0){
				std::cerr<<"Cannot create variable with already used name as matrix. (sca)"<<std::endl;
			}
			scalars[name] = val;
		}else if constexpr(std::is_same_v<T, Vector>){
			if(matrices.count(name) != 0){
				std::cerr<<"Cannot create variable with already used name as matrix. (vec)"<<std::endl;
			}
			if(scalars.count(name) != 0){
				std::cerr<<"Cannot create variable with already used name as scalar. (vec)"<<std::endl;
			}
			vectors[name] = val;
		}else if constexpr(std::is_same_v<T, Matrix>){
			if(scalars.count(name) != 0){
				std::cerr<<"Cannot create variable with already used name as scalar. (mat)"<<std::endl;
			}
			if(vectors.count(name) != 0){
				std::cerr<<"Cannot create variable with already used name as vector. (mat)"<<std::endl;
			}
			matrices[name] = val;
		}else{
			std::cerr<<"Unknown type: typeid:  "<<typeid(T).name()<<std::endl;
		}
	}
	template<class T> std::optional<T> getVar(const std::string& name){
		if constexpr(std::is_same_v<T, double>){
			return(scalars[name]);
		}else if constexpr(std::is_same_v<T, Vector>){
			return(vectors[name]);
		}else if constexpr(std::is_same_v<T, Matrix>){
			return(matrices[name]);
		}else{
			std::cerr<<"Unknown type: typeid:  "<<typeid(T).name()<<std::endl;
			return(std::nullopt);
		}
	}
	void run();
	Func_runner();
};

class variableIdentifier{
public:
	std::string name;
	uint8_t tempType = 0;//0 = none, 1 = scalar, 2 = vector, 3 = matrix
	uint8_t literalType = 0;//same as above
	double dLiteral;
	Vector vLiteral;
	Matrix mLiteral;
	variableIdentifier();
	variableIdentifier(const std::string& n);
	variableIdentifier(uint8_t temp);
	variableIdentifier(double literal);
	variableIdentifier(const Vector& literal);
	variableIdentifier(const Matrix& literal);
	uint8_t getType(const Func_runner* runner);
	template<class T> std::optional<T> getValue(Func_runner* runner) const {
		if(tempType == 0 && literalType == 0){
			return(runner->getVar<T>(name));
		}else{
			if constexpr(std::is_same_v<T, double>){
				if(tempType == 1){
					return(runner->tempScalar);
				}else if(literalType == 1){
					return(dLiteral);
				}else{
					return(std::nullopt);
				}
			}else if constexpr(std::is_same_v<T, Vector>){
				if(tempType == 2){
					return(runner->tempVector);
				}else if(literalType == 2){
					return(vLiteral);
				}else{
					return(std::nullopt);
				}
			}else if constexpr(std::is_same_v<T, Matrix>){
				if(tempType == 3){
					return(runner->tempMatrix);
				}else if(literalType == 3){
					return(mLiteral);
				}else{
					return(std::nullopt);
				}
			}else{
				return(std::nullopt);
			}
		}
	}
};

class Func_step_func : public Func_step{
public:
	variableIdentifier var;
	std::function<void(Func_runner*, const variableIdentifier&)> func;
	void apply();
	Func_step_func();
	Func_step_func(std::function<void(Func_runner*, const variableIdentifier&)> f, const variableIdentifier& v);
	Func_step_func(std::function<void(Func_runner*, const variableIdentifier&)> f);
};

class Func_step_add : public Func_step{
public:
	variableIdentifier var1, var2;
	void apply();
	Func_step_add();
	Func_step_add(const variableIdentifier& arg1, const variableIdentifier& arg2);
};

class Func_step_sub : public Func_step{
public:
	variableIdentifier var1, var2;
	void apply();
	Func_step_sub();
	Func_step_sub(const variableIdentifier& arg1, const variableIdentifier& arg2);
};

class Func_step_mul : public Func_step{
public:
	variableIdentifier var1, var2;
	void apply();
	Func_step_mul();
	Func_step_mul(const variableIdentifier& arg1, const variableIdentifier& arg2);
};

class Func_step_div : public Func_step{
public:
	variableIdentifier var1, var2;
	void apply();
	Func_step_div();
	Func_step_div(const variableIdentifier& arg1, const variableIdentifier& arg2);
};

class Func_step_createVector : public Func_step{
public:
	std::vector<variableIdentifier> vars;
	void apply();
	Func_step_createVector();
	Func_step_createVector(const std::vector<variableIdentifier>& a);
};

class Func_step_loadFromTemp : public Func_step{
public:
	variableIdentifier var;
	std::string name;
	void apply();
	Func_step_loadFromTemp();
	Func_step_loadFromTemp(const variableIdentifier& a1, const std::string& n);
};

class Func_step_loadToReg : public Func_step{
public:
	variableIdentifier val;
	void apply();
	Func_step_loadToReg();
	Func_step_loadToReg(const variableIdentifier& v);
};
