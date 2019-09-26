//*****************************************************************************
//Title		:PANSFEM/Phenomenon/Structure/PlaneStrain/PlaneStrain.cpp
//Author	:Tanabe Yuta
//Date		:2019/06/10
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "PlaneStrainVibration.h"


PANSFEM::PlaneStrainVibration::PlaneStrainVibration() : Equation() {}


PANSFEM::PlaneStrainVibration::~PlaneStrainVibration() {}


PANSFEM::PlaneStrainVibration::PlaneStrainVibration(Element * _pelement, std::vector<int> _ulist, std::vector<int> _refulist, std::vector<int> _plist, Integration* _pintegration) : Equation(_pelement, _ulist, _refulist, _plist, _pintegration, 2, 2, 0, 4) {}


void PANSFEM::PlaneStrainVibration::SetEquation() {
	//----------物性値の取得----------
	double E = this->pelement->pparameter->parameters[this->peq_to_ps[0]];
	double V = this->pelement->pparameter->parameters[this->peq_to_ps[1]];
	double t = this->pelement->pparameter->parameters[this->peq_to_ps[2]];
	double rho = this->pelement->pparameter->parameters[this->peq_to_ps[3]];

	//----------[D]マトリックスの生成----------
	Eigen::MatrixXd D = Eigen::MatrixXd(3, 3);
	D(0, 0) = 1.0 - V;	D(0, 1) = V;		D(0, 2) = 0.0;
	D(1, 0) = V;		D(1, 1) = 1.0 - V;	D(1, 2) = 0.0;
	D(2, 0) = 0.0;		D(2, 1) = 0.0;		D(2, 2) = (1.0 - 2.0*V) / 2.0;
	D *= E / ((1.0 - 2.0*V)*(1.0 + V));

	//----------アダプタ行列の生成----------
	Eigen::MatrixXd A = Eigen::MatrixXd(3, 4);
	A(0, 0) = 1.0;	A(0, 1) = 0.0;	A(0, 2) = 0.0;	A(0, 3) = 0.0;
	A(1, 0) = 0.0;	A(1, 1) = 0.0;	A(1, 2) = 0.0;	A(1, 3) = 1.0;
	A(2, 0) = 0.0;	A(2, 1) = 1.0;	A(2, 2) = 1.0;	A(2, 3) = 0.0;

	//----------[Ke]マトリックスの被積分関数を定義----------
	std::function<Eigen::MatrixXd(std::vector<double>)> ke = [&](std::vector<double> _xi) {
		Eigen::MatrixXd B = A * this->pelement->dTrialdx(this->ueq_to_us, _xi);
		Eigen::MatrixXd ret = B.transpose() * D * B * this->pelement->Jacobian(_xi).determinant() * t;
		return ret;
	};

	//----------[Ke]マトリックスを求める積分----------
	this->Ke = this->pintegration->Integrate(ke);

	//----------[Fe]マトリックスの被積分関数を定義----------
	std::function<Eigen::MatrixXd(std::vector<double>)> fe = [&](std::vector<double> _xi) {
		Eigen::MatrixXd B = this->pelement->Trial(this->ueq_to_us, _xi);
		Eigen::MatrixXd ret = B.transpose() * rho * B * this->pelement->Jacobian(_xi).determinant() * t;
		return ret;
	};

	//----------[Fe]マトリックスを求める----------
	this->Fe = this->pintegration->Integrate(fe);
}