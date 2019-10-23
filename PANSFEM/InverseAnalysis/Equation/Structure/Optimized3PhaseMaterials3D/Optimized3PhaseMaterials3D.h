//***********************************************************************************************************
//Title		:PANSFEM/InversedAnalysis/Structure/Optimized3PhaseMaterials3D/Optimized3PhaseMaterials3D.h
//Author	:Tanabe Yuta
//Date		:2019/10/14
//Copyright	:(C)2019 TanabeYuta
//***********************************************************************************************************


#pragma once
#include "../../../../DirectAnalysis/Equation/Equation.h"


namespace PANSFEM {
	class Optimized3PhaseMaterials3D :
		public Equation
	{
	public:
		Optimized3PhaseMaterials3D();
		virtual ~Optimized3PhaseMaterials3D();
		Optimized3PhaseMaterials3D(Element *_pelement, std::vector<int> _ulist, std::vector<int> _refulist, std::vector<int> _plist, Integration* _pintegration);


		void SetEquation();			//—v‘f\ß“_•û’ö®‚ğİ’è
	};
}
