//*****************************************************************************
//Title		:PANSFEM/Phenomenon/HeatTransfer/Static2D/HeatTransferStatic2D.cpp
//Author	:Tanabe Yuta
//Date		:2019/06/25
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "HeatTransferStatic2D.h"


PANSFEM::HeatTransferStatic2D::HeatTransferStatic2D() : Equation(){}


PANSFEM::HeatTransferStatic2D::~HeatTransferStatic2D() {}


PANSFEM::HeatTransferStatic2D::HeatTransferStatic2D(Element * _pelement, std::vector<int> _ulist, std::vector<double> _parameters) : Equation(_pelement, _ulist, _parameters, 2, 1, 1){
	this->alpha = _parameters[0];
}


void PANSFEM::HeatTransferStatic2D::SetEquation(){
	//----------[Ke]‚ðŒvŽZ----------
	

	//----------{Fe}‚ðŒvŽZ----------
}