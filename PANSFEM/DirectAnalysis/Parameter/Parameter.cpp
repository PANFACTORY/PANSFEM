//*****************************************************************************
//Title		:PANSFEM/DirectAnalysis/Parameter/Parameter.cpp
//Author	:Tanabe Yuta
//Date		:2019/09/25
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include <iostream>


#include "Parameter.h"


PANSFEM::Parameter::Parameter() {}


PANSFEM::Parameter::~Parameter() {}


PANSFEM::Parameter::Parameter(std::vector<double> _parameters, std::vector<int> _plist) {
	try {
		if (_parameters.size() != _plist.size()) {
			throw std::exception();
		}

		//----------ƒpƒ‰ƒ[ƒ^‚Ì‘Î‰ŠÖŒW‚ğ®—----------
		for (int i = 0; i < _plist.size(); i++) {
			this->parameters[_plist[i]] = _parameters[i];
		}
	}
	catch (std::exception e) {
		std::cout << "Error in setting parameters" << std::endl;
	}
}