//*****************************************************************************
//Title		:PANSFEM/Core/Equation/Equation.cpp
//Author	:Tanabe Yuta
//Date		:2019/06/09
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include <iostream>


#include "Equation.h"


PANSFEM::Equation::Equation() : DOX(0), DOU(0), DOREFU(0), NOP(0){}


PANSFEM::Equation::~Equation(){
	delete this->pintegration;
}


PANSFEM::Equation::Equation(Element * _pelement, std::vector<int> _ulist, std::vector<int> _refulist, std::vector<int> _plist, Integration* _pintegration, int _DOX, int _DOU, int _DOREFU, int _NOP) : DOX(_DOX), DOU(_DOU), DOREFU(_DOREFU), NOP(_NOP){
	try {
		//----------DOU‚ÆNOP‚ÌŒŸØ----------
		if (_ulist.size() != _DOU || _refulist.size() != _DOREFU || _plist.size() != _NOP) {
			throw std::exception();
		}

		//----------ƒpƒ‰ƒ[ƒ^‘ã“ü----------
		this->pelement = _pelement;
		this->ueq_to_us = _ulist;
		this->refueq_to_us = _refulist;
		this->peq_to_ps = _plist;
		this->pintegration = _pintegration;
	}
	catch (std::exception e) {
		std::cout << "Error in Equation" << std::endl;
	}
}