#pragma once
#include "../Space.hpp"
#include "../Datatypes/Matrix.hpp"
#include <vector>

class MatR : public Space{
public:
	typedef Matrix element;
	unsigned int m;
	unsigned int n;
	unsigned int Dim(){
		return(m*n);
	}
	std::vector<Matrix> Basis;
	MatR(unsigned int _m, unsigned int _n){
		m = _m;
		n = _n;
		Basis.resize(m*n);
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
	MatR(){}
	std::vector<std::pair<unsigned int, double>> Decompose(Matrix& vec){
		std::vector<std::pair<unsigned int, double>> output;
		output.resize(m*n);
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
