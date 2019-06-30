//*****************************************************************************
//Title		:PANSFEM/Phenomenon/HeatTransfer/Dynamic2D/HeatTransferDynamic2D.cpp
//Author	:Tanabe Yuta
//Date		:2019/06/29
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include "../../../Equation.h"


namespace PANSFEM {
	class HeatTransferDynamic2D :
		public Equation
	{
	public:
		HeatTransferDynamic2D();
		virtual ~HeatTransferDynamic2D();
		HeatTransferDynamic2D(Element *_pelement, std::vector<int> _ulist, std::vector<int> _refulist, std::vector<int> _plist, Integration* _pintegration);
	
	
		double rho;
		double c;
		double k;
		double dt;
		double theta;


		Eigen::MatrixXd GetKe(std::vector<double> _xi);
		Eigen::VectorXd GetFe(std::vector<double> _xi);
	};
}


