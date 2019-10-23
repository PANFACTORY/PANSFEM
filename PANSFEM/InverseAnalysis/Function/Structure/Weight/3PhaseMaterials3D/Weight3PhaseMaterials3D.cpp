//***********************************************************************************************************
//Title		:PANSFEM/InversedAnalysis/Function/Weight/3PhaseMaterials3D/Weight3PhaseMaterials3D.cpp
//Author	:Tanabe Yuta
//Date		:2019/10/14
//Copyright	:(C)2019 TanabeYuta
//***********************************************************************************************************


#include "Weight3PhaseMaterials3D.h"


PANSFEM::Weight3PhaseMaterials3D::Weight3PhaseMaterials3D() : Function() {}


PANSFEM::Weight3PhaseMaterials3D::~Weight3PhaseMaterials3D() {}


PANSFEM::Weight3PhaseMaterials3D::Weight3PhaseMaterials3D(std::vector<int> _plist, std::vector<int> _refulist, std::vector<int> _refplist) : Function(_plist, _refulist, _refplist, 2, 0, 5) {}


Eigen::VectorXd PANSFEM::Weight3PhaseMaterials3D::sensitivitis() {
	Eigen::VectorXd dweight = Eigen::VectorXd::Zero(this->NOP*this->parametersindex.size());

	for (auto pelement : this->pelements) {
		//----------�p�����[�^�̎擾----------
		double s0 = pelement->pparameter->parameters[this->refpf_to_us[0]];
		double s1 = pelement->pparameter->parameters[this->refpf_to_us[1]];
		double rho0 = pelement->pparameter->parameters[this->refpf_to_us[2]];
		double rho1 = pelement->pparameter->parameters[this->refpf_to_us[3]];
		double rho2 = pelement->pparameter->parameters[this->refpf_to_us[4]];

		Eigen::VectorXd dwds = Eigen::VectorXd(2);
		dwds(0) = -rho0 + rho1 * (1.0 - s1) + rho2 * s1;
		dwds(1) = (-rho1 + rho2)*s0;

		//----------�v�f���̏d�ʊ��x�v�Z----------
		std::function<double(std::vector<double>) > f = [=](std::vector<double> _xi) {
			return pelement->Jacobian(_xi).determinant();
		};
		
		//----------�v�f���̐ϕ��v�Z----------
		dweight.segment(this->parametersindex[pelement->pparameter]*this->NOP, 2) += dwds * this->pintegrations[pelement]->Integrate(f);
	}

	return dweight;
}


double PANSFEM::Weight3PhaseMaterials3D::value() {
	double weight = 0.0;
	for (auto pelement : this->pelements) {
		//----------�p�����[�^�̑��----------
		double s0 = pelement->pparameter->parameters[this->refpf_to_us[0]];
		double s1 = pelement->pparameter->parameters[this->refpf_to_us[1]];
		double rho0 = pelement->pparameter->parameters[this->refpf_to_us[2]];
		double rho1 = pelement->pparameter->parameters[this->refpf_to_us[3]];
		double rho2 = pelement->pparameter->parameters[this->refpf_to_us[4]];

		//----------�v�f���̏d�ʌv�Z��----------
		std::function<double(std::vector<double>) > f = [=](std::vector<double> _xi) {
			return ((rho0*(1.0 - s0) + (rho1*(1.0 - s1) + rho2 * s1)*s0) - 0.3) * pelement->Jacobian(_xi).determinant();			//�����ł͗�Ƃ��ďd�ʐ���3%�Ƃ���B
		};

		//----------�v�f���̐ϕ��v�Z----------
		weight += this->pintegrations[pelement]->Integrate(f);
	}

	return weight;
}