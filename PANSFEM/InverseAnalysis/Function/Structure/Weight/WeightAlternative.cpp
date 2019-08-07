//*****************************************************************************
//Title		:PANSFEM/InversedAnalysis/Function/Weight/WeightAlternative.cpp
//Author	:Tanabe Yuta
//Date		:2019/08/03
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "WeightAlternative.h"


PANSFEM::WeightAlternative::WeightAlternative() : Function(){}


PANSFEM::WeightAlternative::~WeightAlternative(){}


PANSFEM::WeightAlternative::WeightAlternative(std::vector<int> _plist, std::vector<int> _refulist, std::vector<int> _refplist) : Function(_plist, _refulist, _refplist, 1, 0, 6){}


Eigen::VectorXd PANSFEM::WeightAlternative::sensitivitis(){
	Eigen::VectorXd dweight = Eigen::VectorXd::Zero(this->NOP*this->pelements.size());

	int i = 0;
	for (auto pelement : this->pelements) {
		//----------パラメータの取得----------
		double s = pelement->parameters[this->refpf_to_us[0]];
		double l = pelement->parameters[this->refpf_to_us[1]];
		double rho = pelement->parameters[this->refpf_to_us[2]];
		double dmax = pelement->parameters[this->refpf_to_us[3]];
		double dmin = pelement->parameters[this->refpf_to_us[4]];
		double t = pelement->parameters[this->refpf_to_us[5]];

		double d = (dmax - dmin)*s + dmin;

		//----------要素毎の感度計算----------
		dweight[i] += 4.0*(dmax - dmin)*(l - d)*t*rho;
		i++;
	}

	return dweight;
}


double PANSFEM::WeightAlternative::value(){
	double weight = 0.0;
	for (auto pelement : this->pelements) {
		//----------パラメータの代入----------
		double s = pelement->parameters[this->refpf_to_us[0]];
		double l = pelement->parameters[this->refpf_to_us[1]];
		double rho = pelement->parameters[this->refpf_to_us[2]];
		double dmax = pelement->parameters[this->refpf_to_us[3]];
		double dmin = pelement->parameters[this->refpf_to_us[4]];
		double t = pelement->parameters[this->refpf_to_us[5]];

		double d = (dmax - dmin)*s + dmin;
		double w = 2.0*(2.0*l - d)*d*t*rho;
		double wmax = 2.0*(2.0*l - dmax)*dmax*t*rho;

		//----------要素毎の重量計算式----------
		std::function<double(std::vector<double>) > f = [=](std::vector<double> _xi) {
			return rho*pelement->Jacobian(_xi).determinant()*t;			//ここでは例として重量制約50%とする。
		};

		//----------要素毎の重量計算----------
		//weight += (w - 0.5*wmax);
		weight += w - 0.7*this->pintegrations[pelement]->Integrate(f);
	}

	return weight;
}