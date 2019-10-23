//*************************************************************************************************
//Title		:PANSFEM/InversedAnalysis/Function/WeightAlternative3D/WeightAlternative3D.cpp
//Author	:Tanabe Yuta
//Date		:2019/09/27
//Copyright	:(C)2019 TanabeYuta
//*************************************************************************************************


#include "WeightAlternative3D.h"


PANSFEM::WeightAlternative3D::WeightAlternative3D() : Function() {}


PANSFEM::WeightAlternative3D::~WeightAlternative3D() {}


PANSFEM::WeightAlternative3D::WeightAlternative3D(std::vector<int> _plist, std::vector<int> _refulist, std::vector<int> _refplist) : Function(_plist, _refulist, _refplist, 1, 0, 5) {}


Eigen::VectorXd PANSFEM::WeightAlternative3D::sensitivitis() {
	Eigen::VectorXd dweight = Eigen::VectorXd::Zero(this->NOP*this->parametersindex.size());

	for (auto pelement : this->pelements) {
		//----------パラメータの取得----------
		double s = pelement->pparameter->parameters[this->refpf_to_us[0]];
		double dmax = pelement->pparameter->parameters[this->refpf_to_us[1]];
		double dmin = pelement->pparameter->parameters[this->refpf_to_us[2]];
		double L = pelement->pparameter->parameters[this->refpf_to_us[3]];
		double rho = pelement->pparameter->parameters[this->refpf_to_us[4]];

		double d = (dmax - dmin)*s + dmin;
		double ddds = dmax - dmin;
		
		//----------要素毎の積分計算----------
		dweight[this->parametersindex[pelement->pparameter]] += 2.0*sqrt(3.0)*3.141592*d*L*ddds*rho;
	}

	return dweight;
}


double PANSFEM::WeightAlternative3D::value() {
	double weight = 0.0;
	for (auto pelement : this->pelements) {
		//----------パラメータの代入----------
		double s = pelement->pparameter->parameters[this->refpf_to_us[0]];
		double dmax = pelement->pparameter->parameters[this->refpf_to_us[1]];
		double dmin = pelement->pparameter->parameters[this->refpf_to_us[2]];
		double L = pelement->pparameter->parameters[this->refpf_to_us[3]];
		double rho = pelement->pparameter->parameters[this->refpf_to_us[4]];

		double d = (dmax - dmin)*s + dmin;

		//----------要素毎の重量計算式----------
		std::function<double(std::vector<double>) > f = [=](std::vector<double> _xi) {
			return 0.02*rho*pelement->Jacobian(_xi).determinant();			//ここでは例として重量制約3%とする。
		};

		//----------要素毎の積分計算----------
		weight += sqrt(3.0)*3.141592*pow(d, 2.0)*L*rho - this->pintegrations[pelement]->Integrate(f);
	}

	return weight;
}