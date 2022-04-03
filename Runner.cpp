#include "Runner.hpp"

Func_step::Func_step(){}

void Func_step::apply(){}

Func_step_func::Func_step_func(){}

Func_step_func::Func_step_func(std::function<void(Func_runner*)> f){
	if(f){
		func = f;
	}
}

void Func_step_func::apply(){
	if(func){
		func(runner);
	}
}

variableIdentifier::variableIdentifier(){}
variableIdentifier::variableIdentifier(const std::string& n){
	name = n;
}
variableIdentifier::variableIdentifier(uint8_t temp){
	tempType = temp;
}

variableIdentifier::variableIdentifier(double literal){
	dLiteral = literal;
	literalType = 1;
}

variableIdentifier::variableIdentifier(const Vector& literal){
	vLiteral = literal;
	literalType = 2;
}

variableIdentifier::variableIdentifier(const Matrix& literal){
	mLiteral = literal;
	literalType = 3;
}

uint8_t variableIdentifier::getType(const Func_runner* runner){
	if(tempType != 0){
		return(tempType);
	}else if(literalType != 0){
		return(literalType);
	}else{
		if(runner->scalars.count(name) != 0){
			return(1);
		}else if(runner->vectors.count(name) != 0){
			return(2);
		}else if(runner->matrices.count(name) != 0){
			return(3);
		}
	}
	return(0);
}

Func_step_add::Func_step_add(){}
Func_step_add::Func_step_add(const variableIdentifier& arg1, const variableIdentifier& arg2){
	var1 = arg1;
	var2 = arg2;
}

void Func_step_add::apply(){
	if(uint8_t varType = var1.getType(runner) == var2.getType(runner)){
		if(varType == 0){
			std::cerr<<"Error: add arguement type is null"<<std::endl;
			return;
		}else{
			if(varType == 1){
				auto val1 = var1.getValue<double>(runner);
				auto val2 = var2.getValue<double>(runner);
				if(val1 && val2){
					runner->tempScalar = val1.value() + val2.value();
				}else{
					std::cerr<<"Error: failed to get add variable value."<<std::endl;
					return;
				}
			}else if(varType == 2){
				auto val1 = var1.getValue<Vector>(runner);
				auto val2 = var2.getValue<Vector>(runner);
				if(val1 && val2){
					runner->tempVector = val1.value() + val2.value();
				}else{
					std::cerr<<"Error: failed to get add variable value."<<std::endl;
					return;
				}
			}else if(varType == 3){
				auto val1 = var1.getValue<Matrix>(runner);
				auto val2 = var2.getValue<Matrix>(runner);
				if(val1 && val2){
					runner->tempMatrix = val1.value() + val2.value();
				}else{
					std::cerr<<"Error: failed to get add variable value."<<std::endl;
					return;
				}
			}
		}
	}else{
		std::cerr<<"Error: add has unequal arguement types. ("<<(int)var1.getType(runner)<<", "<<(int)var2.getType(runner)<<")"<<std::endl;
		return;
	}
}

Func_step_sub::Func_step_sub(){}
Func_step_sub::Func_step_sub(const variableIdentifier& arg1, const variableIdentifier& arg2){
	var1 = arg1;
	var2 = arg2;
}

void Func_step_sub::apply(){
	if(uint8_t varType = var1.getType(runner) == var2.getType(runner)){
		if(varType == 0){
			std::cerr<<"Error: sub arguement type is null"<<std::endl;
			return;
		}else{
			if(varType == 1){
				auto val1 = var1.getValue<double>(runner);
				auto val2 = var2.getValue<double>(runner);
				if(val1 && val2){
					runner->tempScalar = val1.value() - val2.value();
				}else{
					std::cerr<<"Error: failed to get sub variable value."<<std::endl;
					return;
				}
			}else if(varType == 2){
				auto val1 = var1.getValue<Vector>(runner);
				auto val2 = var2.getValue<Vector>(runner);
				if(val1 && val2){
					runner->tempVector = val1.value() - val2.value();
				}else{
					std::cerr<<"Error: failed to get sub variable value."<<std::endl;
					return;
				}
			}else if(varType == 3){
				auto val1 = var1.getValue<Matrix>(runner);
				auto val2 = var2.getValue<Matrix>(runner);
				if(val1 && val2){
					runner->tempMatrix = val1.value() - val2.value();
				}else{
					std::cerr<<"Error: failed to get sub variable value."<<std::endl;
					return;
				}
			}
		}
	}else{
		std::cerr<<"Error: sub has unequal arguement types. ("<<(int)var1.getType(runner)<<", "<<(int)var2.getType(runner)<<")"<<std::endl;
		return;
	}
}

