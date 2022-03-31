#pragma once
#include <iostream>
#include <vector>

class Matrix{
public:
	std::vector<std::vector<double>> data;
	Matrix(std::vector<std::vector<double>> d){
		data = d;
	}
	Matrix(){
		
	}
	void print(){
		for(auto& o : data){
			for(int i=0;i<o.size();i++){
				std::cout<<o[i]<<" ";
			}
			std::cout<<std::endl;
		}
	}
	Matrix(unsigned int m, unsigned int n){
		std::vector<double> l;
		l.resize(m, 0);
		for(int i=0;i<n;i++){
			data.push_back(l);
		}
	}
	Matrix transpose(){
		Matrix out(data.size(), data[0].size());
		for(unsigned int i=0;i<data.size();i++){
			for(unsigned int j=0;j<data[i].size();j++){
				out.data[j][i] = data[i][j];
			}
		}
		return(out);
	}
	bool operator==(const Matrix& o){
		return(data == o.data);
	}
	std::vector<double>& operator[](unsigned int i){
		return(data[i]);
	}
	Matrix operator*(double c){
		Matrix o;
		o.data = data;
		for(int i=0;i<o.data.size();i++){
			for(int j=0;j<o.data[i].size();j++){
				o.data[i][j]*=c;
			}
		}
		return(o);
	}
	Matrix operator/(double c){
		Matrix o;
		o.data = data;
		for(int i=0;i<o.data.size();i++){
			for(int j=0;j<o.data[i].size();j++){
				o.data[i][j]/=c;
			}
		}
		return(o);
	}
	Matrix operator+(const Matrix& b){
		if(data.size() != b.data.size()){
			return(Matrix());
		}
		Matrix o;
		o.data = data;
		for(int i=0;i<o.data.size();i++){
			if(o.data[i].size() != b.data[i].size()){
				return(Matrix());
			}
			for(int j=0;j<o.data[i].size();j++){
				o.data[i][j] += b.data[i][j];
			}
		}
		return(o);
	}
	Matrix operator-(const Matrix& b){
		if(data.size() != b.data.size()){
			return(Matrix());
		}
		Matrix o;
		o.data = data;
		for(int i=0;i<o.data.size();i++){
			if(o.data[i].size() != b.data[i].size()){
				return(Matrix());
			}
			for(int j=0;j<o.data[i].size();j++){
				o.data[i][j] -= b.data[i][j];
			}
		}
		return(o);
	}
	bool operator!=(const Matrix& o){
		return(data != o.data);
	}
};
