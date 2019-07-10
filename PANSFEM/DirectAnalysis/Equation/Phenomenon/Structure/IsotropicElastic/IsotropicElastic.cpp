//*****************************************************************************
//Title		:PANSFEM/Phenomenon/Structure/IsotropicElastic/IsotropicElastic.cpp
//Author	:Tanabe Yuta
//Date		:2019/07/10
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "IsotropicElastic.h"


PANSFEM::IsotropicElastic::IsotropicElastic() : Equation(){}


PANSFEM::IsotropicElastic::~IsotropicElastic(){}


PANSFEM::IsotropicElastic::IsotropicElastic(Element * _pelement, std::vector<int> _ulist, std::vector<int> _refulist, std::vector<int> _plist, Integration * _pintegration) : Equation(_pelement, _ulist, _refulist, _plist, _pintegration, 3, 3, 0, 2){
	this->E = this->pelement->parameters[this->peq_to_ps[0]];
	this->V = this->pelement->parameters[this->peq_to_ps[1]];
}


Eigen::MatrixXd PANSFEM::IsotropicElastic::GetKe(std::vector<double> _xi){
	Eigen::MatrixXd D = Eigen::MatrixXd(6, 6);
	double coef = this->E / ((1.0 - 2.0*this->V)*(1.0 + this->V));
	D(0, 0) = 1.0 - this->V;	D(0, 1) = this->V;			D(0, 2) = 0.0;
	D(1, 0) = this->V;			D(1, 1) = 1.0 - this->V;	D(1, 2) = 0.0;
	D(2, 0) = 0.0;				D(2, 1) = 0.0;				D(2, 2) = (1.0 - 2.0*this->V) / 2.0;
	D *= coef;

	Eigen::MatrixXd A = Eigen::MatrixXd(6, 9);
	A(0, 0) = 1.0;	A(0, 1) = 0.0;	A(0, 2) = 0.0;	A(0, 3) = 0.0;
	A(1, 0) = 0.0;	A(1, 1) = 0.0;	A(1, 2) = 0.0;	A(1, 3) = 1.0;
	A(2, 0) = 0.0;	A(2, 1) = 1.0;	A(2, 2) = 1.0;	A(2, 3) = 0.0;

	Eigen::MatrixXd B = A * this->pelement->dTrialdx(this->ueq_to_us, _xi);
	return B.transpose() * D * B * this->pelement->Jacobian(_xi).determinant();
}


Eigen::VectorXd PANSFEM::IsotropicElastic::GetFe(std::vector<double> _xi){
	Eigen::VectorXd G = Eigen::VectorXd::Zero(3);	//Ž©d–³‚µ‚Æ‚·‚é
	return this->pelement->Test(this->ueq_to_us, _xi).transpose() * G;
}