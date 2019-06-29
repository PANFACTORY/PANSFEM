//*****************************************************************************
//Title		:PANSFEM/Phenomenon/HeatTransfer/Static2D/HeatTransferStatic2D.h
//Author	:Tanabe Yuta
//Date		:2019/06/25
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include "../../../Core/Equation/Equation.h"


namespace PANSFEM {
	class HeatTransferStatic2D :
		public Equation
	{
	public:
		HeatTransferStatic2D();
		virtual ~HeatTransferStatic2D();
		HeatTransferStatic2D(Element *_pelement, std::vector<int> _ulist, std::vector<int> _refulist, std::vector<int> _plist, Integration* _pintegration);


		double alpha;			//”M“`“±—¦


		Eigen::MatrixXd GetKe(std::vector<double> _xi);
		Eigen::VectorXd GetFe(std::vector<double> _xi);
	};
}