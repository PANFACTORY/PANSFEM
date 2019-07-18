//*****************************************************************************
//Title		:PANSFEM/Phenomenon/HeatTransfer/Static2D/HeatTransferStatic2D.cpp
//Author	:Tanabe Yuta
//Date		:2019/06/25
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "HeatTransferStatic2D.h"


PANSFEM::HeatTransferStatic2D::HeatTransferStatic2D() : Equation(){}


PANSFEM::HeatTransferStatic2D::~HeatTransferStatic2D() {}


PANSFEM::HeatTransferStatic2D::HeatTransferStatic2D(Element * _pelement, std::vector<int> _ulist, std::vector<int> _refulist, std::vector<int> _plist, Integration* _pintegration) : Equation(_pelement, _ulist, _refulist, _plist, _pintegration, 2, 1, 0, 1){}


void PANSFEM::HeatTransferStatic2D::SetEquation(){
	//----------�����l�̎擾----------
	double alpha = this->pelement->parameters[this->peq_to_ps[0]];

	//----------[Ke]�}�g���N�X�̔�ϕ��֐����`-----------
	std::function<Eigen::MatrixXd(std::vector<double>)> ke = [&](std::vector<double> _xi) {
		Eigen::MatrixXd B = this->pelement->dTrialdx(this->ueq_to_us, _xi);
		Eigen::MatrixXd ret = alpha * B.transpose() * B * this->pelement->Jacobian(_xi).determinant();
		return ret;
	};

	//----------[Ke]�}�g���b�N�X�����߂�ϕ�----------
	this->Ke = this->pintegration->Integrate(ke);

	//----------���Ȕ��M�x�N�g�������߂�----------
	Eigen::VectorXd G = Eigen::VectorXd::Zero(1);	//���Ȕ��M�����Ƃ���

	//----------{F}�x�N�g���̔�ϕ��֐����`----------
	std::function<Eigen::VectorXd(std::vector<double>)> fe = [&](std::vector<double> _xi) {
		Eigen::VectorXd ret = this->pelement->Test(this->ueq_to_us, _xi).transpose() * G;
		return ret;
	};

	//----------{F}�x�N�g�������߂�----------
	this->Fe = this->pintegration->Integrate(fe);
}