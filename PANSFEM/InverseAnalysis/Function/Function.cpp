//*****************************************************************************
//Title		:PANSFEM/InversedAnalysis/Function/Function.cpp
//Author	:Tanabe Yuta
//Date		:2019/06/30
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "Function.h"


PANSFEM::Function::Function(){}


PANSFEM::Function::~Function(){}


PANSFEM::Function::Function(std::vector<int> _plist){
	this->pf_to_ps = _plist;
}