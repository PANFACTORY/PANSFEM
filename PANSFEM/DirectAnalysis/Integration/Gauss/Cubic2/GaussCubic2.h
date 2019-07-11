//*****************************************************************************
//Title		:PANSFEM/Core/Integration/Gauss/Cubic2/GaussCubic2.h
//Author	:Tanabe Yuta
//Date		:2019/07/11
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include "../../Integration.h"


namespace PANSFEM {
	class GaussCubic2 :
		public Integration
	{
	public:
		GaussCubic2();
		virtual ~GaussCubic2();


		Eigen::MatrixXd Integrate(std::function<Eigen::MatrixXd(std::vector<double>)>);
		Eigen::VectorXd Integrate(std::function<Eigen::VectorXd(std::vector<double>)>);
		double Integrate(std::function<double(std::vector<double>)>);


		static std::vector<std::vector<double> > GP;
		static std::vector<std::vector<double> > GW;
	};
}