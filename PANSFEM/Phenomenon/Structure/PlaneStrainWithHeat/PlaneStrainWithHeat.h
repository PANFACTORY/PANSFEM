//***************************************************************************************
//Title		:PANSFEM/Phenomenon/Structure/PlaneStrainWithHeat/PlaneStrainWithHeat.h
//Author	:Tanabe Yuta
//Date		:2019/06/29
//Copyright	:(C)2019 TanabeYuta
//***************************************************************************************


#pragma once
#include "../../../Core/Equation/Equation.h"


namespace PANSFEM {
	class PlaneStrainWithHeat : 
		public Equation
	{
	public:
		PlaneStrainWithHeat();
		virtual ~PlaneStrainWithHeat();
		PlaneStrainWithHeat(Element *_pelement, std::vector<int> _ulist, std::vector<int> _refulist, std::vector<int> _plist, Integration* _pintegration);


		double E;	//—v‘f‚ÌYoung—¦
		double V;	//—v‘f‚ÌPoisson”ä
		double t;	//—v‘f‚ÌŒú‚³
		double alpha;	//”M–c’£—¦


		virtual Eigen::MatrixXd GetKe(std::vector<double> _xi);
		virtual Eigen::VectorXd GetFe(std::vector<double> _xi);
	};
}