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

Vector t2(Vector n){
	Vector d;
	d.data = n.data;
	for(auto& i : d.data){
		i*=2;
	}
	return(d);
}

Vector ex(Vector n){
	Vector out;
	out.data = {n[0]+3*n[1], 0, 2*n[0]-4*n[1]};
	return(out);
}

Matrix tran(Matrix m){
	return(m.transpose());
}

int main() {
	Transformation<Real<2>, Real<3>> tr(ex);
	tr.vval.Basis[0].print();
	Matrix rep = tr.getMatRep();
	//rep.print();
	
	Transformation<MatR<3,3>, MatR<3,3>> tr2(tran);
	Matrix rep2 = tr2.getMatRep();
	rep2.print();
}