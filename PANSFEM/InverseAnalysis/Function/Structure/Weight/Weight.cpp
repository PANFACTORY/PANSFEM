//*****************************************************************************
//Title		:PANSFEM/InversedAnalysis/Function/Weight/Weight.h
//Author	:Tanabe Yuta
//Date		:2019/07/02
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "Weight.h"


PANSFEM::Weight::Weight() : Function(){}


PANSFEM::Weight::~Weight(){}


PANSFEM::Weight::Weight(std::vector<int> _plist, std::vector<int> _refulist, std::vector<int> _refplist) : Function(_plist, _refulist, _refplist, 1, 0, 2){}


Eigen::VectorXd PANSFEM::Weight::sensitivitis(){
	Eigen::VectorXd dweight = Eigen::VectorXd::Zero(this->NOP*this->pelements.size());

	int i = 0;
	for (auto pelement : this->pelements) {
		//----------�p�����[�^�̎擾----------
		double t = pelement->pparameter->parameters[this->refpf_to_us[1]];

		//----------�v�f���̏d�ʊ��x�v�Z----------
		std::function<double(std::vector<double>) > f = [=](std::vector<double> _xi) {
			return pelement->Jacobian(_xi).determinant()*t;
		};

		//----------�v�f���̐ϕ��v�Z----------
		dweight[i] += this->pintegrations[pelement]->Integrate(f);
		i++;
	}

	return dweight;
}


double PANSFEM::Weight::value(){
	double weight = 0.0;
	for (auto pelement : this->pelements) {
		//----------�p�����[�^�̑��----------
		double rho = pelement->pparameter->parameters[this->refpf_to_us[0]];
		double t = pelement->pparameter->parameters[this->refpf_to_us[1]];

		//----------�v�f���̏d�ʌv�Z��----------
		std::function<double(std::vector<double>) > f = [=](std::vector<double> _xi) {
			return (rho - 0.5)*pelement->Jacobian(_xi).determinant()*t;			//�����ł͗�Ƃ��ďd�ʐ���50%�Ƃ���B
		};

		//----------�v�f���̐ϕ��v�Z----------
		weight += this->pintegrations[pelement]->Integrate(f);
	}

	return weight;
}