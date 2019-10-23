//***********************************************************************************************************
//Title		:PANSFEM/InversedAnalysis/Structure/Optimized2PhaseMaterials3D/Optimized2PhaseMaterials3D.cpp
//Author	:Tanabe Yuta
//Date		:2019/10/22
//Copyright	:(C)2019 TanabeYuta
//***********************************************************************************************************


#include "Optimized2PhaseMaterials3D.h"


PANSFEM::Optimized2PhaseMaterials3D::Optimized2PhaseMaterials3D() : Equation() {}


PANSFEM::Optimized2PhaseMaterials3D::~Optimized2PhaseMaterials3D() {}


PANSFEM::Optimized2PhaseMaterials3D::Optimized2PhaseMaterials3D(Element * _pelement, std::vector<int> _ulist, std::vector<int> _refulist, std::vector<int> _plist, Integration * _pintegration) : Equation(_pelement, _ulist, _refulist, _plist, _pintegration, 3, 3, 0, 7) {}


void PANSFEM::Optimized2PhaseMaterials3D::SetEquation() {
	//----------パラメータの取得----------
	double s = this->pelement->pparameter->parameters[this->peq_to_ps[0]];
	double E0 = this->pelement->pparameter->parameters[this->peq_to_ps[1]];
	double V0 = this->pelement->pparameter->parameters[this->peq_to_ps[2]];
	double G0 = this->pelement->pparameter->parameters[this->peq_to_ps[3]];
	double E1 = this->pelement->pparameter->parameters[this->peq_to_ps[4]];
	double V1 = this->pelement->pparameter->parameters[this->peq_to_ps[5]];
	double G1 = this->pelement->pparameter->parameters[this->peq_to_ps[6]];


	double D0ii = E0 * (1.0 - V0) / ((1.0 - 2.0*V0)*(1.0 + V0));
	double D0ij = E0 * V0 / ((1.0 - 2.0*V0)*(1.0 + V0));
	double D1ii = E1 * (1.0 - V1) / ((1.0 - 2.0*V1)*(1.0 + V1));
	double D1ij = E1 * V1 / ((1.0 - 2.0*V1)*(1.0 + V1));


	double p = 3;


	double Dii = (1.0 - pow(s, p))*D0ii + pow(s, p)*D1ii;
	double Dij = (1.0 - pow(s, p))*D0ij + pow(s, p)*D1ij;
	double G = (1.0 - pow(s, p))*G0 + pow(s, p)*G1;


	//----------[D]の生成----------
	Eigen::MatrixXd D = Eigen::MatrixXd(6, 6);
	D(0, 0) = Dii;	D(0, 1) = Dij;	D(0, 2) = Dij;	D(0, 3) = 0.0;	D(0, 4) = 0.0;	D(0, 5) = 0.0;
	D(1, 0) = Dij;	D(1, 1) = Dii;	D(1, 2) = Dij;	D(1, 3) = 0.0;	D(1, 4) = 0.0;	D(1, 5) = 0.0;
	D(2, 0) = Dij;	D(2, 1) = Dij;	D(2, 2) = Dii;	D(2, 3) = 0.0;	D(2, 4) = 0.0;	D(2, 5) = 0.0;
	D(3, 0) = 0.0;	D(3, 1) = 0.0;	D(3, 2) = 0.0;	D(3, 3) = G;	D(3, 4) = 0.0;	D(3, 5) = 0.0;
	D(4, 0) = 0.0;	D(4, 1) = 0.0;	D(4, 2) = 0.0;	D(4, 3) = 0.0;	D(4, 4) = G;	D(4, 5) = 0.0;
	D(5, 0) = 0.0;	D(5, 1) = 0.0;	D(5, 2) = 0.0;	D(5, 3) = 0.0;	D(5, 4) = 0.0;	D(5, 5) = G;

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
	Eigen::VectorXd Gv = Eigen::VectorXd::Zero(3);	//自重無しとする

	//----------{F}ベクトルの被積分関数を定義----------
	std::function<Eigen::VectorXd(std::vector<double>)> fe = [&](std::vector<double> _xi) {
		Eigen::VectorXd ret = this->pelement->Test(this->ueq_to_us, _xi).transpose() * Gv;
		return ret;
	};

	//----------{F}ベクトルを求める----------
	this->Fe = this->pintegration->Integrate(fe);
}