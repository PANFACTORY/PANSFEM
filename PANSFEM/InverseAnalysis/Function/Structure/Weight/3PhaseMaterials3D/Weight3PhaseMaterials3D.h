//***********************************************************************************************************
//Title		:PANSFEM/InversedAnalysis/Function/Weight/3PhaseMaterials3D/Weight3PhaseMaterials3D.h
//Author	:Tanabe Yuta
//Date		:2019/10/14
//Copyright	:(C)2019 TanabeYuta
//***********************************************************************************************************


#pragma once
#include "../../../Function.h"


namespace PANSFEM {
	class Weight3PhaseMaterials3D :
		public Function
	{
	public:
		Weight3PhaseMaterials3D();
		virtual ~Weight3PhaseMaterials3D();
		Weight3PhaseMaterials3D(std::vector<int> _plist, std::vector<int> _refulist, std::vector<int> _refplist);


		Eigen::VectorXd sensitivitis();						//関数感度ベクトルを返す
		double value();										//関数値を返す
	};
}