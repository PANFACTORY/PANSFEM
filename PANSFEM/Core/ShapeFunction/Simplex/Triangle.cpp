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


Eigen::VectorXd PANSFEM::Triangle::Trial(std::vector<double> _x){
	double x0 = this->pparent->pnodes[0]->x(0), y0 = this->pparent->pnodes[0]->x(1);
	double x1 = this->pparent->pnodes[1]->x(0), y1 = this->pparent->pnodes[1]->x(1);
	double x2 = this->pparent->pnodes[2]->x(0), y2 = this->pparent->pnodes[2]->x(1);
	
	Eigen::VectorXd trial = Eigen::VectorXd(3);
	trial(0) = (x1 * y2 - x2 * y1) + (y1 - y2)*_x[0] + (x2 - x1)*_x[1];
	trial(1) = (x2 * y0 - x0 * y2) + (y2 - y0)*_x[0] + (x0 - x2)*_x[1];
	trial(2) = (x0 * y1 - x1 * y0) + (y0 - y1)*_x[0] + (x1 - x0)*_x[1];
	return trial;
}


Eigen::MatrixXd PANSFEM::Triangle::dTrialdx(std::vector<double> _x){
	double x0 = this->pparent->pnodes[0]->x(0), y0 = this->pparent->pnodes[0]->x(1);
	double x1 = this->pparent->pnodes[1]->x(0), y1 = this->pparent->pnodes[1]->x(1);
	double x2 = this->pparent->pnodes[2]->x(0), y2 = this->pparent->pnodes[2]->x(1);

	Eigen::MatrixXd dtrialdx = Eigen::MatrixXd(2, 3);
	dtrialdx(0, 0) = y1 - y2;	dtrialdx(0, 1) = y2 - y0;	dtrialdx(0, 2) = y0 - y1;
	dtrialdx(1, 0) = x2 - x1;	dtrialdx(1, 1) = x0 - x2;	dtrialdx(1, 2) = x1 - x0;
	return dtrialdx;
}


Eigen::VectorXd PANSFEM::Triangle::Test(std::vector<double> _x){
	return this->Trial(_x);
}


Eigen::MatrixXd PANSFEM::Triangle::dTestdx(std::vector<double> _x){
	return this->dTrialdx(_x);
}