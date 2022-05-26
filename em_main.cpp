#include <iostream>
#include "Spaces/Rn.hpp"
#include "Spaces/MnxmR.hpp"
#include "Transformation.hpp"
#include "Parse.hpp"

#include "WebCpp-Interaction-Lib/DOM.hpp"

void runMR(){
	std::shared_ptr<Element> input = Element::getByClassName("MR_input")[0];
	std::shared_ptr<Element> output = Element::getByClassName("MR_output")[0];
	std::string eqStr = input->dom_value;
	std::cout<<"Computing Matrix Representation of: "<<eqStr<<std::endl;
	Parser p4;
	auto p4_tr = p4.parseFromStr(eqStr);
	const std::string& eqOpr = p4_tr->getMatRep().toString();
	output->dom_innerHTML = (std::string)eqOpr;
}

int main(int argc, char** argv) {
	std::shared_ptr<Element> button = Element::getByClassName("MR_button")[0];
	button->dom_onclick = runMR;
}
