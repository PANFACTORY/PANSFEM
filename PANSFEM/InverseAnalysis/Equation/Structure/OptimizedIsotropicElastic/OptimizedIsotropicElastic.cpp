//***********************************************************************************************************
//Title		:PANSFEM/InverseAnalysis/Equation/Structure/OptimizedIsotropicElastic/OptimizedIsotropicElastic.cpp
//Author	:Tanabe Yuta
//Date		:2019/09/22
//Copyright	:(C)2019 TanabeYuta
//***********************************************************************************************************


#include "OptimizedIsotropicElastic.h"


PANSFEM::OptimizedIsotropicElastic::OptimizedIsotropicElastic() : Equation() {}


PANSFEM::OptimizedIsotropicElastic::~OptimizedIsotropicElastic() {}


PANSFEM::OptimizedIsotropicElastic::OptimizedIsotropicElastic(Element * _pelement, std::vector<int> _ulist, std::vector<int> _refulist, std::vector<int> _plist, Integration * _pintegration) : Equation(_pelement, _ulist, _refulist, _plist, _pintegration, 3, 3, 0, 4){}


void PANSFEM::OptimizedIsotropicElastic::SetEquation() {
	//----------パラメータの取得----------
	double rho = this->pelement->pparameter->parameters[this->peq_to_ps[0]];
	double Emax = this->pelement->pparameter->parameters[this->peq_to_ps[1]];
	double Emin = this->pelement->pparameter->parameters[this->peq_to_ps[2]];
	double V = this->pelement->pparameter->parameters[this->peq_to_ps[3]];

	//----------[D]の生成----------
	double E = (Emax - Emin)*pow(rho, 3.0) + Emin;
	Eigen::MatrixXd D = Eigen::MatrixXd(6, 6);
	D(0, 0) = 1.0 - V;	D(0, 1) = V;		D(0, 2) = V;		D(0, 3) = 0.0;					D(0, 4) = 0.0;					D(0, 5) = 0.0;
	D(1, 0) = V;		D(1, 1) = 1.0 - V;	D(1, 2) = V;		D(1, 3) = 0.0;					D(1, 4) = 0.0;					D(1, 5) = 0.0;
	D(2, 0) = V;		D(2, 1) = V;		D(2, 2) = 1.0 - V;	D(2, 3) = 0.0;					D(2, 4) = 0.0;					D(2, 5) = 0.0;
	D(3, 0) = 0.0;		D(3, 1) = 0.0;		D(3, 2) = 0.0;		D(3, 3) = 0.5*(1.0 - 2.0*V);	D(3, 4) = 0.0;					D(3, 5) = 0.0;
	D(4, 0) = 0.0;		D(4, 1) = 0.0;		D(4, 2) = 0.0;		D(4, 3) = 0.0;					D(4, 4) = 0.5*(1.0 - 2.0*V);	D(4, 5) = 0.0;
	D(5, 0) = 0.0;		D(5, 1) = 0.0;		D(5, 2) = 0.0;		D(5, 3) = 0.0;					D(5, 4) = 0.0;					D(5, 5) = 0.5*(1.0 - 2.0*V);
	D *= E / ((1.0 - 2.0*V)*(1.0 + V));

	//----------アダプタ行列の生成----------
	Eigen::MatrixXd A = Eigen::MatrixXd(6, 9);
	A(0, 0) = 1.0;	A(0, 1) = 0.0;	A(0, 2) = 0.0;	A(0, 3) = 0.0;	A(0, 4) = 0.0;	A(0, 5) = 0.0;	A(0, 6) = 0.0;	A(0, 7) = 0.0;	A(0, 8) = 0.0;
	A(1, 0) = 0.0;	A(1, 1) = 0.0;	A(1, 2) = 0.0;	A(1, 3) = 0.0;	A(1, 4) = 1.0;	A(1, 5) = 0.0;	A(1, 6) = 0.0;	A(1, 7) = 0.0;	A(1, 8) = 0.0;
	A(2, 0) = 0.0;	A(2, 1) = 0.0;	A(2, 2) = 0.0;	A(2, 3) = 0.0;	A(2, 4) = 0.0;	A(2, 5) = 0.0;	A(2, 6) = 0.0;	A(2, 7) = 0.0;	A(2, 8) = 1.0;
	A(3, 0) = 0.0;	A(3, 1) = 1.0;	A(3, 2) = 0.0;	A(3, 3) = 1.0;	A(3, 4) = 0.0;	A(3, 5) = 0.0;	A(3, 6) = 0.0;	A(3, 7) = 0.0;	A(3, 8) = 0.0;
	A(4, 0) = 0.0;	A(4, 1) = 0.0;	A(4, 2) = 0.0;	A(4, 3) = 0.0;	A(4, 4) = 0.0;	A(4, 5) = 1.0;	A(4, 6) = 0.0;	A(4, 7) = 1.0;	A(4, 8) = 0.0;
	A(5, 0) = 0.0;	A(5, 1) = 0.0;	A(5, 2) = 1.0;	A(5, 3) = 0.0;	A(5, 4) = 0.0;	A(5, 5) = 0.0;	A(5, 6) = 1.0;	A(5, 7) = 0.0;	A(5, 8) = 0.0;

	//----------[Ke]マトリックスの被積分関数を定義----------
	std::function<Eigen::MatrixXd(std::vector<double>)> ke = [&](std::vector<double> _xi) {
		Eigen::MatrixXd B = A * this->pelement->dTrialdx(this->ueq_to_us, _xi);
		Eigen::MatrixXd ret = B.transpose() * D * B * this->pelement->Jacobian(_xi).determinant();
		return ret;
	};

	//----------[Ke]マトリックスを求める積分----------
	this->Ke = this->pintegration->Integrate(ke);

	//----------体積力ベクトルを求める----------
	Eigen::VectorXd G = Eigen::VectorXd::Zero(3);	//自重無しとする

	//----------{F}ベクトルの被積分関数を定義----------
	std::function<Eigen::VectorXd(std::vector<double>)> fe = [&](std::vector<double> _xi) {
		Eigen::VectorXd ret = this->pelement->Test(this->ueq_to_us, _xi).transpose() * G;
		return ret;
	};

	//----------{F}ベクトルを求める----------
	this->Fe = this->pintegration->Integrate(fe);
}
