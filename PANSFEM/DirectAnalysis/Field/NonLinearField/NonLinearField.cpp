//*****************************************************************************
//Title		:PANSFEM/DirectAnalysis/Field/NonLinearField/NonLinearField.cpp
//Author	:Tanabe Yuta
//Date		:2019/06/12
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "NonLinearField.h"


PANSFEM::NonLinearField::NonLinearField() : Field() {}


PANSFEM::NonLinearField::~NonLinearField() {}


PANSFEM::NonLinearField::NonLinearField(std::vector<int> _ulist) : Field(_ulist) {}


void PANSFEM::NonLinearField::SolveEquation() {}