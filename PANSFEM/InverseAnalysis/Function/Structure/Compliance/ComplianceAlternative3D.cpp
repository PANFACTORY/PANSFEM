//*************************************************************************************************
//Title		:PANSFEM/InversedAnalysis/Function/ComplianceAlternative3D/ComplianceAlternative3D.cpp
//Author	:Tanabe Yuta
//Date		:2019/09/27
//Copyright	:(C)2019 TanabeYuta
//*************************************************************************************************


#include "ComplianceAlternative3D.h"


PANSFEM::ComplianceAlternative3D::ComplianceAlternative3D() : Function() {}


PANSFEM::ComplianceAlternative3D::~ComplianceAlternative3D() {} 


PANSFEM::ComplianceAlternative3D::ComplianceAlternative3D(std::vector<int> _plist, std::vector<int> _refulist, std::vector<int> _refplist) : Function(_plist, _refulist, _refplist, 1, 3, 5) {}


Eigen::VectorXd PANSFEM::ComplianceAlternative3D::sensitivitis() {
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
		double dmax = pelement->pparameter->parameters[this->refpf_to_us[1]];
		double dmin = pelement->pparameter->parameters[this->refpf_to_us[2]];
		double L = pelement->pparameter->parameters[this->refpf_to_us[3]];
		double E = pelement->pparameter->parameters[this->refpf_to_us[4]];

		double d = (dmax - dmin)*s + dmin;
		double ddds = dmax - dmin;
		double dl = d / L;
		double dEeq = 2.0*sqrt(3.0)*3.141592*E*pow(dl, 3.0)*ddds/L;
		double Veq = 0.499;

		double dDii = dEeq * (1.0 - Veq) / ((1.0 - 2.0*Veq)*(1.0 + Veq));
		double dDij = dEeq * Veq / ((1.0 - 2.0*Veq)*(1.0 + Veq));
		double dGii = (2.5*3.141592*E/sqrt(3.0))*pow(dl, 3.0)*ddds/L;

		//----------[D]の生成----------
		Eigen::MatrixXd dD = Eigen::MatrixXd(6, 6);
		dD(0, 0) = dDii;	dD(0, 1) = dDij;	dD(0, 2) = dDij;	dD(0, 3) = 0.0;		dD(0, 4) = 0.0;		dD(0, 5) = 0.0;
		dD(1, 0) = dDij;	dD(1, 1) = dDii;	dD(1, 2) = dDij;	dD(1, 3) = 0.0;		dD(1, 4) = 0.0;		dD(1, 5) = 0.0;
		dD(2, 0) = dDij;	dD(2, 1) = dDij;	dD(2, 2) = dDii;	dD(2, 3) = 0.0;		dD(2, 4) = 0.0;		dD(2, 5) = 0.0;
		dD(3, 0) = 0.0;		dD(3, 1) = 0.0;		dD(3, 2) = 0.0;		dD(3, 3) = dGii;	dD(3, 4) = 0.0;		dD(3, 5) = 0.0;
		dD(4, 0) = 0.0;		dD(4, 1) = 0.0;		dD(4, 2) = 0.0;		dD(4, 3) = 0.0;		dD(4, 4) = dGii;	dD(4, 5) = 0.0;
		dD(5, 0) = 0.0;		dD(5, 1) = 0.0;		dD(5, 2) = 0.0;		dD(5, 3) = 0.0;		dD(5, 4) = 0.0;		dD(5, 5) = dGii;

		//----------要素毎のコンプライアンスを計算----------
		std::function<Eigen::VectorXd(std::vector<double>) > f = [=](std::vector<double> _xi) {
			Eigen::MatrixXd B = pelement->dTrialdx(this->refuf_to_us, _xi);
			Eigen::VectorXd u = pelement->u(this->refuf_to_us);
			Eigen::VectorXd c = -u.transpose()*B.transpose()*A.transpose()*dD*A*B*u*pelement->Jacobian(_xi).determinant();
			return c;
		};

		//----------要素毎の積分計算----------
		dcompliance.block(this->parametersindex[pelement->pparameter] * this->NOP, 0, this->NOP, 1) += this->pintegrations[pelement]->Integrate(f);
	}

	return dcompliance;
}


double PANSFEM::ComplianceAlternative3D::value() {
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
		double dmax = pelement->pparameter->parameters[this->refpf_to_us[1]];
		double dmin = pelement->pparameter->parameters[this->refpf_to_us[2]];
		double L = pelement->pparameter->parameters[this->refpf_to_us[3]];
		double E = pelement->pparameter->parameters[this->refpf_to_us[4]];

		double d = (dmax - dmin)*s + dmin;
		double dl = d / L;
		double Eeq = 0.5*sqrt(3.0)*3.141592*E*pow(dl, 4.0);
		double Veq = 0.499;

		double Dii = Eeq * (1.0 - Veq) / ((1.0 - 2.0*Veq)*(1.0 + Veq));
		double Dij = Eeq * Veq / ((1.0 - 2.0*Veq)*(1.0 + Veq));
		double Gii = (3.141592*E / (4.0*sqrt(3.0)))*pow(dl, 4.0);

		//----------[D]の生成----------
		Eigen::MatrixXd D = Eigen::MatrixXd(6, 6);
		D(0, 0) = Dii;	D(0, 1) = Dij;	D(0, 2) = Dij;	D(0, 3) = 0.0;	D(0, 4) = 0.0;	D(0, 5) = 0.0;
		D(1, 0) = Dij;	D(1, 1) = Dii;	D(1, 2) = Dij;	D(1, 3) = 0.0;	D(1, 4) = 0.0;	D(1, 5) = 0.0;
		D(2, 0) = Dij;	D(2, 1) = Dij;	D(2, 2) = Dii;	D(2, 3) = 0.0;	D(2, 4) = 0.0;	D(2, 5) = 0.0;
		D(3, 0) = 0.0;	D(3, 1) = 0.0;	D(3, 2) = 0.0;	D(3, 3) = Gii;	D(3, 4) = 0.0;	D(3, 5) = 0.0;
		D(4, 0) = 0.0;	D(4, 1) = 0.0;	D(4, 2) = 0.0;	D(4, 3) = 0.0;	D(4, 4) = Gii;	D(4, 5) = 0.0;
		D(5, 0) = 0.0;	D(5, 1) = 0.0;	D(5, 2) = 0.0;	D(5, 3) = 0.0;	D(5, 4) = 0.0;	D(5, 5) = Gii;

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
