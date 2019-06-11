//*****************************************************************************
//Title		:PANSFEM/Core/ShapeFunction/ShapeFunction.h
//Author	:Tanabe Yuta
//Date		:2019/06/08
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "ShapeFunction.h"


PANSFEM::ShapeFunction::ShapeFunction(){}


PANSFEM::ShapeFunction::~ShapeFunction() {}


Eigen::VectorXd PANSFEM::ShapeFunction::N(std::vector<double> _x){
	return Eigen::VectorXd::Zero(0);
}


Eigen::MatrixXd PANSFEM::ShapeFunction::dNdx(std::vector<double> _x){
	return Eigen::MatrixXd::Zero(0, 0);
}
