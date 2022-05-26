#include <iostream>
#include "Spaces/Rn.hpp"
#include "Spaces/MnxmR.hpp"
#include "Transformation.hpp"
#include "Parse.hpp"

int main(int argc, char** argv) {
	std::string eqStr = "";
	for(int i=1;i<argc;i++){
		eqStr += argv[i];
	}
	if(eqStr == ""){
		std::cout<<"Please specify an equation.  examples: \n\t\"T:R^3->R^3 T(x)=2*x\"\n\t\"T:M3x3->M3x3 T(x)=tr x\""<<std::endl;
	}
	Parser p4;
	auto p4_tr = p4.parseFromStr(eqStr);
	p4_tr->getMatRep().print();
}
