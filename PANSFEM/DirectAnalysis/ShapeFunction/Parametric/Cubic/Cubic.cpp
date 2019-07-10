//*****************************************************************************
//Title		:PANSFEM/Core/ShapeFunction/Parametric/Cubic/Cubic.cpp
//Author	:Tanabe Yuta
//Date		:2019/07/10
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "Cubic.h"


PANSFEM::Cubic::Cubic() {}


PANSFEM::Cubic::~Cubic(){}


Eigen::VectorXd PANSFEM::Cubic::Trial(std::vector<Node*> _pnodes, std::vector<double> _xi){
	Eigen::VectorXd n = Eigen::VectorXd(8);
	n(0) = 0.125*(1.0 - _xi[0])*(1.0 - _xi[1])*(1.0 - _xi[2]);
	n(1) = 0.125*(1.0 + _xi[0])*(1.0 - _xi[1])*(1.0 - _xi[2]);
	n(2) = 0.125*(1.0 + _xi[0])*(1.0 + _xi[1])*(1.0 - _xi[2]);
	n(3) = 0.125*(1.0 - _xi[0])*(1.0 + _xi[1])*(1.0 - _xi[2]);
	n(4) = 0.125*(1.0 - _xi[0])*(1.0 - _xi[1])*(1.0 + _xi[2]);
	n(5) = 0.125*(1.0 + _xi[0])*(1.0 - _xi[1])*(1.0 + _xi[2]);
	n(6) = 0.125*(1.0 + _xi[0])*(1.0 + _xi[1])*(1.0 + _xi[2]);
	n(7) = 0.125*(1.0 - _xi[0])*(1.0 + _xi[1])*(1.0 + _xi[2]);
	return n;
}


Eigen::MatrixXd PANSFEM::Cubic::dTrialdx(std::vector<Node*> _pnodes, std::vector<double> _xi){
	Eigen::MatrixXd dndxi = Eigen::MatrixXd(3, 8);
	dndxi(0, 0) = -0.125*(1.0 - _xi[1])*(1.0 - _xi[2]);	dndxi(0, 1) = 0.125*(1.0 - _xi[1])*(1.0 - _xi[2]);	dndxi(0, 2) = 0.125*(1.0 + _xi[1])*(1.0 - _xi[2]);	dndxi(0, 3) = -0.125*(1.0 + _xi[1])*(1.0 - _xi[2]);
	dndxi(0, 4) = -0.125*(1.0 - _xi[1])*(1.0 + _xi[2]);	dndxi(0, 5) = 0.125*(1.0 - _xi[1])*(1.0 + _xi[2]);	dndxi(0, 6) = 0.125*(1.0 + _xi[1])*(1.0 + _xi[2]);	dndxi(0, 7) = -0.125*(1.0 + _xi[1])*(1.0 + _xi[2]);

	dndxi(1, 0) = -0.125*(1.0 - _xi[2])*(1.0 - _xi[0]);	dndxi(1, 1) = -0.125*(1.0 - _xi[2])*(1.0 + _xi[0]);	dndxi(1, 2) = 0.125*(1.0 - _xi[2])*(1.0 + _xi[0]);	dndxi(1, 3) = 0.125*(1.0 - _xi[2])*(1.0 - _xi[0]);
	dndxi(1, 4) = -0.125*(1.0 + _xi[2])*(1.0 - _xi[0]);	dndxi(1, 5) = -0.125*(1.0 + _xi[2])*(1.0 + _xi[0]);	dndxi(1, 6) = 0.125*(1.0 + _xi[2])*(1.0 + _xi[0]);	dndxi(1, 7) = 0.125*(1.0 + _xi[2])*(1.0 - _xi[0]);
	
	dndxi(2, 0) = -0.125*(1.0 - _xi[0])*(1.0 - _xi[1]);	dndxi(2, 1) = -0.125*(1.0 + _xi[0])*(1.0 - _xi[1]);	dndxi(2, 2) = -0.125*(1.0 + _xi[0])*(1.0 + _xi[1]);	dndxi(2, 3) = -0.125*(1.0 - _xi[0])*(1.0 + _xi[1]);
	dndxi(2, 4) = 0.125*(1.0 - _xi[0])*(1.0 - _xi[1]);	dndxi(2, 5) = 0.125*(1.0 + _xi[0])*(1.0 - _xi[1]);	dndxi(2, 6) = 0.125*(1.0 + _xi[0])*(1.0 + _xi[1]);	dndxi(2, 7) = 0.125*(1.0 - _xi[0])*(1.0 + _xi[1]);
	return dndxi;
}


Eigen::VectorXd PANSFEM::Cubic::Test(std::vector<Node*> _pnodes, std::vector<double> _xi){
	return this->Trial(_pnodes, _xi);
}


Eigen::MatrixXd PANSFEM::Cubic::dTestdx(std::vector<Node*> _pnodes, std::vector<double> _xi){
	return this->dTrialdx(_pnodes, _xi);
}