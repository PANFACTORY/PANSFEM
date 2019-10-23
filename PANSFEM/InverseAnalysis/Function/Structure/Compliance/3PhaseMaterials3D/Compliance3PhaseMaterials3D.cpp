//***********************************************************************************************************
//Title		:PANSFEM/InversedAnalysis/Function/Compliance/3PhaseMaterials3D/Compliance3PhaseMaterials3D.cpp
//Author	:Tanabe Yuta
//Date		:2019/10/14
//Copyright	:(C)2019 TanabeYuta
//***********************************************************************************************************


#include "Compliance3PhaseMaterials3D.h"


PANSFEM::Compliance3PhaseMaterials3D::Compliance3PhaseMaterials3D() : Function() {}


PANSFEM::Compliance3PhaseMaterials3D::~Compliance3PhaseMaterials3D() {}


PANSFEM::Compliance3PhaseMaterials3D::Compliance3PhaseMaterials3D(std::vector<int> _plist, std::vector<int> _refulist, std::vector<int> _refplist) : Function(_plist, _refulist, _refplist, 2, 3, 11) {}


Eigen::VectorXd PANSFEM::Compliance3PhaseMaterials3D::sensitivitis() {
	Eigen::VectorXd dcompliance = Eigen::VectorXd::Zero(this->NOP*this->parametersindex.size());

	//----------[A]の生成----------
	Eigen::MatrixXd A = Eigen::MatrixXd(6, 9);
	A(0, 0) = 1.0;	A(0, 1) = 0.0;	A(0, 2) = 0.0;	A(0, 3) = 0.0;	A(0, 4) = 0.0;	A(0, 5) = 0.0;	A(0, 6) = 0.0;	A(0, 7) = 0.0;	A(0, 8) = 0.0;
	A(1, 0) = 0.0;	A(1, 1) = 0.0;	A(1, 2) = 0.0;	A(1, 3) = 0.0;	A(1, 4) = 1.0;	A(1, 5) = 0.0;	A(1, 6) = 0.0;	A(1, 7) = 0.0;	A(1, 8) = 0.0;
	A(2, 0) = 0.0;	A(2, 1) = 0.0;	A(2, 2) = 0.0;	A(2, 3) = 0.0;	A(2, 4) = 0.0;	A(2, 5) = 0.0;	A(2, 6) = 0.0;	A(2, 7) = 0.0;	A(2, 8) = 1.0;
	A(3, 0) = 0.0;	A(3, 1) = 1.0;	A(3, 2) = 0.0;	A(3, 3) = 1.0;	A(3, 4) = 0.0;	A(3, 5) = 0.0;	A(3, 6) = 0.0;	A(3, 7) = 0.0;	A(3, 8) = 0.0;
	A(4, 0) = 0.0;	A(4, 1) = 0.0;	A(4, 2) = 0.0;	A(4, 3) = 0.0;	A(4, 4) = 0.0;	A(4, 5) = 1.0;	A(4, 6) = 0.0;	A(4, 7) = 1.0;	A(4, 8) = 0.0;
	A(5, 0) = 0.0;	A(5, 1) = 0.0;	A(5, 2) = 1.0;	A(5, 3) = 0.0;	A(5, 4) = 0.0;	A(5, 5) = 0.0;	A(5, 6) = 1.0;	A(5, 7) = 0.0;	A(5, 8) = 0.0;

	for (auto pelement : this->pelements) {
		//----------パラメータの取得----------
		double s0 = pelement->pparameter->parameters[this->refpf_to_us[0]];
		double s1 = pelement->pparameter->parameters[this->refpf_to_us[1]];
		double E0 = pelement->pparameter->parameters[this->refpf_to_us[2]];
		double V0 = pelement->pparameter->parameters[this->refpf_to_us[3]];
		double G0 = pelement->pparameter->parameters[this->refpf_to_us[4]];
		double E1 = pelement->pparameter->parameters[this->refpf_to_us[5]];
		double V1 = pelement->pparameter->parameters[this->refpf_to_us[6]];
		double G1 = pelement->pparameter->parameters[this->refpf_to_us[7]];
		double E2 = pelement->pparameter->parameters[this->refpf_to_us[8]];
		double V2 = pelement->pparameter->parameters[this->refpf_to_us[9]];
		double G2 = pelement->pparameter->parameters[this->refpf_to_us[10]];


		double D0ii = E0 * (1.0 - V0) / ((1.0 - 2.0*V0)*(1.0 + V0));
		double D0ij = E0 * V0 / ((1.0 - 2.0*V0)*(1.0 + V0));
		double D1ii = E1 * (1.0 - V1) / ((1.0 - 2.0*V1)*(1.0 + V1));
		double D1ij = E1 * V1 / ((1.0 - 2.0*V1)*(1.0 + V1));
		double D2ii = E2 * (1.0 - V2) / ((1.0 - 2.0*V2)*(1.0 + V2));
		double D2ij = E2 * V2 / ((1.0 - 2.0*V2)*(1.0 + V2));


		double p = 3, q = 3;


		double dDiids0 = q * pow(s0, q - 1.0)*((1.0 - pow(s1, p))*D1ii + pow(s1, p)*D2ii - D0ii);
		double dDijds0 = q * pow(s0, q - 1.0)*((1.0 - pow(s1, p))*D1ij + pow(s1, p)*D2ij - D0ij);
		double dGds0 = q * pow(s0, q - 1.0)*((1.0 - pow(s1, p))*G1 + pow(s1, p)*G2 - G0);


		double dDiids1 = p * pow(s0, q)*pow(s1, p - 1.0)*(D2ii - D1ii);
		double dDijds1 = p * pow(s0, q)*pow(s1, p - 1.0)*(D2ij - D1ij);
		double dGds1 = p * pow(s0, q)*pow(s1, p - 1.0)*(G2 - G1);


		//----------[D]の生成----------
		Eigen::MatrixXd dDds0 = Eigen::MatrixXd(6, 6);
		dDds0(0, 0) = dDiids0;	dDds0(0, 1) = dDijds0;	dDds0(0, 2) = dDijds0;	dDds0(0, 3) = 0.0;		dDds0(0, 4) = 0.0;		dDds0(0, 5) = 0.0;
		dDds0(1, 0) = dDijds0;	dDds0(1, 1) = dDiids0;	dDds0(1, 2) = dDijds0;	dDds0(1, 3) = 0.0;		dDds0(1, 4) = 0.0;		dDds0(1, 5) = 0.0;
		dDds0(2, 0) = dDijds0;	dDds0(2, 1) = dDijds0;	dDds0(2, 2) = dDiids0;	dDds0(2, 3) = 0.0;		dDds0(2, 4) = 0.0;		dDds0(2, 5) = 0.0;
		dDds0(3, 0) = 0.0;		dDds0(3, 1) = 0.0;		dDds0(3, 2) = 0.0;		dDds0(3, 3) = dGds0;	dDds0(3, 4) = 0.0;		dDds0(3, 5) = 0.0;
		dDds0(4, 0) = 0.0;		dDds0(4, 1) = 0.0;		dDds0(4, 2) = 0.0;		dDds0(4, 3) = 0.0;		dDds0(4, 4) = dGds0;	dDds0(4, 5) = 0.0;
		dDds0(5, 0) = 0.0;		dDds0(5, 1) = 0.0;		dDds0(5, 2) = 0.0;		dDds0(5, 3) = 0.0;		dDds0(5, 4) = 0.0;		dDds0(5, 5) = dGds0;

		Eigen::MatrixXd dDds1 = Eigen::MatrixXd(6, 6);
		dDds1(0, 0) = dDiids1;	dDds1(0, 1) = dDijds1;	dDds1(0, 2) = dDijds1;	dDds1(0, 3) = 0.0;		dDds1(0, 4) = 0.0;		dDds1(0, 5) = 0.0;
		dDds1(1, 0) = dDijds1;	dDds1(1, 1) = dDiids1;	dDds1(1, 2) = dDijds1;	dDds1(1, 3) = 0.0;		dDds1(1, 4) = 0.0;		dDds1(1, 5) = 0.0;
		dDds1(2, 0) = dDijds1;	dDds1(2, 1) = dDijds1;	dDds1(2, 2) = dDiids1;	dDds1(2, 3) = 0.0;		dDds1(2, 4) = 0.0;		dDds1(2, 5) = 0.0;
		dDds1(3, 0) = 0.0;		dDds1(3, 1) = 0.0;		dDds1(3, 2) = 0.0;		dDds1(3, 3) = dGds1;	dDds1(3, 4) = 0.0;		dDds1(3, 5) = 0.0;
		dDds1(4, 0) = 0.0;		dDds1(4, 1) = 0.0;		dDds1(4, 2) = 0.0;		dDds1(4, 3) = 0.0;		dDds1(4, 4) = dGds1;	dDds1(4, 5) = 0.0;
		dDds1(5, 0) = 0.0;		dDds1(5, 1) = 0.0;		dDds1(5, 2) = 0.0;		dDds1(5, 3) = 0.0;		dDds1(5, 4) = 0.0;		dDds1(5, 5) = dGds1;

		//----------要素毎のコンプライアンスを計算----------
		std::function<Eigen::VectorXd(std::vector<double>) > f = [=](std::vector<double> _xi) {
			Eigen::MatrixXd B = pelement->dTrialdx(this->refuf_to_us, _xi);
			Eigen::VectorXd u = pelement->u(this->refuf_to_us);
			Eigen::VectorXd c = Eigen::VectorXd(2);
			c.segment(0, 1) = -u.transpose()*B.transpose()*A.transpose()*dDds0*A*B*u*pelement->Jacobian(_xi).determinant();
			c.segment(1, 1) = -u.transpose()*B.transpose()*A.transpose()*dDds1*A*B*u*pelement->Jacobian(_xi).determinant();
			return c;
		};

		//----------要素毎の積分計算----------
		dcompliance.segment(this->parametersindex[pelement->pparameter] * this->NOP, 2) += this->pintegrations[pelement]->Integrate(f);
	}

	return dcompliance;
}


