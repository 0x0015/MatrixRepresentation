#pragma once
#include <iostream>
#include <vector>

class Vector{
public:
	std::vector<double> data;
	Vector(std::vector<double> d){
		data = d;
	}
	Vector(){
		
	}
	Vector(unsigned int n){
		data.resize(n, 0);
	}
	void print(){
		std::cout<<"(";
		for(int i=0;i<data.size();i++){
			std::cout<<data[i];
			if(i+1<data.size()){
				std::cout<<", ";
			}
		}
		std::cout<<")"<<std::endl;
	}
	bool operator==(const Vector& o){
		return(data == o.data);
	}
	bool operator!=(const Vector& o){
		return(data != o.data);
	}
	double operator[](unsigned int i){
		return(data[i]);
	}
	Vector operator*(double c){
		Vector o;
		o.data = data;
		for(int i=0;i<data.size();i++){
			o.data[i] *= c;
		}
		return(o);
	}
	Vector operator/(double c){
		Vector o;
		o.data = data;
		for(int i=0;i<data.size();i++){
			o.data[i] /= c;
		}
		return(o);
	}
	Vector operator+(const Vector& b){
		if(b.data.size() != data.size()){
			return(Vector());
		}
		Vector o;
		o.data = data;
		for(int i=0;i<o.data.size();i++){
			o.data[i] += b.data[i];
		}
		return(o);
	}
	Vector operator-(const Vector& b){
		if(b.data.size() != data.size()){
			return(Vector());
		}
		Vector o;
		o.data = data;
		for(int i=0;i<o.data.size();i++){
			o.data[i] -= b.data[i];
		}
		return(o);
	}
};
