//***********************************************************************************************************
//Title		:PANSFEM/InversedAnalysis/Function/Weight/2PhaseMaterials3D/Weight2PhaseMaterials3D.h
//Author	:Tanabe Yuta
//Date		:2019/10/22
//Copyright	:(C)2019 TanabeYuta
//***********************************************************************************************************


#pragma once
#include "../../../Function.h"


namespace PANSFEM {
	class Weight2PhaseMaterials3D :
		public Function
	{
	public:
		Weight2PhaseMaterials3D();
		virtual ~Weight2PhaseMaterials3D();
		Weight2PhaseMaterials3D(std::vector<int> _plist, std::vector<int> _refulist, std::vector<int> _refplist);


		Eigen::VectorXd sensitivitis();						//�֐����x�x�N�g����Ԃ�
		double value();										//�֐��l��Ԃ�
	};
}