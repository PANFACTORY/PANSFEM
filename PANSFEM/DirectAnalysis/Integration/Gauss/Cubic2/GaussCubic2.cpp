//*****************************************************************************
//Title		:PANSFEM/Core/Integration/Gauss/Cubic2/GaussCubic2.cpp
//Author	:Tanabe Yuta
//Date		:2019/07/11
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "GaussCubic2.h"


std::vector<std::vector<double> > PANSFEM::GaussCubic2::GP = { { -sqrt(3.0 / 5.0), -sqrt(3.0 / 5.0), -sqrt(3.0 / 5.0) },
															   {  0.0,             -sqrt(3.0 / 5.0), -sqrt(3.0 / 5.0) },
															   { sqrt(3.0 / 5.0),  -sqrt(3.0 / 5.0), -sqrt(3.0 / 5.0) },
															   { -sqrt(3.0 / 5.0), 0.0,              -sqrt(3.0 / 5.0) },
															   { 0.0,              0.0,              -sqrt(3.0 / 5.0) },
															   { sqrt(3.0 / 5.0),  0.0,              -sqrt(3.0 / 5.0) },
															   { -sqrt(3.0 / 5.0), sqrt(3.0 / 5.0),  -sqrt(3.0 / 5.0) },
															   { 0.0,              sqrt(3.0 / 5.0),  -sqrt(3.0 / 5.0) },
															   { sqrt(3.0 / 5.0),  sqrt(3.0 / 5.0),  -sqrt(3.0 / 5.0) },
															   { -sqrt(3.0 / 5.0), -sqrt(3.0 / 5.0), 0.0              },
															   { 0.0,              -sqrt(3.0 / 5.0), 0.0              },
															   { sqrt(3.0 / 5.0),  -sqrt(3.0 / 5.0), 0.0              },
															   { -sqrt(3.0 / 5.0), 0.0,              0.0              },
															   { 0.0,              0.0,              0.0              },
															   { sqrt(3.0 / 5.0),  0.0,              0.0              },
															   { -sqrt(3.0 / 5.0), sqrt(3.0 / 5.0),  0.0              },
                                                               { 0.0,              sqrt(3.0 / 5.0),  0.0              },
															   { sqrt(3.0 / 5.0),  sqrt(3.0 / 5.0),  0.0              },
															   { -sqrt(3.0 / 5.0), -sqrt(3.0 / 5.0), sqrt(3.0 / 5.0)  },
															   { 0.0,              -sqrt(3.0 / 5.0), sqrt(3.0 / 5.0)  },
															   { sqrt(3.0 / 5.0),  -sqrt(3.0 / 5.0), sqrt(3.0 / 5.0)  },
															   { -sqrt(3.0 / 5.0), 0.0,              sqrt(3.0 / 5.0)  },
															   { 0.0,              0.0,              sqrt(3.0 / 5.0)  },
															   { sqrt(3.0 / 5.0),  0.0,              sqrt(3.0 / 5.0)  },
															   { -sqrt(3.0 / 5.0), sqrt(3.0 / 5.0),  sqrt(3.0 / 5.0)  },
															   { 0.0,              sqrt(3.0 / 5.0),  sqrt(3.0 / 5.0)  },
															   { sqrt(3.0 / 5.0),  sqrt(3.0 / 5.0),  sqrt(3.0 / 5.0)  } };
