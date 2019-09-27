//*************************************************************************************************
//Title		:PANSFEM/InversedAnalysis/Function/WeightAlternative3D/WeightAlternative3D.h
//Author	:Tanabe Yuta
//Date		:2019/09/27
//Copyright	:(C)2019 TanabeYuta
//*************************************************************************************************


#pragma once
#include "../../Function.h"


namespace PANSFEM {
	class WeightAlternative3D :
		public Function
	{
	public:
		WeightAlternative3D();
		virtual ~WeightAlternative3D();
		WeightAlternative3D(std::vector<int> _plist, std::vector<int> _refulist, std::vector<int> _refplist);


		Eigen::VectorXd sensitivitis();						//関数感度ベクトルを返す
		double value();										//関数値を返す
	};
}