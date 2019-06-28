//*****************************************************************************
//Title		:PANSFEM/Core/Integration/Gauss/Square/GaussSquare.h
//Author	:Tanabe Yuta
//Date		:2019/06/08
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include "../../Integration.h"


namespace PANSFEM {
	class GaussSquare :
		public Integration
	{
	public:
		GaussSquare();
		virtual ~GaussSquare();


		Eigen::MatrixXd Integrate(std::function<Eigen::MatrixXd(std::vector<double>)>);
		Eigen::VectorXd Integrate(std::function<Eigen::VectorXd(std::vector<double>)>);


		static std::vector<std::vector<double> > GP;
		static std::vector<std::vector<double> > GW;
	};	
}