std::vector<std::vector<double> > PANSFEM::GaussCubic2::GW = { { 5.0 / 9.0, 5.0 / 9.0, 5.0 / 9.0 },
															   { 8.0 / 9.0, 5.0 / 9.0, 5.0 / 9.0 },
															   { 5.0 / 9.0, 5.0 / 9.0, 5.0 / 9.0 },
															   { 5.0 / 9.0, 8.0 / 9.0, 5.0 / 9.0 },
															   { 8.0 / 9.0, 8.0 / 9.0, 5.0 / 9.0 },
															   { 5.0 / 9.0, 8.0 / 9.0, 5.0 / 9.0 },
															   { 5.0 / 9.0, 5.0 / 9.0, 5.0 / 9.0 },
															   { 8.0 / 9.0, 5.0 / 9.0, 5.0 / 9.0 },
                                                               { 5.0 / 9.0, 5.0 / 9.0, 5.0 / 9.0 },
															   { 5.0 / 9.0, 5.0 / 9.0, 8.0 / 9.0 },
															   { 8.0 / 9.0, 5.0 / 9.0, 8.0 / 9.0 },
															   { 5.0 / 9.0, 5.0 / 9.0, 8.0 / 9.0 },
															   { 5.0 / 9.0, 8.0 / 9.0, 8.0 / 9.0 },
															   { 8.0 / 9.0, 8.0 / 9.0, 8.0 / 9.0 },
															   { 5.0 / 9.0, 8.0 / 9.0, 8.0 / 9.0 },
															   { 5.0 / 9.0, 5.0 / 9.0, 8.0 / 9.0 },
                                                               { 8.0 / 9.0, 5.0 / 9.0, 8.0 / 9.0 },
															   { 5.0 / 9.0, 5.0 / 9.0, 8.0 / 9.0 },
															   { 5.0 / 9.0, 5.0 / 9.0, 5.0 / 9.0 },
															   { 8.0 / 9.0, 5.0 / 9.0, 5.0 / 9.0 },
															   { 5.0 / 9.0, 5.0 / 9.0, 5.0 / 9.0 },
															   { 5.0 / 9.0, 8.0 / 9.0, 5.0 / 9.0 },
															   { 8.0 / 9.0, 8.0 / 9.0, 5.0 / 9.0 },
															   { 5.0 / 9.0, 8.0 / 9.0, 5.0 / 9.0 },
                                                               { 5.0 / 9.0, 5.0 / 9.0, 5.0 / 9.0 },
															   { 8.0 / 9.0, 5.0 / 9.0, 5.0 / 9.0 },
															   { 5.0 / 9.0, 5.0 / 9.0, 5.0 / 9.0 }, };


PANSFEM::GaussCubic2::GaussCubic2() {}


PANSFEM::GaussCubic2::~GaussCubic2() {}


Eigen::MatrixXd PANSFEM::GaussCubic2::Integrate(std::function<Eigen::MatrixXd(std::vector<double>)> _f) {
	Eigen::MatrixXd m = GaussCubic2::GW[0][0] * GaussCubic2::GW[0][1] * GaussCubic2::GW[0][2] * _f(GaussCubic2::GP[0]);
	for (int i = 1; i < 27; i++) {
		m += GaussCubic2::GW[i][0] * GaussCubic2::GW[i][1] * GaussCubic2::GW[0][2] * _f(this->GP[i]);
	}
	return m;
}


Eigen::VectorXd PANSFEM::GaussCubic2::Integrate(std::function<Eigen::VectorXd(std::vector<double>)> _f) {
	Eigen::VectorXd v = GaussCubic2::GW[0][0] * GaussCubic2::GW[0][1] * GaussCubic2::GW[0][2] * _f(GaussCubic2::GP[0]);
	for (int i = 1; i < 27; i++) {
		v += GaussCubic2::GW[i][0] * GaussCubic2::GW[i][1] * GaussCubic2::GW[0][2] * _f(GaussCubic2::GP[i]);
	}
	return v;
}


double PANSFEM::GaussCubic2::Integrate(std::function<double(std::vector<double>)> _f) {
	double d = GaussCubic2::GW[0][0] * GaussCubic2::GW[0][1] * GaussCubic2::GW[0][2] * _f(GaussCubic2::GP[0]);
	for (int i = 1; i < 27; i++) {
		d += GaussCubic2::GW[i][0] * GaussCubic2::GW[i][1] * GaussCubic2::GW[0][2] * _f(GaussCubic2::GP[i]);
	}
	return d;
}