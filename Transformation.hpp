#pragma once
#include <iostream>
#include <functional>
#include <type_traits>

template<class V, class W> class Transformation{
public:
	std::function<typename W::element(typename V::element)> func;
	V vval;
	W wval;
	bool isLinear(){
		if(func){
			return(true);
		}
		return(false);
	}
	Matrix getMatRep(){
		if(!isLinear()){
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