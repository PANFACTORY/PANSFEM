//***********************************************************************************************************
//Title		:PANSFEM/InversedAnalysis/Function/Compliance/3PhaseMaterials3D/Compliance3PhaseMaterials3D.h
//Author	:Tanabe Yuta
//Date		:2019/10/14
//Copyright	:(C)2019 TanabeYuta
//***********************************************************************************************************


#pragma once
#include "../../../Function.h"


namespace PANSFEM {
	class Compliance3PhaseMaterials3D :
		public Function
	{
	public:
		Compliance3PhaseMaterials3D();
		virtual ~Compliance3PhaseMaterials3D();
		Compliance3PhaseMaterials3D(std::vector<int> _plist, std::vector<int> _refulist, std::vector<int> _refplist);


		Eigen::VectorXd sensitivitis();						//関数感度ベクトルを返す
		double value();										//関数値を返す
	};
}
