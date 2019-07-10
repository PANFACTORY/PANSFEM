//*****************************************************************************
//Title		:PANSFEM/Core/Integration/Gauss/Cubic/GaussCubic.cpp
//Author	:Tanabe Yuta
//Date		:2019/07/10
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "GaussCubic.h"


std::vector<std::vector<double> > PANSFEM::GaussCubic::GP = { { -1.0 / sqrt(3.0), -1.0 / sqrt(3.0), -1.0 / sqrt(3.0) },
															  {  1.0 / sqrt(3.0), -1.0 / sqrt(3.0), -1.0 / sqrt(3.0) },
															  {  1.0 / sqrt(3.0),  1.0 / sqrt(3.0), -1.0 / sqrt(3.0) },
															  { -1.0 / sqrt(3.0),  1.0 / sqrt(3.0), -1.0 / sqrt(3.0) },
															  { -1.0 / sqrt(3.0), -1.0 / sqrt(3.0),  1.0 / sqrt(3.0) },
															  {  1.0 / sqrt(3.0), -1.0 / sqrt(3.0),  1.0 / sqrt(3.0) },
															  {  1.0 / sqrt(3.0),  1.0 / sqrt(3.0),  1.0 / sqrt(3.0) },
															  { -1.0 / sqrt(3.0),  1.0 / sqrt(3.0),  1.0 / sqrt(3.0) } };
std::vector<std::vector<double> > PANSFEM::GaussCubic::GW = { { 1.0, 1.0, 1.0 },
															  { 1.0, 1.0, 1.0 },
															  { 1.0, 1.0, 1.0 },
															  { 1.0, 1.0, 1.0 },
															  { 1.0, 1.0, 1.0 },
															  { 1.0, 1.0, 1.0 },
															  { 1.0, 1.0, 1.0 },
															  { 1.0, 1.0, 1.0 } };


PANSFEM::GaussCubic::GaussCubic() {}


PANSFEM::GaussCubic::~GaussCubic() {}


Eigen::MatrixXd PANSFEM::GaussCubic::Integrate(std::function<Eigen::MatrixXd(std::vector<double>)> _f) {
	Eigen::MatrixXd m = GaussCubic::GW[0][0] * GaussCubic::GW[0][1] * GaussCubic::GW[0][2] * _f(GaussCubic::GP[0]);
	for (int i = 1; i < 8; i++) {
		m += GaussCubic::GW[i][0] * GaussCubic::GW[i][1] * GaussCubic::GW[0][2] * _f(this->GP[i]);
	}
	return m;
}


Eigen::VectorXd PANSFEM::GaussCubic::Integrate(std::function<Eigen::VectorXd(std::vector<double>)> _f) {
	Eigen::VectorXd v = GaussCubic::GW[0][0] * GaussCubic::GW[0][1] * GaussCubic::GW[0][2] * _f(GaussCubic::GP[0]);
	for (int i = 1; i < 8; i++) {
		v += GaussCubic::GW[i][0] * GaussCubic::GW[i][1] * GaussCubic::GW[0][2] * _f(GaussCubic::GP[i]);
	}
	return v;
}


double PANSFEM::GaussCubic::Integrate(std::function<double(std::vector<double>)> _f) {
	double d = GaussCubic::GW[0][0] * GaussCubic::GW[0][1] * GaussCubic::GW[0][2] * _f(GaussCubic::GP[0]);
	for (int i = 1; i < 8; i++) {
		d += GaussCubic::GW[i][0] * GaussCubic::GW[i][1] * GaussCubic::GW[0][2] * _f(GaussCubic::GP[i]);
	}
	return d;
}