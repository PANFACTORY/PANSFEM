//*****************************************************************************
//Title		:PANSFEM/Core/ShapeFunction/Triangle/Triangle.h
//Author	:Tanabe Yuta
//Date		:2019/06/11
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "Triangle.h"


PANSFEM::Triangle::Triangle() : ShapeFunction() {}


PANSFEM::Triangle::~Triangle(){}


PANSFEM::Triangle::Triangle(Element *_pelement) : ShapeFunction(_pelement){}


Eigen::VectorXd PANSFEM::Triangle::Trial(std::vector<double> _xi){
	Eigen::VectorXd n = Eigen::VectorXd(3);
	n(0) = _xi[0];	
	n(1) = _xi[1];
	n(2) = 1.0 - _xi[0] - _xi[1];
	return n;
}


Eigen::MatrixXd PANSFEM::Triangle::dTrialdx(std::vector<double> _xi){
	Eigen::MatrixXd dndxi = Eigen::MatrixXd(2, 3);
	dndxi(0, 0) = 1.0;	dndxi(0, 1) = 0.0;	dndxi(0, 2) = -1.0;
	dndxi(1, 0) = 0.0;	dndxi(1, 1) = 1.0;	dndxi(1, 2) = -1.0;
	return dndxi;
}


Eigen::VectorXd PANSFEM::Triangle::Test(std::vector<double> _x){
	return this->Trial(_x);
}


Eigen::MatrixXd PANSFEM::Triangle::dTestdx(std::vector<double> _x){
	return this->dTrialdx(_x);
}