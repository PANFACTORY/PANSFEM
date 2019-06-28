//*****************************************************************************
//Title		:PANSFEM/Phenomenon/HeatTransfer/Static2D/HeatTransferStatic2D.cpp
//Author	:Tanabe Yuta
//Date		:2019/06/25
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "HeatTransferStatic2D.h"


PANSFEM::HeatTransferStatic2D::HeatTransferStatic2D() : Equation(){}


PANSFEM::HeatTransferStatic2D::~HeatTransferStatic2D() {}


PANSFEM::HeatTransferStatic2D::HeatTransferStatic2D(Element * _pelement, std::vector<int> _ulist, std::vector<int> _plist, Integration* _pintegration) : Equation(_pelement, _ulist, _plist, _pintegration, 2, 1, 1){
	this->alpha = this->pelement->parameters[this->peq_to_ps[0]];
}


Eigen::MatrixXd PANSFEM::HeatTransferStatic2D::GetKe(std::vector<double> _xi){
	Eigen::MatrixXd B = this->pelement->dTrialdx(this->ueq_to_us, _xi);
	return this->alpha * B.transpose() * B * this->pelement->Jacobian(_xi).determinant();
}


Eigen::VectorXd PANSFEM::HeatTransferStatic2D::GetFe(std::vector<double> _xi){
	Eigen::VectorXd G = Eigen::VectorXd::Zero(1);	//Ž©ŒÈ”­”M–³‚µ‚Æ‚·‚é
	return this->pelement->Test(this->ueq_to_us, { 0.0, 0.0 }).transpose() * G;
}