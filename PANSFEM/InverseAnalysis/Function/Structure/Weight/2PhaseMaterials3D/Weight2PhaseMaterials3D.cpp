//***********************************************************************************************************
//Title		:PANSFEM/InversedAnalysis/Function/Weight/2PhaseMaterials3D/Weight2PhaseMaterials3D.cpp
//Author	:Tanabe Yuta
//Date		:2019/10/22
//Copyright	:(C)2019 TanabeYuta
//***********************************************************************************************************


#include "Weight2PhaseMaterials3D.h"


PANSFEM::Weight2PhaseMaterials3D::Weight2PhaseMaterials3D() : Function() {}


PANSFEM::Weight2PhaseMaterials3D::~Weight2PhaseMaterials3D() {}


PANSFEM::Weight2PhaseMaterials3D::Weight2PhaseMaterials3D(std::vector<int> _plist, std::vector<int> _refulist, std::vector<int> _refplist) : Function(_plist, _refulist, _refplist, 1, 0, 3) {}


Eigen::VectorXd PANSFEM::Weight2PhaseMaterials3D::sensitivitis() {
	Eigen::VectorXd dweight = Eigen::VectorXd::Zero(this->NOP*this->parametersindex.size());

	for (auto pelement : this->pelements) {
		//----------�p�����[�^�̎擾----------
		double s = pelement->pparameter->parameters[this->refpf_to_us[0]];
		double rho0 = pelement->pparameter->parameters[this->refpf_to_us[1]];
		double rho1 = pelement->pparameter->parameters[this->refpf_to_us[2]];

		double drhods = -rho0 + rho1;

		//----------�v�f���̏d�ʊ��x�v�Z----------
		std::function<double(std::vector<double>) > f = [=](std::vector<double> _xi) {
			return pelement->Jacobian(_xi).determinant();
		};

		//----------�v�f���̐ϕ��v�Z----------
		dweight[this->parametersindex[pelement->pparameter] * this->NOP] += drhods * this->pintegrations[pelement]->Integrate(f);
	}

	return dweight;
}


double PANSFEM::Weight2PhaseMaterials3D::value() {
	double weight = 0.0;
	for (auto pelement : this->pelements) {
		//----------�p�����[�^�̑��----------
		double s = pelement->pparameter->parameters[this->refpf_to_us[0]];
		double rho0 = pelement->pparameter->parameters[this->refpf_to_us[1]];
		double rho1 = pelement->pparameter->parameters[this->refpf_to_us[2]];
		
		//----------�v�f���̏d�ʌv�Z��----------
		std::function<double(std::vector<double>) > f = [=](std::vector<double> _xi) {
			return ((rho0*(1.0 - s) + rho1*s) - 0.1) * pelement->Jacobian(_xi).determinant();			//�����ł͗�Ƃ��ďd�ʐ���3%�Ƃ���B
		};

		//----------�v�f���̐ϕ��v�Z----------
		weight += this->pintegrations[pelement]->Integrate(f);
	}

	return weight;
}