Func_step_mul::Func_step_mul(){}
Func_step_mul::Func_step_mul(const variableIdentifier& arg1, const variableIdentifier& arg2){
	var1 = arg1;
	var2 = arg2;
}

void Func_step_mul::apply(){
	uint8_t var1Type = var1.getType(runner);
	uint8_t var2Type = var2.getType(runner);
	if(var1Type == 1 || var2Type == 1){
		uint8_t varType;
		uint8_t scalarVar;
		if(var1Type == 1){
			varType = var2Type;
			scalarVar = 1;
		}else{
			varType = var1Type;
			scalarVar = 2;
		}
		double doubleVar;
		if(scalarVar == 1){
			auto val = var1.getValue<double>(runner);
			if(val){
				doubleVar = val.value();
			}else{
				std::cerr<<"Error: failed to get mul scalar value."<<std::endl;
			}
		}else{
			auto val = var2.getValue<double>(runner);
			if(val){
				doubleVar = val.value();
			}else{
				std::cerr<<"Error: failed to get mul scalar value."<<std::endl;
			}
		}
		if(varType == 1){
			if(scalarVar == 1){
				auto val = var2.getValue<double>(runner);
				if(val){
					runner->tempScalar = doubleVar * val.value();
				}else{
					std::cerr<<"Error: failed to get mul variable value."<<std::endl;
					return;
				}
			}else{
				auto val = var1.getValue<double>(runner);
				if(val){
					runner->tempScalar = doubleVar * val.value();
				}else{
					std::cerr<<"Error: failed to get mul variable value."<<std::endl;
					return;
				}
			}
		}else if(varType == 2){
			if(scalarVar == 1){
				auto val = var2.getValue<Vector>(runner);
				if(val){
					runner->tempVector = val.value() * doubleVar;
				}else{
					std::cerr<<"Error: failed to get mul variable value."<<std::endl;
					return;
				}
			}else{
				auto val = var1.getValue<Vector>(runner);
				if(val){
					runner->tempVector = val.value() * doubleVar;
				}else{
					std::cerr<<"Error: failed to get mul variable value."<<std::endl;
					return;
				}
			}
		}else if(varType == 3){
			if(scalarVar == 1){
				auto val = var2.getValue<Matrix>(runner);
				if(val){
					runner->tempMatrix = val.value() * doubleVar;
				}else{
					std::cerr<<"Error: failed to get mul variable value."<<std::endl;
					return;
				}
			}else{
				auto val = var1.getValue<Matrix>(runner);
				if(val){
					runner->tempMatrix = val.value() * doubleVar;
				}else{
					std::cerr<<"Error: failed to get mul variable value."<<std::endl;
					return;
				}
			}
		}
	}else{
		std::cerr<<"Error: mul does not have a scalar type ("<<(int)var1Type<<", "<<(int)var2Type<<")"<<std::endl;
		return;
	}
}

Func_step_div::Func_step_div(){}
Func_step_div::Func_step_div(const variableIdentifier& arg1, const variableIdentifier& arg2){
	var1 = arg1;
	var2 = arg2;
}

