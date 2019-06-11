//*****************************************************************************
//Title		:PANSFEM/Core/ShapeFunction/ShapeFunction.h
//Author	:Tanabe Yuta
//Date		:2019/06/08
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "ShapeFunction.h"


PANSFEM::ShapeFunction::ShapeFunction(){}


PANSFEM::ShapeFunction::~ShapeFunction() {}


PANSFEM::ShapeFunction::ShapeFunction(Element * _pelement) {
	this->pparent = _pelement;
}