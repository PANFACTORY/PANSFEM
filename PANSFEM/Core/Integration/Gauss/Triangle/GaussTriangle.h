//*****************************************************************************
//Title		:PANSFEM/Core/Integration/Gauss/Triangle/GaussTriangle.h
//Author	:Tanabe Yuta
//Date		:2019/06/08
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include "../../Integration.h"


namespace PANSFEM {
	class GaussTriangle :
		public Integration
	{
	public:
		GaussTriangle();
		virtual ~GaussTriangle();


		Eigen::MatrixXd Integrate(std::function<Eigen::MatrixXd(std::vector<double>)>);
		Eigen::VectorXd Integrate(std::function<Eigen::VectorXd(std::vector<double>)>);
	};
}