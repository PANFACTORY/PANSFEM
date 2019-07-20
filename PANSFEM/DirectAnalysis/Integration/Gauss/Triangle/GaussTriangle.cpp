//*****************************************************************************
//Title		:PANSFEM/Core/Integration/Gauss/Triangle/GaussTriangle.h
//Author	:Tanabe Yuta
//Date		:2019/06/08
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "GaussTriangle.h"


PANSFEM::GaussTriangle::GaussTriangle(){}


PANSFEM::GaussTriangle::~GaussTriangle(){}


Eigen::MatrixXd PANSFEM::GaussTriangle::Integrate(std::function<Eigen::MatrixXd(std::vector<double>)> _f){
	return 0.5*_f({ 1.0 / 3.0, 1.0 / 3.0 });
}


Eigen::VectorXd PANSFEM::GaussTriangle::Integrate(std::function<Eigen::VectorXd(std::vector<double>)> _f) {
	return 0.5*_f({ 1.0 / 3.0, 1.0 / 3.0 });
}


double PANSFEM::GaussTriangle::Integrate(std::function<double(std::vector<double>)> _f) {
	return 0.5*_f({ 1.0 / 3.0, 1.0 / 3.0 });
}