void Func_step_div::apply(){
	uint8_t var1Type = var1.getType(runner);
	uint8_t var2Type = var2.getType(runner);
	if(var1Type == 1 || var2Type == 1){
		uint8_t varType;
		uint8_t scalarVar;
		if(var1Type == 1){
			varType = var2Type;
			scalarVar = 1;
		}else{
			varType = var1Type;
			scalarVar = 2;
		}
		double doubleVar;
		if(scalarVar == 1){
			auto val = var1.getValue<double>(runner);
			if(val){
				doubleVar = val.value();
			}else{
				std::cerr<<"Error: failed to get div scalar value."<<std::endl;
			}
		}else{
			auto val = var2.getValue<double>(runner);
			if(val){
				doubleVar = val.value();
			}else{
				std::cerr<<"Error: failed to get div scalar value."<<std::endl;
			}
		}
		if(varType == 1){
			if(scalarVar == 1){
				auto val = var2.getValue<double>(runner);
				if(val){
					runner->tempScalar = val.value()/doubleVar;
				}else{
					std::cerr<<"Error: failed to get div variable value."<<std::endl;
					return;
				}
			}else{
				auto val = var1.getValue<double>(runner);
				if(val){
					runner->tempScalar = val.value()/doubleVar;
				}else{
					std::cerr<<"Error: failed to get div variable value."<<std::endl;
					return;
				}
			}
		}else if(varType == 2){
			if(scalarVar == 1){
				auto val = var2.getValue<Vector>(runner);
				if(val){
					runner->tempVector = val.value() / doubleVar;
				}else{
					std::cerr<<"Error: failed to get div variable value."<<std::endl;
					return;
				}
			}else{
				auto val = var1.getValue<Vector>(runner);
				if(val){
					runner->tempVector = val.value() / doubleVar;
				}else{
					std::cerr<<"Error: failed to get div variable value."<<std::endl;
					return;
				}
			}
		}else if(varType == 3){
			if(scalarVar == 1){
				auto val = var2.getValue<Matrix>(runner);
				if(val){
					runner->tempMatrix = val.value() / doubleVar;
				}else{
					std::cerr<<"Error: failed to get div variable value."<<std::endl;
					return;
				}
			}else{
				auto val = var1.getValue<Matrix>(runner);
				if(val){
					runner->tempMatrix = val.value() / doubleVar;
				}else{
					std::cerr<<"Error: failed to get div variable value."<<std::endl;
					return;
				}
			}
		}
	}else{
		std::cerr<<"Error: div does not have a scalar type"<<std::endl;
		return;
	}
}

Func_step_createVector::Func_step_createVector(){}
Func_step_createVector::Func_step_createVector(const std::vector<variableIdentifier>& a){
	vars = a;
}

void Func_step_createVector::apply(){
	Vector output;
	for(int i=0;i<vars.size();i++){
		if(vars[i].getType(runner) != 1){
			std::cerr<<"Error: cannot create vector out of non-scalar utilising non-scalar types."<<std::endl;
			return;
		}else{
			auto val = vars[i].getValue<double>(runner);
			if(val){
				output.data.push_back(val.value());
			}else{
				std::cerr<<"Error: failed to get vector creation value."<<std::endl;
				return;
			}
		}
	}
	runner->tempVector = output;
}

Func_step_loadFromTemp::Func_step_loadFromTemp(){}
Func_step_loadFromTemp::Func_step_loadFromTemp(const variableIdentifier& v, const std::string& n){
	var = v;
	name = n;
}

void Func_step_loadFromTemp::apply(){
	uint8_t varType = var.tempType;
	if(varType != 0){
		if(varType == 1){
			runner->setVar<double>(name, runner->tempScalar);
		}else if(varType == 2){
			runner->setVar<Vector>(name, runner->tempVector);
		}else if(varType == 3){
			runner->setVar<Matrix>(name, runner->tempMatrix);
		}
	}else{
		std::cerr<<"Error: cannot load unknown variable.  (name ="<<var.name<<")"<<std::endl;
	}
}

void Func_step_loadToReg::apply(){
	if(val.getType(runner) == 1){
		auto dval = val.getValue<double>(runner);
		if(dval){
			runner->tempScalar = dval.value();
		}else{
			std::cerr<<"Unable to get variable value."<<std::endl;
			return;
		}
	}else if(val.getType(runner) == 2){
		auto vval = val.getValue<Vector>(runner);
		if(vval){
			runner->tempVector = vval.value();
		}else{
			std::cerr<<"Unable to get variable value."<<std::endl;
			return;
		}
	}else if(val.getType(runner) == 3){
		auto mval = val.getValue<Matrix>(runner);
		if(mval){
			runner->tempMatrix = mval.value();
		}else{
			std::cerr<<"Unable to get variable value."<<std::endl;
			return;
		}
	}else{	
		std::cerr<<"Unknown type: "<<(int)val.getType(runner)<<std::endl;
	}
}

Func_step_loadToReg::Func_step_loadToReg(){}
Func_step_loadToReg::Func_step_loadToReg(const variableIdentifier& v){
	val = v;
}

Func_runner::Func_runner(){}

void Func_runner::run(){
	for(unsigned int i=0;i<steps.size();i++){
		steps[i]->apply();
	}
}
