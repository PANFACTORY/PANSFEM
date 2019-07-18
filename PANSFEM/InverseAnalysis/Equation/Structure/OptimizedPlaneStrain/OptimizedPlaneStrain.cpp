//***********************************************************************************************************
//Title		:PANSFEM/InverseAnalysis/Equation/Structure/OptimizedPlaneStrain/OptimizedPlaneStrain.cpp
//Author	:Tanabe Yuta
//Date		:2019/07/07
//Copyright	:(C)2019 TanabeYuta
//***********************************************************************************************************


#include "OptimizedPlaneStrain.h"


PANSFEM::OptimizedPlaneStrain::OptimizedPlaneStrain() : Equation(){}


PANSFEM::OptimizedPlaneStrain::~OptimizedPlaneStrain(){}


PANSFEM::OptimizedPlaneStrain::OptimizedPlaneStrain(Element * _pelement, std::vector<int> _ulist, std::vector<int> _refulist, std::vector<int> _plist, Integration * _pintegration) : Equation(_pelement, _ulist, _refulist, _plist, _pintegration, 2, 2, 0, 5){}


void PANSFEM::OptimizedPlaneStrain::SetEquation(){
	//----------�p�����[�^�̎擾----------
	double rho = this->pelement->parameters[this->peq_to_ps[0]];
	double Emax = this->pelement->parameters[this->peq_to_ps[1]];
	double Emin = this->pelement->parameters[this->peq_to_ps[2]];
	double V = this->pelement->parameters[this->peq_to_ps[3]];
	double t = this->pelement->parameters[this->peq_to_ps[4]];

	//----------[D]�̐���----------
	double E = (Emax - Emin)*pow(rho, 3.0) + Emin;
	Eigen::MatrixXd D = Eigen::MatrixXd(3, 3);
	D(0, 0) = 1.0 - V;	D(0, 1) = V;		D(0, 2) = 0.0;
	D(1, 0) = V;		D(1, 1) = 1.0 - V;	D(1, 2) = 0.0;
	D(2, 0) = 0.0;		D(2, 1) = 0.0;		D(2, 2) = (1.0 - 2.0*V) / 2.0;
	D *= E / ((1.0 - 2.0*V)*(1.0 + V));

	//----------�A�_�v�^�s��̐���----------
	Eigen::MatrixXd A = Eigen::MatrixXd(3, 4);
	A(0, 0) = 1.0;	A(0, 1) = 0.0;	A(0, 2) = 0.0;	A(0, 3) = 0.0;
	A(1, 0) = 0.0;	A(1, 1) = 0.0;	A(1, 2) = 0.0;	A(1, 3) = 1.0;
	A(2, 0) = 0.0;	A(2, 1) = 1.0;	A(2, 2) = 1.0;	A(2, 3) = 0.0;

	//----------[Ke]�}�g���b�N�X�̔�ϕ��֐����`----------
	std::function<Eigen::MatrixXd(std::vector<double>)> ke = [&](std::vector<double> _xi) {
		Eigen::MatrixXd B = A * this->pelement->dTrialdx(this->ueq_to_us, _xi);
		Eigen::MatrixXd ret = B.transpose() * D * B * this->pelement->Jacobian(_xi).determinant() * t;
		return ret;
	};

	//----------[Ke]�}�g���b�N�X�����߂�ϕ�----------
	this->Ke = this->pintegration->Integrate(ke);

	//----------�̐ϗ̓x�N�g�������߂�----------
	Eigen::VectorXd G = Eigen::VectorXd::Zero(2);	//���d�����Ƃ���

	//----------{F}�x�N�g���̔�ϕ��֐����`----------
	std::function<Eigen::VectorXd(std::vector<double>)> fe = [&](std::vector<double> _xi) {
		Eigen::VectorXd ret = this->pelement->Test(this->ueq_to_us, _xi).transpose() * G;
		return ret;
	};

	//----------{F}�x�N�g�������߂�----------
	this->Fe = this->pintegration->Integrate(fe);
}