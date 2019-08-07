//***********************************************************************************************************
//Title		:PANSFEM/InversedAnalysis/Structure/OptimizedAlternative2D/OptimizedAltanative3Phase2D.h
//Author	:Tanabe Yuta
//Date		:2019/08/07
//Copyright	:(C)2019 TanabeYuta
//***********************************************************************************************************


#pragma once
#include "../../../../DirectAnalysis/Equation/Equation.h"


namespace PANSFEM {
	class OptimizedAlternative3Phase2D :
		public Equation
	{
	public:
		OptimizedAlternative3Phase2D();
		virtual ~OptimizedAlternative3Phase2D();
		OptimizedAlternative3Phase2D(Element *_pelement, std::vector<int> _ulist, std::vector<int> _refulist, std::vector<int> _plist, Integration* _pintegration);


		void SetEquation();			//—v‘f\ß“_•û’ö®‚ğİ’è
	};
}