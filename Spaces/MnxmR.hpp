#pragma once
#include "../Space.hpp"
#include "../Datatypes/Matrix.hpp"
#include <array>

template <unsigned int m, unsigned int n> class MatR : public Space{
public:
	typedef Matrix element;
	unsigned int Dim(){
		return(m*n);
	}
	std::array<Matrix, m*n> Basis;
	MatR(){
		for(int i=0;i<m*n;i++){
			Matrix elem(n, m);
			unsigned int c = 0;
			for(int j=0;j<m;j++){
				std::vector<double> r;
				r.resize(n);
				for(int x=0;x<n;x++){
					if(c == i){
						r[x] = 1;
					}else{
						r[x] = 0;
					}
					c++;
				}
				elem.data[j] = r;
			}
			Basis[i] = Matrix(elem);
		}
	}
	std::array<std::pair<unsigned int, double>, m*n> Decompose(Matrix& vec){
		std::array<std::pair<unsigned int, double>, m*n> output;
		unsigned int counter = 0;
		for(auto& i : vec.data){
			for(auto& o : i){
				output[counter] = std::pair<unsigned int, double>(counter, o);
				counter++;
			}
		}
		return(output);
	}
};