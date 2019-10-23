//***********************************************************************************************************
//Title		:PANSFEM/InversedAnalysis/Function/Compliance/2PhaseMaterials3D/Compliance2PhaseMaterials3D.cpp
//Author	:Tanabe Yuta
//Date		:2019/10/22
//Copyright	:(C)2019 TanabeYuta
//***********************************************************************************************************


#include "Compliance2PhaseMaterials3D.h"


PANSFEM::Compliance2PhaseMaterials3D::Compliance2PhaseMaterials3D() : Function() {}


PANSFEM::Compliance2PhaseMaterials3D::~Compliance2PhaseMaterials3D() {}


PANSFEM::Compliance2PhaseMaterials3D::Compliance2PhaseMaterials3D(std::vector<int> _plist, std::vector<int> _refulist, std::vector<int> _refplist) : Function(_plist, _refulist, _refplist, 1, 3, 7) {}


Eigen::VectorXd PANSFEM::Compliance2PhaseMaterials3D::sensitivitis() {
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
		double s = pelement->pparameter->parameters[this->refpf_to_us[0]];
		double E0 = pelement->pparameter->parameters[this->refpf_to_us[1]];
		double V0 = pelement->pparameter->parameters[this->refpf_to_us[2]];
		double G0 = pelement->pparameter->parameters[this->refpf_to_us[3]];
		double E1 = pelement->pparameter->parameters[this->refpf_to_us[4]];
		double V1 = pelement->pparameter->parameters[this->refpf_to_us[5]];
		double G1 = pelement->pparameter->parameters[this->refpf_to_us[6]];


		double D0ii = E0 * (1.0 - V0) / ((1.0 - 2.0*V0)*(1.0 + V0));
		double D0ij = E0 * V0 / ((1.0 - 2.0*V0)*(1.0 + V0));
		double D1ii = E1 * (1.0 - V1) / ((1.0 - 2.0*V1)*(1.0 + V1));
		double D1ij = E1 * V1 / ((1.0 - 2.0*V1)*(1.0 + V1));


		double p = 3;


		double dDiids = -p * pow(s, p - 1)*(D0ii - D1ii);
		double dDijds = -p * pow(s, p - 1)*(D0ij - D1ij);
		double dGds = -p * pow(s, p - 1)*(G0 - G1);


		//----------[D]の生成----------
		Eigen::MatrixXd dDds = Eigen::MatrixXd(6, 6);
		dDds(0, 0) = dDiids;	dDds(0, 1) = dDijds;	dDds(0, 2) = dDijds;	dDds(0, 3) = 0.0;		dDds(0, 4) = 0.0;		dDds(0, 5) = 0.0;
		dDds(1, 0) = dDijds;	dDds(1, 1) = dDiids;	dDds(1, 2) = dDijds;	dDds(1, 3) = 0.0;		dDds(1, 4) = 0.0;		dDds(1, 5) = 0.0;
		dDds(2, 0) = dDijds;	dDds(2, 1) = dDijds;	dDds(2, 2) = dDiids;	dDds(2, 3) = 0.0;		dDds(2, 4) = 0.0;		dDds(2, 5) = 0.0;
		dDds(3, 0) = 0.0;		dDds(3, 1) = 0.0;		dDds(3, 2) = 0.0;		dDds(3, 3) = dGds;		dDds(3, 4) = 0.0;		dDds(3, 5) = 0.0;
		dDds(4, 0) = 0.0;		dDds(4, 1) = 0.0;		dDds(4, 2) = 0.0;		dDds(4, 3) = 0.0;		dDds(4, 4) = dGds;		dDds(4, 5) = 0.0;
		dDds(5, 0) = 0.0;		dDds(5, 1) = 0.0;		dDds(5, 2) = 0.0;		dDds(5, 3) = 0.0;		dDds(5, 4) = 0.0;		dDds(5, 5) = dGds;


		//----------要素毎のコンプライアンスを計算----------
		std::function<double(std::vector<double>) > f = [=](std::vector<double> _xi) {
			Eigen::MatrixXd B = pelement->dTrialdx(this->refuf_to_us, _xi);
			Eigen::VectorXd u = pelement->u(this->refuf_to_us);
			return (-u.transpose()*B.transpose()*A.transpose()*dDds*A*B*u*pelement->Jacobian(_xi).determinant())[0];
		};

		//----------要素毎の積分計算----------
		dcompliance[this->parametersindex[pelement->pparameter] * this->NOP] += this->pintegrations[pelement]->Integrate(f);
	}

	return dcompliance;
}


double PANSFEM::Compliance2PhaseMaterials3D::value() {
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
		double s = pelement->pparameter->parameters[this->refpf_to_us[0]];
		double E0 = pelement->pparameter->parameters[this->refpf_to_us[1]];
		double V0 = pelement->pparameter->parameters[this->refpf_to_us[2]];
		double G0 = pelement->pparameter->parameters[this->refpf_to_us[3]];
		double E1 = pelement->pparameter->parameters[this->refpf_to_us[4]];
		double V1 = pelement->pparameter->parameters[this->refpf_to_us[5]];
		double G1 = pelement->pparameter->parameters[this->refpf_to_us[6]];


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