//***********************************************************************************************************
//Title		:PANSFEM/InversedAnalysis/Function/ComplianceIsotropicElastic/ComplianceIsotropicElastic.cpp
//Author	:Tanabe Yuta
//Date		:2019/09/22
//Copyright	:(C)2019 TanabeYuta
//***********************************************************************************************************


#include "ComplianceIsotropicElastic.h"


PANSFEM::ComplianceIsotropicElastic::ComplianceIsotropicElastic() : Function(){}


PANSFEM::ComplianceIsotropicElastic::~ComplianceIsotropicElastic(){}


PANSFEM::ComplianceIsotropicElastic::ComplianceIsotropicElastic(std::vector<int> _plist, std::vector<int> _refulist, std::vector<int> _refplist) : Function(_plist, _refulist, _refplist, 1, 3, 4) {}


Eigen::VectorXd PANSFEM::ComplianceIsotropicElastic::sensitivitis() {
	Eigen::VectorXd dcompliance = Eigen::VectorXd::Zero(this->NOP*this->pelements.size());

	//----------[A]の生成----------
	Eigen::MatrixXd A = Eigen::MatrixXd(6, 9);
	A(0, 0) = 1.0;	A(0, 1) = 0.0;	A(0, 2) = 0.0;	A(0, 3) = 0.0;	A(0, 4) = 0.0;	A(0, 5) = 0.0;	A(0, 6) = 0.0;	A(0, 7) = 0.0;	A(0, 8) = 0.0;
	A(1, 0) = 0.0;	A(1, 1) = 0.0;	A(1, 2) = 0.0;	A(1, 3) = 0.0;	A(1, 4) = 1.0;	A(1, 5) = 0.0;	A(1, 6) = 0.0;	A(1, 7) = 0.0;	A(1, 8) = 0.0;
	A(2, 0) = 0.0;	A(2, 1) = 0.0;	A(2, 2) = 0.0;	A(2, 3) = 0.0;	A(2, 4) = 0.0;	A(2, 5) = 0.0;	A(2, 6) = 0.0;	A(2, 7) = 0.0;	A(2, 8) = 1.0;
	A(3, 0) = 0.0;	A(3, 1) = 1.0;	A(3, 2) = 0.0;	A(3, 3) = 1.0;	A(3, 4) = 0.0;	A(3, 5) = 0.0;	A(3, 6) = 0.0;	A(3, 7) = 0.0;	A(3, 8) = 0.0;
	A(4, 0) = 0.0;	A(4, 1) = 0.0;	A(4, 2) = 0.0;	A(4, 3) = 0.0;	A(4, 4) = 0.0;	A(4, 5) = 1.0;	A(4, 6) = 0.0;	A(4, 7) = 1.0;	A(4, 8) = 0.0;
	A(5, 0) = 0.0;	A(5, 1) = 0.0;	A(5, 2) = 1.0;	A(5, 3) = 0.0;	A(5, 4) = 0.0;	A(5, 5) = 0.0;	A(5, 6) = 1.0;	A(5, 7) = 0.0;	A(5, 8) = 0.0;

	int i = 0;
	for (auto pelement : this->pelements) {
		//----------パラメータの取得----------
		double rho = pelement->pparameter->parameters[this->refpf_to_us[0]];
		double Emax = pelement->pparameter->parameters[this->refpf_to_us[1]];
		double Emin = pelement->pparameter->parameters[this->refpf_to_us[2]];
		double V = pelement->pparameter->parameters[this->refpf_to_us[3]];

		//----------[D]の生成----------
		double dE = 3.0 * (Emax - Emin)*pow(rho, 2.0);
		Eigen::MatrixXd dD = Eigen::MatrixXd(6, 6);
		dD(0, 0) = 1.0 - V;	dD(0, 1) = V;		dD(0, 2) = V;		dD(0, 3) = 0.0;					dD(0, 4) = 0.0;					dD(0, 5) = 0.0;
		dD(1, 0) = V;		dD(1, 1) = 1.0 - V;	dD(1, 2) = V;		dD(1, 3) = 0.0;					dD(1, 4) = 0.0;					dD(1, 5) = 0.0;
		dD(2, 0) = V;		dD(2, 1) = V;		dD(2, 2) = 1.0 - V;	dD(2, 3) = 0.0;					dD(2, 4) = 0.0;					dD(2, 5) = 0.0;
		dD(3, 0) = 0.0;		dD(3, 1) = 0.0;		dD(3, 2) = 0.0;		dD(3, 3) = 0.5*(1.0 - 2.0*V);	dD(3, 4) = 0.0;					dD(3, 5) = 0.0;
		dD(4, 0) = 0.0;		dD(4, 1) = 0.0;		dD(4, 2) = 0.0;		dD(4, 3) = 0.0;					dD(4, 4) = 0.5*(1.0 - 2.0*V);	dD(4, 5) = 0.0;
		dD(5, 0) = 0.0;		dD(5, 1) = 0.0;		dD(5, 2) = 0.0;		dD(5, 3) = 0.0;					dD(5, 4) = 0.0;					dD(5, 5) = 0.5*(1.0 - 2.0*V);
		dD *= dE / ((1.0 - 2.0*V)*(1.0 + V));

		//----------要素毎のコンプライアンスを計算----------
		std::function<Eigen::VectorXd(std::vector<double>) > f = [=](std::vector<double> _xi) {
			Eigen::MatrixXd B = pelement->dTrialdx(this->refuf_to_us, _xi);
			Eigen::VectorXd u = pelement->u(this->refuf_to_us);
			Eigen::VectorXd c = -u.transpose()*B.transpose()*A.transpose()*dD*A*B*u*pelement->Jacobian(_xi).determinant();
			return c;
		};

		//----------要素毎の積分計算----------
		dcompliance.block(i*this->NOP, 0, this->NOP, 1) += this->pintegrations[pelement]->Integrate(f);
		i++;
	}

	return dcompliance;
}


double PANSFEM::ComplianceIsotropicElastic::value() {
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
		double rho = pelement->pparameter->parameters[this->refpf_to_us[0]];
		double Emax = pelement->pparameter->parameters[this->refpf_to_us[1]];
		double Emin = pelement->pparameter->parameters[this->refpf_to_us[2]];
		double V = pelement->pparameter->parameters[this->refpf_to_us[3]];

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