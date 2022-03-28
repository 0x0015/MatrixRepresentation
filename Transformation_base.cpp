#include "Transformation_base.hpp"

bool Trs_base::isLinear(){
	return(false);
}

Matrix Trs_base::getMatRep(){
	return(Matrix());
}

Trs_base::Trs_base(){}
