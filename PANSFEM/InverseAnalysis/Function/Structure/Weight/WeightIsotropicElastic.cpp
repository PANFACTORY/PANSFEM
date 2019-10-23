//*************************************************************************************************
//Title		:PANSFEM/InversedAnalysis/Function/WeightIsotropicElastic/WeightIsotropicElastic.cpp
//Author	:Tanabe Yuta
//Date		:2019/09/22
//Copyright	:(C)2019 TanabeYuta
//*************************************************************************************************


#include "WeightIsotropicElastic.h"

PANSFEM::WeightIsotropicElastic::WeightIsotropicElastic() : Function() {}


PANSFEM::WeightIsotropicElastic::~WeightIsotropicElastic() {}


PANSFEM::WeightIsotropicElastic::WeightIsotropicElastic(std::vector<int> _plist, std::vector<int> _refulist, std::vector<int> _refplist) : Function(_plist, _refulist, _refplist, 1, 0, 1) {}


Eigen::VectorXd PANSFEM::WeightIsotropicElastic::sensitivitis() {
	Eigen::VectorXd dweight = Eigen::VectorXd::Zero(this->NOP*this->parametersindex.size());

	for (auto pelement : this->pelements) {
		//----------�p�����[�^�̎擾----------

		//----------�v�f���̏d�ʊ��x�v�Z----------
		std::function<double(std::vector<double>) > f = [=](std::vector<double> _xi) {
			return pelement->Jacobian(_xi).determinant();
		};

		//----------�v�f���̐ϕ��v�Z----------
		dweight[this->parametersindex[pelement->pparameter]] += this->pintegrations[pelement]->Integrate(f);
	}

	return dweight;
}


double PANSFEM::WeightIsotropicElastic::value() {
	double weight = 0.0;
	for (auto pelement : this->pelements) {
		//----------�p�����[�^�̑��----------
		double rho = pelement->pparameter->parameters[this->refpf_to_us[0]];

		//----------�v�f���̏d�ʌv�Z��----------
		std::function<double(std::vector<double>) > f = [=](std::vector<double> _xi) {
			return (rho - 0.1)*pelement->Jacobian(_xi).determinant();			//�����ł͗�Ƃ��ďd�ʐ���50%�Ƃ���B
		};

		//----------�v�f���̐ϕ��v�Z----------
		weight += this->pintegrations[pelement]->Integrate(f);
	}

	return weight;
}