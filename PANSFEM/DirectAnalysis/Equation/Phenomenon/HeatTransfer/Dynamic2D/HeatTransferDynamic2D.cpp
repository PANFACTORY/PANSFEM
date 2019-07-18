//*****************************************************************************
//Title		:PANSFEM/Phenomenon/HeatTransfer/Dynamic2D/HeatTransferDynamic2D.cpp
//Author	:Tanabe Yuta
//Date		:2019/06/29
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "HeatTransferDynamic2D.h"


PANSFEM::HeatTransferDynamic2D::HeatTransferDynamic2D() : Equation(){}


PANSFEM::HeatTransferDynamic2D::~HeatTransferDynamic2D(){}


PANSFEM::HeatTransferDynamic2D::HeatTransferDynamic2D(Element * _pelement, std::vector<int> _ulist, std::vector<int> _refulist, std::vector<int> _plist, Integration * _pintegration) : Equation(_pelement, _ulist, _refulist, _plist, _pintegration, 2, 1, 0, 5){}


void PANSFEM::HeatTransferDynamic2D::SetEquation(){
	//----------�����l�̎擾----------
	double rho = this->pelement->parameters[this->peq_to_ps[0]];
	double c = this->pelement->parameters[this->peq_to_ps[1]];
	double k = this->pelement->parameters[this->peq_to_ps[2]];
	double dt = this->pelement->parameters[this->peq_to_ps[3]];
	double theta = this->pelement->parameters[this->peq_to_ps[4]];

	//----------[Ke]�}�g���b�N�X�̔�ϕ��֐����`----------
	std::function<Eigen::MatrixXd(std::vector<double>)> ke = [&](std::vector<double> _xi) {
		//----------[C]���v�Z----------
		Eigen::MatrixXd N = this->pelement->Trial(this->ueq_to_us, _xi);
		Eigen::MatrixXd C = rho * c*N.transpose()*N;

		//----------[K]���v�Z----------
		Eigen::MatrixXd dN = this->pelement->dTrialdx(this->ueq_to_us, _xi);
		Eigen::MatrixXd K = k * dN.transpose()*dN;

		Eigen::MatrixXd ret = (C + theta*K*dt)*this->pelement->Jacobian(_xi).determinant();
		return ret;
	};
	
	//----------[Ke]�}�g���b�N�X�����߂�ϕ�----------
	this->Ke = this->pintegration->Integrate(ke);

	//----------{F}�x�N�g���̔�ϕ��֐����`----------
	std::function<Eigen::VectorXd(std::vector<double>)> fe = [&](std::vector<double> _xi) {
		//----------[C]���v�Z----------
		Eigen::MatrixXd N = this->pelement->Trial(this->ueq_to_us, _xi);
		Eigen::MatrixXd C = rho*c*N.transpose()*N;

		//----------[K]���v�Z----------
		Eigen::MatrixXd dN = this->pelement->dTrialdx(this->ueq_to_us, _xi);
		Eigen::MatrixXd K = k*dN.transpose()*dN;

		//----------���񎩌Ȕ��M�͍l���Ȃ�----------

		Eigen::VectorXd ret = (C - (1.0 - theta)*K*dt) * this->pelement->u(this->ueq_to_us) *this->pelement->Jacobian(_xi).determinant();
		return ret;
	};

	//----------{F}�x�N�g�������߂�----------
	this->Fe = this->pintegration->Integrate(fe);
}