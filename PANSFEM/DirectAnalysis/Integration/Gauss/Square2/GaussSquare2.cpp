//*****************************************************************************
//Title		:PANSFEM/Core/Integration/Gauss/Square2/GaussSquare2.cpp
//Author	:Tanabe Yuta
//Date		:2019/07/09
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "GaussSquare2.h"


std::vector<std::vector<double> > PANSFEM::GaussSquare2::GP = { { -sqrt(3.0/5.0), -sqrt(3.0/5.0) },
															    { 0.0, -sqrt(3.0/5.0) },
															    { sqrt(3.0/5.0), -sqrt(3.0/5.0) },
						                                        { -sqrt(3.0/5.0), 0.0 },
															    { 0.0, 0.0 },
															    { sqrt(3.0/5.0), 0.0 },
															    { -sqrt(3.0/5.0), sqrt(3.0/5.0) },
															    { 0.0, sqrt(3.0/5.0) },
															    { sqrt(3.0/5.0), sqrt(3.0/5.0) } };
std::vector<std::vector<double> > PANSFEM::GaussSquare2::GW = { { 5.0/9.0, 5.0/9.0 },
																{ 8.0/9.0, 5.0/9.0 },
															    { 5.0/9.0, 5.0/9.0 },
						                                        { 5.0/9.0, 8.0/9.0 },
						                                        { 8.0/9.0, 8.0/9.0 },
																{ 5.0/9.0, 8.0/9.0 },
																{ 5.0/9.0, 5.0/9.0 },
																{ 8.0/9.0, 5.0/9.0 },
																{ 5.0/9.0, 5.0/9.0 } };


PANSFEM::GaussSquare2::GaussSquare2(){}


PANSFEM::GaussSquare2::~GaussSquare2(){}


Eigen::MatrixXd PANSFEM::GaussSquare2::Integrate(std::function<Eigen::MatrixXd(std::vector<double>)> _f){
	Eigen::MatrixXd m = GaussSquare2::GW[0][0] * GaussSquare2::GW[0][1] * _f(GaussSquare2::GP[0]);
	for (int i = 1; i < 9; i++) {
		m += GaussSquare2::GW[i][0] * GaussSquare2::GW[i][1] * _f(this->GP[i]);
	}
	return m;
}


Eigen::VectorXd PANSFEM::GaussSquare2::Integrate(std::function<Eigen::VectorXd(std::vector<double>)> _f){
	Eigen::VectorXd v = GaussSquare2::GW[0][0] * GaussSquare2::GW[0][1] * _f(GaussSquare2::GP[0]);
	for (int i = 1; i < 9; i++) {
		v += GaussSquare2::GW[i][0] * GaussSquare2::GW[i][1] * _f(GaussSquare2::GP[i]);
	}
	return v;
}


double PANSFEM::GaussSquare2::Integrate(std::function<double(std::vector<double>)> _f) {
	double d = GaussSquare2::GW[0][0] * GaussSquare2::GW[0][1] * _f(GaussSquare2::GP[0]);
	for (int i = 1; i < 9; i++) {
		d += GaussSquare2::GW[i][0] * GaussSquare2::GW[i][1] * _f(GaussSquare2::GP[i]);
	}
	return d;
}