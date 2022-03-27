#pragma once
#include <iostream>
#include <functional>
#include <type_traits>

template<class V, class W> class Transformation{
public:
	std::function<typename W::element(typename V::element)> func;
	V vval;
	W wval;
	bool isLinear(){//very rough.  just checks some common ways that it could fail.  does not use a CAS, just checks values.
		if(!func){
			return(false);
		}
		for(auto& a : vval.Basis){
			for(auto& b : vval.Basis){
				if(func(a+b) != func(a) + func(b)){
					return(false);
				}
			}
		}
		std::vector<double> interestingScalars = {-1, 0, 1, 2, 10, 3.14, 100, 1000, 2048, 100000};
		for(auto& a : vval.Basis){
			for(auto& c : interestingScalars){
				if(func(a*c) != func(a) * c){
					return(false);
				}
			}
		}
		return(true);
	}
	Matrix getMatRep(){
		if(!isLinear()){
			std::cout<<"Error:  transformation is not linear."<<std::endl;
			return(Matrix(0,0));
		}
		Matrix output;
		std::vector<typename W::element> T;
		for(auto& i : vval.Basis){
			T.push_back(func(i));
		}
		for(auto& o : T){
			auto t = wval.Decompose(o);
			std::vector<double> d;
			d.resize(wval.Basis.size());
			for(auto& n : t){
				d[n.first] = n.second;
			}
			output.data.push_back(d);
		}
		return(output.transpose());
	}
	Transformation(std::function<typename W::element(typename V::element)> f){
		if(f){
			func = f;
		}
	}
};
