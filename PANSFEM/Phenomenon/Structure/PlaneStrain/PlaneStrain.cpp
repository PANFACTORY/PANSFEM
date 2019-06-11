//*****************************************************************************
//Title		:PANSFEM/Phenomenon/Structure/PlaneStrain/PlaneStrain.cpp
//Author	:Tanabe Yuta
//Date		:2019/06/10
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "PlaneStrain.h"


PANSFEM::PlaneStrain::PlaneStrain() : Equation(){}


PANSFEM::PlaneStrain::~PlaneStrain(){}


PANSFEM::PlaneStrain::PlaneStrain(Element * _pelement, std::vector<int> _ulist, std::vector<double> _parameters) : Equation(_pelement, _ulist, _parameters, 2, 3){
	this->E = _parameters[0];
	this->V = _parameters[1];
	this->t = _parameters[2];
}

void PANSFEM::PlaneStrain::SetEquation(){}