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
		//----------パラメータの取得----------
		double t = pelement->pparameter->parameters[this->refpf_to_us[1]];

		//----------要素毎の重量感度計算----------
		std::function<double(std::vector<double>) > f = [=](std::vector<double> _xi) {
			return pelement->Jacobian(_xi).determinant()*t;
		};

		//----------要素毎の積分計算----------
		dweight[i] += this->pintegrations[pelement]->Integrate(f);
		i++;
	}

	return dweight;
}


double PANSFEM::Weight::value(){
	double weight = 0.0;
	for (auto pelement : this->pelements) {
		//----------パラメータの代入----------
		double rho = pelement->pparameter->parameters[this->refpf_to_us[0]];
		double t = pelement->pparameter->parameters[this->refpf_to_us[1]];

		//----------要素毎の重量計算式----------
		std::function<double(std::vector<double>) > f = [=](std::vector<double> _xi) {
			return (rho - 0.5)*pelement->Jacobian(_xi).determinant()*t;			//ここでは例として重量制約50%とする。
		};

		//----------要素毎の積分計算----------
		weight += this->pintegrations[pelement]->Integrate(f);
	}

	return weight;
}