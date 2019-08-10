//***************************************************************************************
//Title		:PANSFEM/InversedAnalysis/Function/Weight/WeightAlternative3Phase.cpp
//Author	:Tanabe Yuta
//Date		:2019/08/07
//Copyright	:(C)2019 TanabeYuta
//***************************************************************************************


#include "WeightAlternative3Phase.h"


PANSFEM::WeightAlternative3Phase::WeightAlternative3Phase() : Function() {}


PANSFEM::WeightAlternative3Phase::~WeightAlternative3Phase() {}


PANSFEM::WeightAlternative3Phase::WeightAlternative3Phase(std::vector<int> _plist, std::vector<int> _refulist, std::vector<int> _refplist) : Function(_plist, _refulist, _refplist, 2, 0, 8) {}


Eigen::VectorXd PANSFEM::WeightAlternative3Phase::sensitivitis() {
	Eigen::VectorXd dweight = Eigen::VectorXd::Zero(this->NOP*this->pelements.size());

	int i = 0;
	for (auto pelement : this->pelements) {
		//----------パラメータの取得----------
		double s0 = pelement->parameters[this->refpf_to_us[0]];
		double s1 = pelement->parameters[this->refpf_to_us[1]];
		double l = pelement->parameters[this->refpf_to_us[2]];
		double rho = pelement->parameters[this->refpf_to_us[3]];
		double dmax = pelement->parameters[this->refpf_to_us[4]];
		double dmin = pelement->parameters[this->refpf_to_us[5]];
		double d0 = pelement->parameters[this->refpf_to_us[6]];
		double t = pelement->parameters[this->refpf_to_us[7]];
		
		double p = 3.0;
		double d = (((dmax - dmin)*s1 + dmin) - d0)*pow(s0, p) + d0;
		Eigen::MatrixXd ddds = Eigen::MatrixXd::Zero(2, 1);
		ddds(0, 0) = p*((dmax - dmin)*s1 + dmin - d0)*pow(s0, p - 1.0);
		ddds(1, 0) = (dmax - dmin)*pow(s0, p);
		
		//----------要素毎の感度計算----------
		dweight.block(i*this->NOP, 0, this->NOP, 1) += ddds * 4.0*(l - d)*t*rho;
		i++;
	}

	return dweight;
}


double PANSFEM::WeightAlternative3Phase::value() {
	double weight = 0.0;
	for (auto pelement : this->pelements) {
		//----------パラメータの代入----------
		double s0 = pelement->parameters[this->refpf_to_us[0]];
		double s1 = pelement->parameters[this->refpf_to_us[1]];
		double l = pelement->parameters[this->refpf_to_us[2]];
		double rho = pelement->parameters[this->refpf_to_us[3]];
		double dmax = pelement->parameters[this->refpf_to_us[4]];
		double dmin = pelement->parameters[this->refpf_to_us[5]];
		double d0 = pelement->parameters[this->refpf_to_us[6]];
		double t = pelement->parameters[this->refpf_to_us[7]];

		double p = 3.0;
		double d = (((dmax - dmin)*s1 + dmin) - d0)*pow(s0, p) + d0;
		double w = 2.0*(2.0*l - d)*d*t*rho;

		//----------要素毎の重量計算式----------
		std::function<double(std::vector<double>) > f = [=](std::vector<double> _xi) {
			return rho * pelement->Jacobian(_xi).determinant()*t;			
		};

		//----------要素毎の重量計算----------
		weight += w - 0.1*this->pintegrations[pelement]->Integrate(f);
	}

	return weight;
}