//***************************************************************************************
//Title		:PANSFEM/Phenomenon/Structure/PlaneStrainWithHeat/PlaneStrainWithHeat.h
//Author	:Tanabe Yuta
//Date		:2019/06/29
//Copyright	:(C)2019 TanabeYuta
//***************************************************************************************


#include "PlaneStrainWithHeat.h"


PANSFEM::PlaneStrainWithHeat::PlaneStrainWithHeat() : Equation(){}


PANSFEM::PlaneStrainWithHeat::~PlaneStrainWithHeat(){}


PANSFEM::PlaneStrainWithHeat::PlaneStrainWithHeat(Element * _pelement, std::vector<int> _ulist, std::vector<int> _refulist, std::vector<int> _plist, Integration * _pintegration)
	: Equation(_pelement, _ulist, _refulist, _plist, _pintegration, 2, 2, 1, 4){
	this->E = this->pelement->parameters[this->peq_to_ps[0]];
	this->V = this->pelement->parameters[this->peq_to_ps[1]];
	this->t = this->pelement->parameters[this->peq_to_ps[2]];
	this->alpha = this->pelement->parameters[_plist[3]];
}


Eigen::MatrixXd PANSFEM::PlaneStrainWithHeat::GetKe(std::vector<double> _xi){
	Eigen::MatrixXd D = Eigen::MatrixXd(3, 3);
	double coef = this->E / ((1.0 - 2.0*this->V)*(1.0 + this->V));
	D(0, 0) = 1.0 - this->V;	D(0, 1) = this->V;			D(0, 2) = 0.0;
	D(1, 0) = this->V;			D(1, 1) = 1.0 - this->V;	D(1, 2) = 0.0;
	D(2, 0) = 0.0;				D(2, 1) = 0.0;				D(2, 2) = (1.0 - 2.0*this->V) / 2.0;
	D *= coef;

	Eigen::MatrixXd A = Eigen::MatrixXd(3, 4);
	A(0, 0) = 1.0;	A(0, 1) = 0.0;	A(0, 2) = 0.0;	A(0, 3) = 0.0;
	A(1, 0) = 0.0;	A(1, 1) = 0.0;	A(1, 2) = 0.0;	A(1, 3) = 1.0;
	A(2, 0) = 0.0;	A(2, 1) = 1.0;	A(2, 2) = 1.0;	A(2, 3) = 0.0;

	Eigen::MatrixXd B = A * this->pelement->dTrialdx(this->ueq_to_us, _xi);
	return B.transpose() * D * B * this->t * this->pelement->Jacobian(_xi).determinant();
}


Eigen::VectorXd PANSFEM::PlaneStrainWithHeat::GetFe(std::vector<double> _xi){
	//----------[D]----------
	Eigen::MatrixXd D = Eigen::MatrixXd(3, 3);
	double coef = this->E / ((1.0 - 2.0*this->V)*(1.0 + this->V));
	D(0, 0) = 1.0 - this->V;	D(0, 1) = this->V;			D(0, 2) = 0.0;
	D(1, 0) = this->V;			D(1, 1) = 1.0 - this->V;	D(1, 2) = 0.0;
	D(2, 0) = 0.0;				D(2, 1) = 0.0;				D(2, 2) = (1.0 - 2.0*this->V) / 2.0;
	D *= coef;

	//----------[B]----------
	Eigen::MatrixXd A = Eigen::MatrixXd(3, 4);
	A(0, 0) = 1.0;	A(0, 1) = 0.0;	A(0, 2) = 0.0;	A(0, 3) = 0.0;
	A(1, 0) = 0.0;	A(1, 1) = 0.0;	A(1, 2) = 0.0;	A(1, 3) = 1.0;
	A(2, 0) = 0.0;	A(2, 1) = 1.0;	A(2, 2) = 1.0;	A(2, 3) = 0.0;
	Eigen::MatrixXd B = A * this->pelement->dTrialdx(this->ueq_to_us, _xi);

	//----------{ƒÃT}----------
	Eigen::VectorXd EPST = Eigen::VectorXd(3);
	EPST(0) = 1.0;	EPST(1) = 1.0;	EPST(2) = 0.0;
	EPST *= this->alpha * this->pelement->Trial(this->refueq_to_us, _xi) * this->pelement->u(this->refueq_to_us);

	std::cout << EPST << std::endl;

	return B.transpose() * D * EPST * this->t * this->pelement->Jacobian(_xi).determinant();
}


