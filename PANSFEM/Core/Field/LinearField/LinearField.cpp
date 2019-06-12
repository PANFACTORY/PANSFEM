//*****************************************************************************
//Title		:PANSFEM/Core/Field/LinearField/LinearField.cpp
//Author	:Tanabe Yuta
//Date		:2019/06/12
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "LinearField.h"


PANSFEM::LinearField::LinearField() : Field() {}


PANSFEM::LinearField::~LinearField(){}


PANSFEM::LinearField::LinearField(std::vector<int> _ulist, std::vector<Equation*> _pequations) : Field(_ulist, _pequations){}