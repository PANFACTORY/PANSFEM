//*****************************************************************************
//Title		:PANSFEM/Core/ShapeFunction/Triangle/Triangle.h
//Author	:Tanabe Yuta
//Date		:2019/06/11
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "Triangle.h"


PANSFEM::Triangle::Triangle(){}


PANSFEM::Triangle::~Triangle(){}


PANSFEM::Triangle::Triangle(Element * _pelement) : ShapeFunction(_pelement){}


Eigen::VectorXd PANSFEM::Triangle::Trial(std::vector<int> _ulist, std::vector<double> _x){
	return Eigen::VectorXd();
}


Eigen::MatrixXd PANSFEM::Triangle::dTrialdx(std::vector<int> _ulist, std::vector<double> _x){
	return Eigen::MatrixXd();
}


Eigen::VectorXd PANSFEM::Triangle::Test(std::vector<int> _ulist, std::vector<double> _x){
	return Eigen::VectorXd();
}


Eigen::MatrixXd PANSFEM::Triangle::dTestdx(std::vector<int> _ulist, std::vector<double> _x){
	return Eigen::MatrixXd();
}