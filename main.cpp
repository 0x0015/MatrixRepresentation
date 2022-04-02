#include <iostream>
#include "Spaces/Rn.hpp"
#include "Spaces/MnxmR.hpp"
#include "Transformation.hpp"
#include "Parse.hpp"

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
	Real r3(3);
	std::cout<<"R^3 Identity transformation:"<<std::endl;
	Transformation<Real, Real> id_tr(r3, r3, identity);
	id_tr.getMatRep().print();
	std::cout<<"Example from class: T:R^2->R^3 T(a1, a2) = (a1+3*a2, 0, 2a1-4a2)"<<std::endl;
	Real r2(2);
	Transformation<Real, Real> tr(r2, r3, ex);
	tr.getMatRep().print();
	std::cout<<"Mat 3x3 transpose:"<<std::endl;
	MatR mat3x3(3, 3);
	Transformation<MatR, MatR> tr2(mat3x3, mat3x3,tran);
	tr2.getMatRep().print();
	std::cout<<"T:R->R T(x)=x^2 (note: not linear)"<<std::endl;
	Real r1(1);
	Transformation<Real, Real> sq_tr(r1, r1, sq);
	sq_tr.getMatRep().print();
	std::string custom_tr = "T:R^3->R^3 T(x)=2*x";
	std::cout<<"Custom transformation: "<<custom_tr<<std::endl;
	Parser p;
	auto p_tr = p.parseFromStr(custom_tr);
	//std::static_pointer_cast<Transformation<Real, Real>>(p_tr)->func(Vector({1,2,3})).print();
	//std::static_pointer_cast<Transformation<Real, Real>>(p_tr)->func(Vector({0,-1,2})).print();
	p_tr->getMatRep().print();
}
