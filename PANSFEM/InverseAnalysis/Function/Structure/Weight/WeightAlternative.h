//*****************************************************************************
//Title		:PANSFEM/InversedAnalysis/Function/Weight/WeightAlternative.h
//Author	:Tanabe Yuta
//Date		:2019/08/03
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include "../../Function.h"


namespace PANSFEM {
	class WeightAlternative : 
		public Function
	{
	public:
		WeightAlternative();
		virtual ~WeightAlternative();
		WeightAlternative(std::vector<int> _plist, std::vector<int> _refulist, std::vector<int> _refplist);


		Eigen::VectorXd sensitivitis();		//関数感度ベクトルを返す
		double value();						//関数値を返す
	};
}