//*****************************************************************************
//Title		:PANSFEM/Core/Integration/Gauss/Cubic/GaussCubic.h
//Author	:Tanabe Yuta
//Date		:2019/07/10
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include "../../Integration.h"


namespace PANSFEM {
	class GaussCubic :
		public Integration
	{
	public:
		GaussCubic();
		virtual ~GaussCubic();


		Eigen::MatrixXd Integrate(std::function<Eigen::MatrixXd(std::vector<double>)>);
		Eigen::VectorXd Integrate(std::function<Eigen::VectorXd(std::vector<double>)>);
		double Integrate(std::function<double(std::vector<double>)>);


		static std::vector<std::vector<double> > GP;
		static std::vector<std::vector<double> > GW;
	};
}