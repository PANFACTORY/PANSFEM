//*****************************************************************************
//Title		:PANSFEM/InversedAnalysis/Function/Compliance/Compliance.cpp
//Author	:Tanabe Yuta
//Date		:2019/07/02
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "Compliance.h"


PANSFEM::Compliance::Compliance() : Function(){}


PANSFEM::Compliance::~Compliance(){}


PANSFEM::Compliance::Compliance(std::vector<int> _plist, std::vector<int> _refulist, std::vector<int> _refplist) : Function(_plist, _refulist, _refplist, 1, 2, 5){}


Eigen::VectorXd PANSFEM::Compliance::sensitivitis(){
	Eigen::VectorXd dcompliance = Eigen::VectorXd::Zero(this->NOP*this->pelements.size());

	//----------[A]の生成----------
	Eigen::MatrixXd A = Eigen::MatrixXd(3, 4);
	A(0, 0) = 1.0;	A(0, 1) = 0.0;	A(0, 2) = 0.0;	A(0, 3) = 0.0;
	A(1, 0) = 0.0;	A(1, 1) = 0.0;	A(1, 2) = 0.0;	A(1, 3) = 1.0;
	A(2, 0) = 0.0;	A(2, 1) = 1.0;	A(2, 2) = 1.0;	A(2, 3) = 0.0;

	int i = 0;
	for (auto pelement : this->pelements) {
		//----------パラメータの取得----------
		double rho = pelement->parameters[this->refpf_to_us[0]];
		double Emax = pelement->parameters[this->refpf_to_us[1]];
		double Emin = pelement->parameters[this->refpf_to_us[2]];
		double V = pelement->parameters[this->refpf_to_us[3]];
		double t = pelement->parameters[this->refpf_to_us[4]];

		//----------[D]の生成----------
		double dE = 3.0 * (Emax - Emin)*pow(rho, 2.0);
		Eigen::MatrixXd dD = Eigen::MatrixXd(3, 3);
		dD(0, 0) = 1.0;	dD(0, 1) = V;	dD(0, 2) = 0.0;
		dD(1, 0) = V;	dD(1, 1) = 1.0;	dD(1, 2) = 0.0;
		dD(2, 0) = 0.0;	dD(2, 1) = 0.0;	dD(2, 2) = 0.5*(1.0 - V);
		dD *= dE / (1.0 - pow(V, 2.0));

		//----------要素毎のコンプライアンスを計算----------
		std::function<Eigen::VectorXd(std::vector<double>) > f = [=](std::vector<double> _xi) {
			Eigen::MatrixXd B = pelement->dTrialdx(this->refuf_to_us, _xi);
			Eigen::VectorXd u = pelement->u(this->refuf_to_us);
			Eigen::VectorXd c = -u.transpose()*B.transpose()*A.transpose()*dD*A*B*u*pelement->Jacobian(_xi).determinant()*t;
			return c;
		};
		
		//----------要素毎の積分計算----------
		dcompliance.block(i*this->NOP, 0, this->NOP, 1) += this->pintegrations[pelement]->Integrate(f);
		i++;
	}

	return dcompliance;
}


double PANSFEM::Compliance::value(){
	double compliance = 0.0;

	//----------[A]の生成----------
	Eigen::MatrixXd A = Eigen::MatrixXd(3, 4);
	A(0, 0) = 1.0;	A(0, 1) = 0.0;	A(0, 2) = 0.0;	A(0, 3) = 0.0;
	A(1, 0) = 0.0;	A(1, 1) = 0.0;	A(1, 2) = 0.0;	A(1, 3) = 1.0;
	A(2, 0) = 0.0;	A(2, 1) = 1.0;	A(2, 2) = 1.0;	A(2, 3) = 0.0;

	for (auto pelement : this->pelements) {
		//----------パラメータの取得----------
		double rho = pelement->parameters[this->refpf_to_us[0]];
		double Emax = pelement->parameters[this->refpf_to_us[1]];
		double Emin = pelement->parameters[this->refpf_to_us[2]];
		double V = pelement->parameters[this->refpf_to_us[3]];
		double t = pelement->parameters[this->refpf_to_us[4]];

		//----------[D]の生成----------
		double E = (Emax - Emin)*pow(rho, 3.0) + Emin;
		Eigen::MatrixXd D = Eigen::MatrixXd(3, 3);
		D(0, 0) = 1.0;	D(0, 1) = V;	D(0, 2) = 0.0;
		D(1, 0) = V;	D(1, 1) = 1.0;	D(1, 2) = 0.0;
		D(2, 0) = 0.0;	D(2, 1) = 0.0;	D(2, 2) = 0.5*(1.0 - V);
		D *= E / (1.0 - pow(V, 2.0));
 
		//----------要素毎のコンプライアンスを計算----------
		std::function<double(std::vector<double>) > f = [=](std::vector<double> _xi) {
			Eigen::MatrixXd B = pelement->dTrialdx(this->refuf_to_us, _xi);
			Eigen::VectorXd u = pelement->u(this->refuf_to_us);
			return (u.transpose()*B.transpose()*A.transpose()*D*A*B*u*pelement->Jacobian(_xi).determinant()*t)(0);
		};

		//----------要素毎の積分計算----------
		compliance += this->pintegrations[pelement]->Integrate(f);
	}

	return compliance;
}