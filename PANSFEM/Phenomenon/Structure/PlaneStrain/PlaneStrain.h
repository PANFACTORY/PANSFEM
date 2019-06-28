//*****************************************************************************
//Title		:PANSFEM/Phenomenon/Structure/PlaneStrain/PlaneStrain.h
//Author	:Tanabe Yuta
//Date		:2019/06/10
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include <vector>


#include "../../../Core/Equation/Equation.h"


namespace PANSFEM {
	class PlaneStrain :
		public Equation
	{
	public:
		PlaneStrain();
		virtual ~PlaneStrain();
		PlaneStrain(Element *_pelement, std::vector<int> _ulist, std::vector<double> _parameters, Integration* _pintegration);


		double E;	//—v‘f‚ÌYoung—¦
		double V;	//—v‘f‚ÌPoisson”ä
		double t;	//—v‘f‚ÌŒú‚³


		Eigen::MatrixXd GetKe(std::vector<double> _xi);
		Eigen::VectorXd GetFe(std::vector<double> _xi);
	};
}