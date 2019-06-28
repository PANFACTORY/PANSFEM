//*****************************************************************************
//Title		:PANSFEM/Phenomenon/Structure/PlaneStrain/PlaneStrain.cpp
//Author	:Tanabe Yuta
//Date		:2019/06/10
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "PlaneStrain.h"


PANSFEM::PlaneStrain::PlaneStrain() : Equation(){}


PANSFEM::PlaneStrain::~PlaneStrain(){}


PANSFEM::PlaneStrain::PlaneStrain(Element * _pelement, std::vector<int> _ulist, std::vector<double> _parameters, Integration* _pintegration) : Equation(_pelement, _ulist, _parameters, _pintegration, 2, 2, 3){
	this->E = _parameters[0];
	this->V = _parameters[1];
	this->t = _parameters[2];
}


Eigen::MatrixXd PANSFEM::PlaneStrain::GetKe(std::vector<double> _xi){
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


Eigen::VectorXd PANSFEM::PlaneStrain::GetFe(std::vector<double> _xi) {
	Eigen::VectorXd G = Eigen::VectorXd::Zero(2);	//Ž©d–³‚µ‚Æ‚·‚é
	return this->pelement->Test(this->ueq_to_us, { 0.0, 0.0 }).transpose() * G;
}