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
		//----------パラメータの取得----------

		//----------要素毎の重量感度計算----------
		std::function<double(std::vector<double>) > f = [=](std::vector<double> _xi) {
			return pelement->Jacobian(_xi).determinant();
		};

		//----------要素毎の積分計算----------
		dweight[this->parametersindex[pelement->pparameter]] += this->pintegrations[pelement]->Integrate(f);
	}

	return dweight;
}


double PANSFEM::WeightIsotropicElastic::value() {
	double weight = 0.0;
	for (auto pelement : this->pelements) {
		//----------パラメータの代入----------
		double rho = pelement->pparameter->parameters[this->refpf_to_us[0]];

		//----------要素毎の重量計算式----------
		std::function<double(std::vector<double>) > f = [=](std::vector<double> _xi) {
			return (rho - 0.1)*pelement->Jacobian(_xi).determinant();			//ここでは例として重量制約50%とする。
		};

		//----------要素毎の積分計算----------
		weight += this->pintegrations[pelement]->Integrate(f);
	}

	return weight;
}