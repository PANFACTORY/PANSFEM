//***********************************************************************************************************
//Title		:PANSFEM/InversedAnalysis/Function/ComplianceAlternative3Phase/ComplianceAlternative3Phase.cpp
//Author	:Tanabe Yuta
//Date		:2019/08/07
//Copyright	:(C)2019 TanabeYuta
//***********************************************************************************************************


#include "ComplianceAlternative3Phase.h"


PANSFEM::ComplianceAlternative3Phase::ComplianceAlternative3Phase() : Function() {}


PANSFEM::ComplianceAlternative3Phase::~ComplianceAlternative3Phase() {}


PANSFEM::ComplianceAlternative3Phase::ComplianceAlternative3Phase(std::vector<int> _plist, std::vector<int> _refulist, std::vector<int> _refplist) : Function(_plist, _refulist, _refplist, 2, 2, 8) {}


Eigen::VectorXd PANSFEM::ComplianceAlternative3Phase::sensitivitis() {
	Eigen::VectorXd dcompliance = Eigen::VectorXd::Zero(this->NOP*this->pelements.size());

	//----------[A]の生成----------
	Eigen::MatrixXd A = Eigen::MatrixXd(3, 4);
	A(0, 0) = 1.0;	A(0, 1) = 0.0;	A(0, 2) = 0.0;	A(0, 3) = 0.0;
	A(1, 0) = 0.0;	A(1, 1) = 0.0;	A(1, 2) = 0.0;	A(1, 3) = 1.0;
	A(2, 0) = 0.0;	A(2, 1) = 1.0;	A(2, 2) = 1.0;	A(2, 3) = 0.0;

	int i = 0;
	for (auto pelement : this->pelements) {
		//----------パラメータの取得----------
		double s0 = pelement->parameters[this->refpf_to_us[0]];
		double s1 = pelement->parameters[this->refpf_to_us[1]];
		double E = pelement->parameters[this->refpf_to_us[2]];
		double l = pelement->parameters[this->refpf_to_us[3]];
		double dmax = pelement->parameters[this->refpf_to_us[4]];
		double dmin = pelement->parameters[this->refpf_to_us[5]];
		double d0 = pelement->parameters[this->refpf_to_us[6]];
		double t = pelement->parameters[this->refpf_to_us[7]];

		double d = (((dmax - dmin)*s1 + dmin) - d0)*s0 + d0;
		double dl = d / l;
		double dl2 = pow(dl, 3.0);
		double onepdl2 = 1.0 + dl2;
		double onemdl2 = 1.0 - dl2;

		Eigen::MatrixXd ddds = Eigen::MatrixXd::Zero(2, 1);
		ddds(0, 0) = (dmax - dmin)*s1 + dmin - d0;	ddds(1, 0) = (dmax - dmin)*s0;

		//----------[D]の生成----------
		Eigen::MatrixXd dD = Eigen::MatrixXd(3, 3);
		dD(0, 0) = onepdl2;	dD(0, 1) = onemdl2;	dD(0, 2) = 0.0;
		dD(1, 0) = onemdl2;	dD(1, 1) = onepdl2;	dD(1, 2) = 0.0;
		dD(2, 0) = 0.0;		dD(2, 1) = 0.0;		dD(2, 2) = onepdl2;
		dD *= E / l;

		//----------要素毎のコンプライアンスを計算----------
		std::function<Eigen::VectorXd(std::vector<double>) > f = [=](std::vector<double> _xi) {
			Eigen::MatrixXd B = pelement->dTrialdx(this->refuf_to_us, _xi);
			Eigen::VectorXd u = pelement->u(this->refuf_to_us);
			Eigen::VectorXd c = -u.transpose()*B.transpose()*A.transpose()*dD*A*B*u*pelement->Jacobian(_xi).determinant()*t;
			return c;
		};

		//----------要素毎の積分計算----------
		dcompliance.block(i*this->NOP, 0, this->NOP, 1) += ddds * this->pintegrations[pelement]->Integrate(f);
		i++;
	}

	return dcompliance;
}


double PANSFEM::ComplianceAlternative3Phase::value() {
	double compliance = 0.0;

	//----------[A]の生成----------
	Eigen::MatrixXd A = Eigen::MatrixXd(3, 4);
	A(0, 0) = 1.0;	A(0, 1) = 0.0;	A(0, 2) = 0.0;	A(0, 3) = 0.0;
	A(1, 0) = 0.0;	A(1, 1) = 0.0;	A(1, 2) = 0.0;	A(1, 3) = 1.0;
	A(2, 0) = 0.0;	A(2, 1) = 1.0;	A(2, 2) = 1.0;	A(2, 3) = 0.0;

	for (auto pelement : this->pelements) {
		//----------パラメータの取得----------
		double s0 = pelement->parameters[this->refpf_to_us[0]];
		double s1 = pelement->parameters[this->refpf_to_us[1]];
		double E = pelement->parameters[this->refpf_to_us[2]];
		double l = pelement->parameters[this->refpf_to_us[3]];
		double dmax = pelement->parameters[this->refpf_to_us[4]];
		double dmin = pelement->parameters[this->refpf_to_us[5]];
		double d0 = pelement->parameters[this->refpf_to_us[6]];
		double t = pelement->parameters[this->refpf_to_us[7]];

		double d = (((dmax - dmin)*s1 + dmin) - d0)*s0 + d0;
		double dl = d / l;
		double dl3 = pow(dl, 3.0);
		double dlpdl3 = dl + dl3;
		double dlmdl3 = dl - dl3;

		//----------[D]の生成----------
		Eigen::MatrixXd D = Eigen::MatrixXd(3, 3);
		D(0, 0) = dlpdl3;	D(0, 1) = dlmdl3;	D(0, 2) = 0.0;
		D(1, 0) = dlmdl3;	D(1, 1) = dlpdl3;	D(1, 2) = 0.0;
		D(2, 0) = 0.0;		D(2, 1) = 0.0;		D(2, 2) = dlpdl3;
		D *= E;

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