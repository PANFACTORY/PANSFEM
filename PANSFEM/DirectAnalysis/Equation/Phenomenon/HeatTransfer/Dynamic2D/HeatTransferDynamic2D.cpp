//*****************************************************************************
//Title		:PANSFEM/Phenomenon/HeatTransfer/Dynamic2D/HeatTransferDynamic2D.cpp
//Author	:Tanabe Yuta
//Date		:2019/06/29
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "HeatTransferDynamic2D.h"


PANSFEM::HeatTransferDynamic2D::HeatTransferDynamic2D() : Equation(){}


PANSFEM::HeatTransferDynamic2D::~HeatTransferDynamic2D(){}


PANSFEM::HeatTransferDynamic2D::HeatTransferDynamic2D(Element * _pelement, std::vector<int> _ulist, std::vector<int> _refulist, std::vector<int> _plist, Integration * _pintegration) : Equation(_pelement, _ulist, _refulist, _plist, _pintegration, 2, 1, 0, 5){
	this->rho = this->pelement->parameters[this->peq_to_ps[0]];
	this->c = this->pelement->parameters[this->peq_to_ps[1]];
	this->k = this->pelement->parameters[this->peq_to_ps[2]];
	this->dt = this->pelement->parameters[this->peq_to_ps[3]];
	this->theta = this->pelement->parameters[this->peq_to_ps[4]];
}


Eigen::MatrixXd PANSFEM::HeatTransferDynamic2D::GetKe(std::vector<double> _xi){
	//----------[C]‚ðŒvŽZ----------
	Eigen::MatrixXd N = this->pelement->Trial(this->ueq_to_us, _xi);
	Eigen::MatrixXd C = this->rho*this->c*N.transpose()*N;

	//----------[K]‚ðŒvŽZ----------
	Eigen::MatrixXd dN = this->pelement->dTrialdx(this->ueq_to_us, _xi);
	Eigen::MatrixXd K = this->k*dN.transpose()*dN;

	return (C + this->theta*K*this->dt)*this->pelement->Jacobian(_xi).determinant();
}


Eigen::VectorXd PANSFEM::HeatTransferDynamic2D::GetFe(std::vector<double> _xi){
	//----------[C]‚ðŒvŽZ----------
	Eigen::MatrixXd N = this->pelement->Trial(this->ueq_to_us, _xi);
	Eigen::MatrixXd C = this->rho*this->c*N.transpose()*N;

	//----------[K]‚ðŒvŽZ----------
	Eigen::MatrixXd dN = this->pelement->dTrialdx(this->ueq_to_us, _xi);
	Eigen::MatrixXd K = this->k*dN.transpose()*dN;

	//----------¡‰ñŽ©ŒÈ”­”M‚Íl‚¦‚È‚¢----------

	return (C - (1.0 - this->theta)*K*this->dt) * this->pelement->u(this->ueq_to_us) *this->pelement->Jacobian(_xi).determinant();
}