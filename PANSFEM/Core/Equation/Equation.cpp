//*****************************************************************************
//Title		:PANSFEM/Core/Equation/Equation.cpp
//Author	:Tanabe Yuta
//Date		:2019/06/09
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include <iostream>


#include "Equation.h"


PANSFEM::Equation::Equation() : DOX(0), DOU(0), NOP(0){}


PANSFEM::Equation::~Equation(){}


PANSFEM::Equation::Equation(Element * _pelement, std::vector<int> _ulist, std::vector<double> _parameters, int _DOX, int _DOU, int _NOP) : DOX(_DOX), DOU(_DOU), NOP(_NOP){
	try {
		//----------DOU‚ÆNOP‚ÌŒŸØ----------
		if (_ulist.size() != _DOU || _parameters.size() != _NOP) {
			throw std::exception();
		}

		//----------ƒpƒ‰ƒ[ƒ^‘ã“ü----------
		this->pelement = _pelement;
		this->ueq_to_us = _ulist;
	}
	catch (std::exception e) {
		std::cout << "Error in Equation" << std::endl;
	}
}