#include <iostream>
#include "Spaces/Rn.hpp"
#include "Spaces/MnxmR.hpp"
#include "Transformation.hpp"

Vector identity(Vector n){
	return(n);
}

Vector zero(Vector n){
	return(Vector(n.data.size()));
}

Vector ex(Vector n){
	Vector out;
	out.data = {n[0]+3*n[1], 0, 2*n[0]-4*n[1]};
	return(out);
}

Matrix tran(Matrix m){
	return(m.transpose());
}

Vector sq(Vector n){
	Vector out(n.data);
	for(auto& i : out.data){
		i = i*i;
	}
	return(out);
}

int main() {
	std::cout<<"R^3 Identity transformation:"<<std::endl;
	Transformation<Real<3>, Real<3>> id_tr(identity);
	id_tr.getMatRep().print();
	std::cout<<"Example from class: T:R^2->R^3 T(a1, a2) = (a1+3*a2, 0, 2a1-4a2)"<<std::endl;
	Transformation<Real<2>, Real<3>> tr(ex);
	tr.getMatRep().print();
	std::cout<<"Mat 3x3 transpose:"<<std::endl;
	Transformation<MatR<3,3>, MatR<3,3>> tr2(tran);
	tr2.getMatRep().print();
	std::cout<<"T:R->R T(x)=x^2 (note: not linear)"<<std::endl;
	Transformation<Real<1>, Real<1>> sq_tr(sq);
	sq_tr.getMatRep().print();
}
