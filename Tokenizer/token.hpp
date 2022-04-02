#pragma once
#include <iostream>
#include <string>
#include <vector>

class token{
	public:
		std::string filename;
		int c;
		int l;
		std::string value;
		token(std::string v, std::string f, int ch, int li){
			value = v;
			filename = f;
			c=ch;
			l=li;
		}
		token(std::string v){
			value = v;
		}
};
