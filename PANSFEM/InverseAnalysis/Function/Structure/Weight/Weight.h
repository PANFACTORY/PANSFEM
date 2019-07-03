//*****************************************************************************
//Title		:PANSFEM/InversedAnalysis/Function/Weight/Weight.h
//Author	:Tanabe Yuta
//Date		:2019/07/02
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include "../../Function.h"


namespace PANSFEM {
	class Weight :
		public Function
	{
	public:
		Weight();
		virtual ~Weight();
		Weight(std::vector<double> _plist);


		Eigen::VectorXd sensitivitis();		//関数感度ベクトルを返す
		double value();						//関数値を返す
	};
}