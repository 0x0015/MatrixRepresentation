#pragma once
#include <iostream>
#include <vector>
#include "Datatypes/Matrix.hpp"

class Trs_base{
public:
	virtual bool isLinear();
	virtual Matrix getMatRep();
	Trs_base();
};
