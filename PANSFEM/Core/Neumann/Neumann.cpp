//*****************************************************************************
//Title		:PANSFEM/Core/Neumann/Neumann.h
//Author	:Tanabe Yuta
//Date		:2019/06/22
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "Neumann.h"


PANSFEM::Neumann::Neumann(){}


PANSFEM::Neumann::~Neumann(){}


PANSFEM::Neumann::Neumann(Node * _pnode, std::vector<double> _q){
	this->pnode = _pnode;
	this->q = _q;
}