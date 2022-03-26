#pragma once
#include "../Space.hpp"
#include "../Datatypes/Vector.hpp"
#include <array>

template <unsigned int n> class Real : public Space{
public:
	typedef Vector element;
	unsigned int Dim(){
		return(n);
	}
	std::array<Vector, n> Basis;
	Real(){
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
	std::array<std::pair<unsigned int, double>, n> Decompose(Vector& vec){
		std::array<std::pair<unsigned int, double>, n> output;
		for(unsigned int i=0;i<n;i++){
			std::pair<unsigned int, double> t;
			t.first = i;
			t.second = vec.data[i];
			output[i] = t;
		}
		return(output);
	}
};