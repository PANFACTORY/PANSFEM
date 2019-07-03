//*****************************************************************************
//Title		:PANSFEM/Core/Integration/Gauss/Square/GaussSquare.cpp
//Author	:Tanabe Yuta
//Date		:2019/06/08
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "GaussSquare.h"


std::vector<std::vector<double> > PANSFEM::GaussSquare::GP = { { -1.0 / sqrt(3.0), -1.0 / sqrt(3.0) },
															   {  1.0 / sqrt(3.0), -1.0 / sqrt(3.0) },
															   {  1.0 / sqrt(3.0),  1.0 / sqrt(3.0) },
						                                       { -1.0 / sqrt(3.0),  1.0 / sqrt(3.0) } };
std::vector<std::vector<double> > PANSFEM::GaussSquare::GW = { { 1.0, 1.0 },
															   { 1.0, 1.0 },
						                                       { 1.0, 1.0 },
						                                       { 1.0, 1.0 } };


PANSFEM::GaussSquare::GaussSquare(){}


PANSFEM::GaussSquare::~GaussSquare(){}


Eigen::MatrixXd PANSFEM::GaussSquare::Integrate(std::function<Eigen::MatrixXd(std::vector<double>)> _f){
	Eigen::MatrixXd m = GaussSquare::GW[0][0] * GaussSquare::GW[0][1] * _f(GaussSquare::GP[0]);
	for (int i = 1; i < 4; i++) {
		m += GaussSquare::GW[i][0] * GaussSquare::GW[i][1] * _f(this->GP[i]);
	}
	return m;
}


Eigen::VectorXd PANSFEM::GaussSquare::Integrate(std::function<Eigen::VectorXd(std::vector<double>)> _f){
	Eigen::VectorXd v = GaussSquare::GW[0][0] * GaussSquare::GW[0][1] * _f(GaussSquare::GP[0]);
	for (int i = 1; i < 4; i++) {
		v += GaussSquare::GW[i][0] * GaussSquare::GW[i][1] * _f(GaussSquare::GP[i]);
	}
	return v;
}


double PANSFEM::GaussSquare::Integrate(std::function<double(std::vector<double>)> _f) {
	double d = GaussSquare::GW[0][0] * GaussSquare::GW[0][1] * _f(GaussSquare::GP[0]);
	for (int i = 1; i < 4; i++) {
		d += GaussSquare::GW[i][0] * GaussSquare::GW[i][1] * _f(GaussSquare::GP[i]);
	}
	return d;
}