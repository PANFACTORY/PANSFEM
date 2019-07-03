//*****************************************************************************
//Title		:PANSFEM/InversedAnalysis/Function/Function.cpp
//Author	:Tanabe Yuta
//Date		:2019/06/30
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "Function.h"


PANSFEM::Function::Function() : NOP(0), DOREFU(0), NOREFP(0){}


PANSFEM::Function::~Function(){}


PANSFEM::Function::Function(std::vector<int> _plist, std::vector<int> _refulist, std::vector<int> _refplist, int _NOP, int _DOREFU, int _NOREFP) : NOP(_NOP), DOREFU(_DOREFU), NOREFP(_NOREFP) {
	try {
		this->pf_to_ps = _plist;
		this->refuf_to_us = _refulist;
		this->refpf_to_us = _refplist;

		if (this->NOP != this->pf_to_ps.size() || this->DOREFU != this->refuf_to_us.size() || this->NOREFP != this->refpf_to_us.size()) {
			throw std::exception();
		}
	}
	catch (std::exception e) {
		std::cout << "Error in Function" << std::endl;
	}
}