double PANSFEM::Compliance3PhaseMaterials3D::value() {
	double compliance = 0.0;

	//----------[A]の生成----------
	Eigen::MatrixXd A = Eigen::MatrixXd(6, 9);
	A(0, 0) = 1.0;	A(0, 1) = 0.0;	A(0, 2) = 0.0;	A(0, 3) = 0.0;	A(0, 4) = 0.0;	A(0, 5) = 0.0;	A(0, 6) = 0.0;	A(0, 7) = 0.0;	A(0, 8) = 0.0;
	A(1, 0) = 0.0;	A(1, 1) = 0.0;	A(1, 2) = 0.0;	A(1, 3) = 0.0;	A(1, 4) = 1.0;	A(1, 5) = 0.0;	A(1, 6) = 0.0;	A(1, 7) = 0.0;	A(1, 8) = 0.0;
	A(2, 0) = 0.0;	A(2, 1) = 0.0;	A(2, 2) = 0.0;	A(2, 3) = 0.0;	A(2, 4) = 0.0;	A(2, 5) = 0.0;	A(2, 6) = 0.0;	A(2, 7) = 0.0;	A(2, 8) = 1.0;
	A(3, 0) = 0.0;	A(3, 1) = 1.0;	A(3, 2) = 0.0;	A(3, 3) = 1.0;	A(3, 4) = 0.0;	A(3, 5) = 0.0;	A(3, 6) = 0.0;	A(3, 7) = 0.0;	A(3, 8) = 0.0;
	A(4, 0) = 0.0;	A(4, 1) = 0.0;	A(4, 2) = 0.0;	A(4, 3) = 0.0;	A(4, 4) = 0.0;	A(4, 5) = 1.0;	A(4, 6) = 0.0;	A(4, 7) = 1.0;	A(4, 8) = 0.0;
	A(5, 0) = 0.0;	A(5, 1) = 0.0;	A(5, 2) = 1.0;	A(5, 3) = 0.0;	A(5, 4) = 0.0;	A(5, 5) = 0.0;	A(5, 6) = 1.0;	A(5, 7) = 0.0;	A(5, 8) = 0.0;

	for (auto pelement : this->pelements) {
		//----------パラメータの取得----------
		double s0 = pelement->pparameter->parameters[this->refpf_to_us[0]];
		double s1 = pelement->pparameter->parameters[this->refpf_to_us[1]];
		double E0 = pelement->pparameter->parameters[this->refpf_to_us[2]];
		double V0 = pelement->pparameter->parameters[this->refpf_to_us[3]];
		double G0 = pelement->pparameter->parameters[this->refpf_to_us[4]];
		double E1 = pelement->pparameter->parameters[this->refpf_to_us[5]];
		double V1 = pelement->pparameter->parameters[this->refpf_to_us[6]];
		double G1 = pelement->pparameter->parameters[this->refpf_to_us[7]];
		double E2 = pelement->pparameter->parameters[this->refpf_to_us[8]];
		double V2 = pelement->pparameter->parameters[this->refpf_to_us[9]];
		double G2 = pelement->pparameter->parameters[this->refpf_to_us[10]];


		double D0ii = E0 * (1.0 - V0) / ((1.0 - 2.0*V0)*(1.0 + V0));
		double D0ij = E0 * V0 / ((1.0 - 2.0*V0)*(1.0 + V0));
		double D1ii = E1 * (1.0 - V1) / ((1.0 - 2.0*V1)*(1.0 + V1));
		double D1ij = E1 * V1 / ((1.0 - 2.0*V1)*(1.0 + V1));
		double D2ii = E2 * (1.0 - V2) / ((1.0 - 2.0*V2)*(1.0 + V2));
		double D2ij = E2 * V2 / ((1.0 - 2.0*V2)*(1.0 + V2));


		double p = 3, q = 3;


		double Dii = (1.0 - pow(s0, q))*D0ii + pow(s0, q)*((1.0 - pow(s1, p))*D1ii + pow(s1, p)*D2ii);
		double Dij = (1.0 - pow(s0, q))*D0ij + pow(s0, q)*((1.0 - pow(s1, p))*D1ij + pow(s1, p)*D2ij);
		double G = (1.0 - pow(s0, q))*G0 + pow(s0, q)*((1.0 - pow(s1, p))*G1 + pow(s1, p)*G2);


		//----------[D]の生成----------
		Eigen::MatrixXd D = Eigen::MatrixXd(6, 6);
		D(0, 0) = Dii;	D(0, 1) = Dij;	D(0, 2) = Dij;	D(0, 3) = 0.0;	D(0, 4) = 0.0;	D(0, 5) = 0.0;
		D(1, 0) = Dij;	D(1, 1) = Dii;	D(1, 2) = Dij;	D(1, 3) = 0.0;	D(1, 4) = 0.0;	D(1, 5) = 0.0;
		D(2, 0) = Dij;	D(2, 1) = Dij;	D(2, 2) = Dii;	D(2, 3) = 0.0;	D(2, 4) = 0.0;	D(2, 5) = 0.0;
		D(3, 0) = 0.0;	D(3, 1) = 0.0;	D(3, 2) = 0.0;	D(3, 3) = G;	D(3, 4) = 0.0;	D(3, 5) = 0.0;
		D(4, 0) = 0.0;	D(4, 1) = 0.0;	D(4, 2) = 0.0;	D(4, 3) = 0.0;	D(4, 4) = G;	D(4, 5) = 0.0;
		D(5, 0) = 0.0;	D(5, 1) = 0.0;	D(5, 2) = 0.0;	D(5, 3) = 0.0;	D(5, 4) = 0.0;	D(5, 5) = G;

		//----------要素毎のコンプライアンスを計算----------
		std::function<double(std::vector<double>) > f = [=](std::vector<double> _xi) {
			Eigen::MatrixXd B = pelement->dTrialdx(this->refuf_to_us, _xi);
			Eigen::VectorXd u = pelement->u(this->refuf_to_us);
			return (u.transpose()*B.transpose()*A.transpose()*D*A*B*u*pelement->Jacobian(_xi).determinant())(0);
		};

		//----------要素毎の積分計算----------
		compliance += this->pintegrations[pelement]->Integrate(f);
	}

	return compliance;
}