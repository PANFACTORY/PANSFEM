//*****************************************************************************
//Title		:PANSFEM/Core/ShapeFunction/Parametric/Quadrangle/Quadrangle.h
//Author	:Tanabe Yuta
//Date		:2019/06/11
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "Quadrangle.h"


PANSFEM::Quadrangle::Quadrangle() {}


PANSFEM::Quadrangle::~Quadrangle(){}


Eigen::VectorXd PANSFEM::Quadrangle::Trial(std::vector<Node*> _pnodes, std::vector<double> _xi){
	Eigen::VectorXd n = Eigen::VectorXd(4);
	n(0) = 0.25*(1.0 - _xi[0])*(1.0 - _xi[1]);
	n(1) = 0.25*(1.0 + _xi[0])*(1.0 - _xi[1]);
	n(2) = 0.25*(1.0 + _xi[0])*(1.0 + _xi[1]);
	n(3) = 0.25*(1.0 - _xi[0])*(1.0 + _xi[1]);
	return n;
}


Eigen::MatrixXd PANSFEM::Quadrangle::dTrialdx(std::vector<Node*> _pnodes, std::vector<double> _xi){
	Eigen::MatrixXd dndxi = Eigen::MatrixXd(2, 4);
	dndxi(0, 0) = -0.25*(1.0 - _xi[1]);	dndxi(0, 1) = 0.25*(1.0 - _xi[1]);	dndxi(0, 2) = 0.25*(1.0 + _xi[1]);	dndxi(0, 3) = -0.25*(1.0 + _xi[1]);
	dndxi(1, 0) = -0.25*(1.0 - _xi[0]);	dndxi(1, 1) = -0.25*(1.0 + _xi[0]);	dndxi(1, 2) = 0.25*(1.0 + _xi[0]);	dndxi(1, 3) = 0.25*(1.0 - _xi[0]);
	return dndxi;
}


Eigen::VectorXd PANSFEM::Quadrangle::Test(std::vector<Node*> _pnodes, std::vector<double> _xi){
	return this->Trial(_pnodes, _xi);
}


Eigen::MatrixXd PANSFEM::Quadrangle::dTestdx(std::vector<Node*> _pnodes, std::vector<double> _xi){
	return this->dTrialdx(_pnodes, _xi);
}