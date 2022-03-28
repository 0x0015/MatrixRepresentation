#pragma once
#include "../Space.hpp"
#include "../Datatypes/Vector.hpp"
#include <vector>

class Real : public Space{
public:
	typedef Vector element;
	unsigned int n;
	unsigned int Dim(){
		return(n);
	}
	std::vector<Vector> Basis;
	Real(unsigned int _n){
		n = _n;
		Basis.resize(n);
		for(unsigned int i=0;i<n;i++){
			std::vector<double> elem;
			elem.resize(n);
			for(unsigned int j=0;j<n;j++){
				if(j == i){
					elem[j] = 1;
				}else{
					elem[j] = 0;
				}
			}
			Basis[i] = Vector(elem);
		}
	}
	Real(){}
	std::vector<std::pair<unsigned int, double>> Decompose(Vector& vec){
		std::vector<std::pair<unsigned int, double>> output;
		output.resize(n);
		for(unsigned int i=0;i<n;i++){
			std::pair<unsigned int, double> t;
			t.first = i;
			t.second = vec.data[i];
			output[i] = t;
		}
		return(output);
	}
};
