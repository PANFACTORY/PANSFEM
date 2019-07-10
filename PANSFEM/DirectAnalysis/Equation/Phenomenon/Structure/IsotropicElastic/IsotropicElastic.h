//*****************************************************************************
//Title		:PANSFEM/Phenomenon/Structure/IsotropicElastic/IsotropicElastic.h
//Author	:Tanabe Yuta
//Date		:2019/07/10
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include <vector>


#include "../../../Equation.h"


namespace PANSFEM {
	class IsotropicElastic :
		public Equation
	{
	public:
		IsotropicElastic();
		virtual ~IsotropicElastic();
		IsotropicElastic(Element *_pelement, std::vector<int> _ulist, std::vector<int> _refulist, std::vector<int> _plist, Integration* _pintegration);


		double E;	//—v‘f‚ÌYoung—¦
		double V;	//—v‘f‚ÌPoisson”ä
		

		virtual Eigen::MatrixXd GetKe(std::vector<double> _xi);
		virtual Eigen::VectorXd GetFe(std::vector<double> _xi);
	};
}