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

	//----------[A]�̐���----------
	Eigen::MatrixXd A = Eigen::MatrixXd(3, 4);
	A(0, 0) = 1.0;	A(0, 1) = 0.0;	A(0, 2) = 0.0;	A(0, 3) = 0.0;
	A(1, 0) = 0.0;	A(1, 1) = 0.0;	A(1, 2) = 0.0;	A(1, 3) = 1.0;
	A(2, 0) = 0.0;	A(2, 1) = 1.0;	A(2, 2) = 1.0;	A(2, 3) = 0.0;

	for (auto pelement : this->pelements) {
		//----------[D]�̐���----------
		double V = pelement->parameters[this->refpf_to_us[3]];
		double dE = 3.0 * (pelement->parameters[this->refpf_to_us[1]] - pelement->parameters[this->refpf_to_us[2]])*pow(pelement->parameters[this->refpf_to_us[0]], 2.0);
		Eigen::MatrixXd dD = Eigen::MatrixXd(3, 3);
		dD(0, 0) = 1.0;	dD(0, 1) = V;	dD(0, 2) = 0.0;
		dD(1, 0) = V;	dD(1, 1) = 1.0;	dD(1, 2) = 0.0;
		dD(2, 0) = 0.0;	dD(2, 1) = 0.0;	dD(2, 2) = 0.5*(1.0 - V);
		dD *= dE / (1.0 - pow(V, 2.0));

		//----------�v�f���̃R���v���C�A���X���v�Z----------
		std::function<double(std::vector<double>) > f = [=](std::vector<double> _xi) {
			Eigen::MatrixXd B = pelement->dTrialdx(this->refuf_to_us, _xi);
			Eigen::VectorXd u = pelement->u(this->refuf_to_us);
			return u.transpose()*B.transpose()*A.transpose()*dD*A*B*u;
		};

		//----------�v�f���̐ϕ��v�Z----------
		dcompliance += this->pintegrations[pelement]->Integrate(f);
	}

	return Eigen::VectorXd();
}


double PANSFEM::Compliance::value(){
	double compliance = 0.0;

	//----------[A]�̐���----------
	Eigen::MatrixXd A = Eigen::MatrixXd(3, 4);
	A(0, 0) = 1.0;	A(0, 1) = 0.0;	A(0, 2) = 0.0;	A(0, 3) = 0.0;
	A(1, 0) = 0.0;	A(1, 1) = 0.0;	A(1, 2) = 0.0;	A(1, 3) = 1.0;
	A(2, 0) = 0.0;	A(2, 1) = 1.0;	A(2, 2) = 1.0;	A(2, 3) = 0.0;

	for (auto pelement : this->pelements) {
		//----------[D]�̐���----------
		double V = pelement->parameters[this->refpf_to_us[3]];
		double E = (pelement->parameters[this->refpf_to_us[1]] - pelement->parameters[this->refpf_to_us[2]])*pow(pelement->parameters[this->refpf_to_us[0]], 3.0) + pelement->parameters[this->refpf_to_us[2]];
		Eigen::MatrixXd D = Eigen::MatrixXd(3, 3);
		D(0, 0) = 1.0;	D(0, 1) = V;	D(0, 2) = 0.0;
		D(1, 0) = V;	D(1, 1) = 1.0;	D(1, 2) = 0.0;
		D(2, 0) = 0.0;	D(2, 1) = 0.0;	D(2, 2) = 0.5*(1.0 - V);
		D *= E / (1.0 - pow(V, 2.0));
 
		//----------�v�f���̃R���v���C�A���X���v�Z----------
		std::function<double(std::vector<double>) > f = [=](std::vector<double> _xi) {
			Eigen::MatrixXd B = pelement->dTrialdx(this->refuf_to_us, _xi);
			Eigen::VectorXd u = pelement->u(this->refuf_to_us);
			return u.transpose()*B.transpose()*A.transpose()*D*A*B*u; 
		};

		//----------�v�f���̐ϕ��v�Z----------
		compliance += this->pintegrations[pelement]->Integrate(f);
	}

	return compliance;
}