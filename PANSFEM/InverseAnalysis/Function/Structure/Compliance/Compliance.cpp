//*****************************************************************************
//Title		:PANSFEM/InversedAnalysis/Function/Compliance/Compliance.cpp
//Author	:Tanabe Yuta
//Date		:2019/07/02
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "Compliance.h"


PANSFEM::Compliance::Compliance(){}


PANSFEM::Compliance::~Compliance(){}


PANSFEM::Compliance::Compliance(std::vector<int> _plist) : Function(_plist){}


Eigen::VectorXd PANSFEM::Compliance::sensitivitis(){
	return Eigen::VectorXd();
}


double PANSFEM::Compliance::value(){
	double compliance = 0.0;

	for (auto pelement : this->pelements) {
		
		
	}

	return compliance;
}