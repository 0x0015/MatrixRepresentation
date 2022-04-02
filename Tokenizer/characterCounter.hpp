#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "getStringAt.hpp"

class characterCounter{
	public:
		int totalC;
		int l;
		int c;
		void count(std::string file){
			
			if(getStringAt(file, totalC) == "\n"){
				c = 1;
				l++;
			}else{
				c++;
			}
			totalC++;
			//std::cout<<getStringAt(file, totalC);
			//std::cout<<totalC;
			//std::cout<<"l";
			//std::cout<<l;
			//std::cout<<"c";
			//std::cout<<c<<std::endl;;
		}
		characterCounter(){
			totalC=0;
			c=1;
			l=1;
		}
};
