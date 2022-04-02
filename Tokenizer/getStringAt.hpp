#include <iostream>
#include <string>
#pragma once

using namespace std;

string getStringAt(string data, size_t index){//small utility function
	return(data.substr(index, 1));
}
