//*****************************************************************************
//Title		:PANSFEM/Core/Field/Field.h
//Author	:Tanabe Yuta
//Date		:2019/06/08
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "Field.h"

PANSFEM::Field::Field(){}


PANSFEM::Field::~Field(){}


PANSFEM::Field::Field(std::vector<int> _ulist, std::vector<Equation*> _pequations){
	this->pequations = _pequations;
}
