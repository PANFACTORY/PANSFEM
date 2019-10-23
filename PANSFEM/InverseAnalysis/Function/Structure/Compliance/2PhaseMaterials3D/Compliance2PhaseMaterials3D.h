//***********************************************************************************************************
//Title		:PANSFEM/InversedAnalysis/Function/Compliance/2PhaseMaterials3D/Compliance2PhaseMaterials3D.h
//Author	:Tanabe Yuta
//Date		:2019/10/22
//Copyright	:(C)2019 TanabeYuta
//***********************************************************************************************************


#pragma once
#include "../../../Function.h"


namespace PANSFEM {
	class Compliance2PhaseMaterials3D :
		public Function
	{
	public:
		Compliance2PhaseMaterials3D();
		virtual ~Compliance2PhaseMaterials3D();
		Compliance2PhaseMaterials3D(std::vector<int> _plist, std::vector<int> _refulist, std::vector<int> _refplist);


		Eigen::VectorXd sensitivitis();						//関数感度ベクトルを返す
		double value();										//関数値を返す
	};
}