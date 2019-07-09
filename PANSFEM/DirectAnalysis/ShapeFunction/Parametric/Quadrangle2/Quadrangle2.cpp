//*****************************************************************************
//Title		:PANSFEM/Core/ShapeFunction/Parametric/Quadrangle2/Quadrangle2.h
//Author	:Tanabe Yuta
//Date		:2019/07/09
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "Quadrangle2.h"


PANSFEM::Quadrangle2::Quadrangle2() {}


PANSFEM::Quadrangle2::~Quadrangle2(){}


Eigen::VectorXd PANSFEM::Quadrangle2::Trial(std::vector<Node*> _pnodes, std::vector<double> _xi){
	Eigen::VectorXd n = Eigen::VectorXd(8);
	n(0) = 0.25*(1.0 - _xi[0])*(1.0 - _xi[1])*(-_xi[0] - _xi[1] - 1.0);
	n(1) = 0.25*(1.0 + _xi[0])*(1.0 - _xi[1])*(_xi[0] - _xi[1] - 1.0);
	n(2) = 0.25*(1.0 + _xi[0])*(1.0 + _xi[1])*(_xi[0] + _xi[1] - 1.0);
	n(3) = 0.25*(1.0 - _xi[0])*(1.0 + _xi[1])*(-_xi[0] + _xi[1] - 1.0);
	n(4) = 0.5*(1.0 - _xi[0])*(1.0 + _xi[0])*(1.0 - _xi[1]);
	n(5) = 0.5*(1.0 + _xi[0])*(1.0 + _xi[1])*(1.0 - _xi[1]);
	n(6) = 0.5*(1.0 + _xi[0])*(1.0 - _xi[0])*(1.0 + _xi[1]);
	n(7) = 0.5*(1.0 - _xi[0])*(1.0 + _xi[1])*(1.0 - _xi[1]);
	return n;
}


Eigen::MatrixXd PANSFEM::Quadrangle2::dTrialdx(std::vector<Node*> _pnodes, std::vector<double> _xi){
	Eigen::MatrixXd dndxi = Eigen::MatrixXd(2, 8);
	dndxi(0, 0) = 0.25*(-(1.0 - _xi[1])*(-_xi[0] - _xi[1] - 1.0) - (1.0 - _xi[0])*(1.0 - _xi[1]));
	dndxi(0, 1) = 0.25*((1.0 - _xi[1])*(_xi[0] - _xi[1] - 1.0) + (1.0 + _xi[0])*(1.0 - _xi[1]));
	dndxi(0, 2) = 0.25*((1.0 + _xi[1])*(_xi[0] + _xi[1] - 1.0) + (1.0 + _xi[0])*(1.0 + _xi[1]));
	dndxi(0, 3) = 0.25*(-(1.0 + _xi[1])*(-_xi[0] + _xi[1] - 1.0) - (1.0 - _xi[0])*(1.0 + _xi[1]));
	dndxi(0, 4) = -_xi[0] * (1.0 - _xi[1]);
	dndxi(0, 5) = 0.5*(1.0 + _xi[1])*(1.0 - _xi[1]);
	dndxi(0, 6) = -_xi[0] * (1.0 + _xi[1]);
	dndxi(0, 7) = -0.5*(1.0 + _xi[1])*(1.0 - _xi[1]);

	dndxi(1, 0) = 0.25*(-(1.0 - _xi[0])*(-_xi[0] - _xi[1] - 1.0) - (1.0 - _xi[0])*(1.0 - _xi[1]));
	dndxi(1, 1) = 0.25*(-(1.0 + _xi[0])*(_xi[0] - _xi[1] - 1.0) - (1.0 + _xi[0])*(1.0 - _xi[1]));
	dndxi(1, 2) = 0.25*((1.0 + _xi[0])*(_xi[0] + _xi[1] - 1.0) + (1.0 + _xi[0])*(1.0 + _xi[1]));
	dndxi(1, 3) = 0.25*((1.0 - _xi[0])*(-_xi[0] + _xi[1] - 1.0) + (1.0 - _xi[0])*(1.0 + _xi[1]));
	dndxi(1, 4) = -0.5*(1.0 + _xi[0])*(1.0 - _xi[0]);
	dndxi(1, 5) = -_xi[1] * (1.0 + _xi[0]);
	dndxi(1, 6) = 0.5*(1.0 + _xi[0])*(1.0 - _xi[0]);
	dndxi(1, 7) = -_xi[1] * (1.0 - _xi[0]);
	return dndxi;
}


Eigen::VectorXd PANSFEM::Quadrangle2::Test(std::vector<Node*> _pnodes, std::vector<double> _xi){
	return this->Trial(_pnodes, _xi);
}


Eigen::MatrixXd PANSFEM::Quadrangle2::dTestdx(std::vector<Node*> _pnodes, std::vector<double> _xi){
	return this->dTrialdx(_pnodes, _xi);
}