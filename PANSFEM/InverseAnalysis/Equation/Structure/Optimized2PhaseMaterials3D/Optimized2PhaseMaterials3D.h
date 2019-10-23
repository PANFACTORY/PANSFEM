//***********************************************************************************************************
//Title		:PANSFEM/InversedAnalysis/Structure/Optimized2PhaseMaterials3D/Optimized2PhaseMaterials3D.h
//Author	:Tanabe Yuta
//Date		:2019/10/22
//Copyright	:(C)2019 TanabeYuta
//***********************************************************************************************************


#pragma once
#include "../../../../DirectAnalysis/Equation/Equation.h"


namespace PANSFEM {
	class Optimized2PhaseMaterials3D :
		public Equation
	{
	public:
		Optimized2PhaseMaterials3D();
		virtual ~Optimized2PhaseMaterials3D();
		Optimized2PhaseMaterials3D(Element *_pelement, std::vector<int> _ulist, std::vector<int> _refulist, std::vector<int> _plist, Integration* _pintegration);


		void SetEquation();			//�v�f�\�ߓ_��������ݒ�
	};
}