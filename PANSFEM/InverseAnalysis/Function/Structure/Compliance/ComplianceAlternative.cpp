//*************************************************************************************************
//Title		:PANSFEM/InversedAnalysis/Function/ComplianceAlternative/ComplianceAlternative.cpp
//Author	:Tanabe Yuta
//Date		:2019/08/03
//Copyright	:(C)2019 TanabeYuta
//*************************************************************************************************


#include "ComplianceAlternative.h"


PANSFEM::ComplianceAlternative::ComplianceAlternative() : Function(){}


PANSFEM::ComplianceAlternative::~ComplianceAlternative(){}


PANSFEM::ComplianceAlternative::ComplianceAlternative(std::vector<int> _plist, std::vector<int> _refulist, std::vector<int> _refplist) : Function(_plist, _refulist, _refplist, 1, 2, 6){}


Eigen::VectorXd PANSFEM::ComplianceAlternative::sensitivitis(){
	Eigen::VectorXd dcompliance = Eigen::VectorXd::Zero(this->NOP*this->parametersindex.size());

	//----------[A]の生成----------
	Eigen::MatrixXd A = Eigen::MatrixXd(3, 4);
	A(0, 0) = 1.0;	A(0, 1) = 0.0;	A(0, 2) = 0.0;	A(0, 3) = 0.0;
	A(1, 0) = 0.0;	A(1, 1) = 0.0;	A(1, 2) = 0.0;	A(1, 3) = 1.0;
	A(2, 0) = 0.0;	A(2, 1) = 1.0;	A(2, 2) = 1.0;	A(2, 3) = 0.0;

	for (auto pelement : this->pelements) {
		//----------パラメータの取得----------
		double s = pelement->pparameter->parameters[this->refpf_to_us[0]];
		double E = pelement->pparameter->parameters[this->refpf_to_us[1]];
		double l = pelement->pparameter->parameters[this->refpf_to_us[2]];
		double dmax = pelement->pparameter->parameters[this->refpf_to_us[3]];
		double dmin = pelement->pparameter->parameters[this->refpf_to_us[4]];
		double t = pelement->pparameter->parameters[this->refpf_to_us[5]];

		double d = (dmax - dmin)*s + dmin;
		double dl = d / l;
		double dl2 = pow(dl, 3.0);
		double onepdl2 = 1.0 + dl2;
		double onemdl2 = 1.0 - dl2;
		double dml = (dmax - dmin) / l;

		//----------[D]の生成----------
		Eigen::MatrixXd dD = Eigen::MatrixXd(3, 3);
		dD(0, 0) = onepdl2;	dD(0, 1) = onemdl2;	dD(0, 2) = 0.0;
		dD(1, 0) = onemdl2;	dD(1, 1) = onepdl2;	dD(1, 2) = 0.0;
		dD(2, 0) = 0.0;		dD(2, 1) = 0.0;		dD(2, 2) = onepdl2;
		dD *= E*dml;

		//----------要素毎のコンプライアンスを計算----------
		std::function<Eigen::VectorXd(std::vector<double>) > f = [=](std::vector<double> _xi) {
			Eigen::MatrixXd B = pelement->dTrialdx(this->refuf_to_us, _xi);
			Eigen::VectorXd u = pelement->u(this->refuf_to_us);
			Eigen::VectorXd c = -u.transpose()*B.transpose()*A.transpose()*dD*A*B*u*pelement->Jacobian(_xi).determinant()*t;
			return c;
		};

		//----------要素毎の積分計算----------
		dcompliance.block(this->parametersindex[pelement->pparameter]*this->NOP, 0, this->NOP, 1) += this->pintegrations[pelement]->Integrate(f);
	}

	return dcompliance;
}


double PANSFEM::ComplianceAlternative::value(){
	double compliance = 0.0;

	//----------[A]の生成----------
	Eigen::MatrixXd A = Eigen::MatrixXd(3, 4);
	A(0, 0) = 1.0;	A(0, 1) = 0.0;	A(0, 2) = 0.0;	A(0, 3) = 0.0;
	A(1, 0) = 0.0;	A(1, 1) = 0.0;	A(1, 2) = 0.0;	A(1, 3) = 1.0;
	A(2, 0) = 0.0;	A(2, 1) = 1.0;	A(2, 2) = 1.0;	A(2, 3) = 0.0;

	for (auto pelement : this->pelements) {
		//----------パラメータの取得----------
		double s = pelement->pparameter->parameters[this->refpf_to_us[0]];
		double E = pelement->pparameter->parameters[this->refpf_to_us[1]];
		double l = pelement->pparameter->parameters[this->refpf_to_us[2]];
		double dmax = pelement->pparameter->parameters[this->refpf_to_us[3]];
		double dmin = pelement->pparameter->parameters[this->refpf_to_us[4]];
		double t = pelement->pparameter->parameters[this->refpf_to_us[5]];

		double d = (dmax - dmin)*s + dmin;
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