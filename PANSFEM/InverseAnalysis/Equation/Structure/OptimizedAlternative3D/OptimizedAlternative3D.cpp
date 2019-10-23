//*************************************************************************************************
//Title		:PANSFEM/InversedAnalysis/Structure/OptimizedAlternative3D/OptimizedAltanative3D.cpp
//Author	:Tanabe Yuta
//Date		:2019/09/27
//Copyright	:(C)2019 TanabeYuta
//*************************************************************************************************


#include "OptimizedAlternative3D.h"


PANSFEM::OptimizedAlternative3D::OptimizedAlternative3D() : Equation() {}


PANSFEM::OptimizedAlternative3D::~OptimizedAlternative3D(){}


PANSFEM::OptimizedAlternative3D::OptimizedAlternative3D(Element * _pelement, std::vector<int> _ulist, std::vector<int> _refulist, std::vector<int> _plist, Integration * _pintegration) : Equation(_pelement, _ulist, _refulist, _plist, _pintegration, 3, 3, 0, 5) {}


void PANSFEM::OptimizedAlternative3D::SetEquation() {
	//----------�p�����[�^�̎擾----------
	double s = this->pelement->pparameter->parameters[this->peq_to_ps[0]];
	double dmax = this->pelement->pparameter->parameters[this->peq_to_ps[1]];
	double dmin = this->pelement->pparameter->parameters[this->peq_to_ps[2]];
	double L = this->pelement->pparameter->parameters[this->peq_to_ps[3]];
	double E = this->pelement->pparameter->parameters[this->peq_to_ps[4]];

	double d = (dmax - dmin)*s + dmin;
	double dl = d / L;
	double Eeq = 0.5*sqrt(3.0)*3.141592*E*pow(dl, 4.0);
	double Veq = 0.499;

	double Dii = Eeq * (1.0 - Veq) / ((1.0 - 2.0*Veq)*(1.0 + Veq));
	double Dij = Eeq * Veq / ((1.0 - 2.0*Veq)*(1.0 + Veq));
	double Gii = (5.0*3.141592*E / (8.0*sqrt(3.0)))*pow(dl, 4.0);

	//----------[D]�̐���----------
	Eigen::MatrixXd D = Eigen::MatrixXd(6, 6);
	D(0, 0) = Dii;	D(0, 1) = Dij;	D(0, 2) = Dij;	D(0, 3) = 0.0;	D(0, 4) = 0.0;	D(0, 5) = 0.0;
	D(1, 0) = Dij;	D(1, 1) = Dii;	D(1, 2) = Dij;	D(1, 3) = 0.0;	D(1, 4) = 0.0;	D(1, 5) = 0.0;
	D(2, 0) = Dij;	D(2, 1) = Dij;	D(2, 2) = Dii;	D(2, 3) = 0.0;	D(2, 4) = 0.0;	D(2, 5) = 0.0;
	D(3, 0) = 0.0;	D(3, 1) = 0.0;	D(3, 2) = 0.0;	D(3, 3) = Gii;	D(3, 4) = 0.0;	D(3, 5) = 0.0;
	D(4, 0) = 0.0;	D(4, 1) = 0.0;	D(4, 2) = 0.0;	D(4, 3) = 0.0;	D(4, 4) = Gii;	D(4, 5) = 0.0;
	D(5, 0) = 0.0;	D(5, 1) = 0.0;	D(5, 2) = 0.0;	D(5, 3) = 0.0;	D(5, 4) = 0.0;	D(5, 5) = Gii;
	
	//----------�A�_�v�^�s��̐���----------
	Eigen::MatrixXd A = Eigen::MatrixXd(6, 9);
	A(0, 0) = 1.0;	A(0, 1) = 0.0;	A(0, 2) = 0.0;	A(0, 3) = 0.0;	A(0, 4) = 0.0;	A(0, 5) = 0.0;	A(0, 6) = 0.0;	A(0, 7) = 0.0;	A(0, 8) = 0.0;
	A(1, 0) = 0.0;	A(1, 1) = 0.0;	A(1, 2) = 0.0;	A(1, 3) = 0.0;	A(1, 4) = 1.0;	A(1, 5) = 0.0;	A(1, 6) = 0.0;	A(1, 7) = 0.0;	A(1, 8) = 0.0;
	A(2, 0) = 0.0;	A(2, 1) = 0.0;	A(2, 2) = 0.0;	A(2, 3) = 0.0;	A(2, 4) = 0.0;	A(2, 5) = 0.0;	A(2, 6) = 0.0;	A(2, 7) = 0.0;	A(2, 8) = 1.0;
	A(3, 0) = 0.0;	A(3, 1) = 1.0;	A(3, 2) = 0.0;	A(3, 3) = 1.0;	A(3, 4) = 0.0;	A(3, 5) = 0.0;	A(3, 6) = 0.0;	A(3, 7) = 0.0;	A(3, 8) = 0.0;
	A(4, 0) = 0.0;	A(4, 1) = 0.0;	A(4, 2) = 0.0;	A(4, 3) = 0.0;	A(4, 4) = 0.0;	A(4, 5) = 1.0;	A(4, 6) = 0.0;	A(4, 7) = 1.0;	A(4, 8) = 0.0;
	A(5, 0) = 0.0;	A(5, 1) = 0.0;	A(5, 2) = 1.0;	A(5, 3) = 0.0;	A(5, 4) = 0.0;	A(5, 5) = 0.0;	A(5, 6) = 1.0;	A(5, 7) = 0.0;	A(5, 8) = 0.0;

	//----------[Ke]�}�g���b�N�X�̔�ϕ��֐����`----------
	std::function<Eigen::MatrixXd(std::vector<double>)> ke = [&](std::vector<double> _xi) {
		Eigen::MatrixXd B = A * this->pelement->dTrialdx(this->ueq_to_us, _xi);
		Eigen::MatrixXd ret = B.transpose() * D * B * this->pelement->Jacobian(_xi).determinant();
		return ret;
	};

	//----------[Ke]�}�g���b�N�X�����߂�ϕ�----------
	this->Ke = this->pintegration->Integrate(ke);

	//----------�̐ϗ̓x�N�g�������߂�----------
	Eigen::VectorXd G = Eigen::VectorXd::Zero(3);	//���d�����Ƃ���

	//----------{F}�x�N�g���̔�ϕ��֐����`----------
	std::function<Eigen::VectorXd(std::vector<double>)> fe = [&](std::vector<double> _xi) {
		Eigen::VectorXd ret = this->pelement->Test(this->ueq_to_us, _xi).transpose() * G;
		return ret;
	};

	//----------{F}�x�N�g�������߂�----------
	this->Fe = this->pintegration->Integrate(fe);
}