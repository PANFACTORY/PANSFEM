//*************************************************************************************************
//Title		:PANSFEM/InversedAnalysis/Structure/OptimizedAlternative2D/OptimizedAltanative2D.h
//Author	:Tanabe Yuta
//Date		:2019/08/02
//Copyright	:(C)2019 TanabeYuta
//*************************************************************************************************


#pragma once
#include "../../../../DirectAnalysis/Equation/Equation.h"


namespace PANSFEM {
	class OptimizedAlternative2D :
		public Equation
	{
	public:
		OptimizedAlternative2D();
		virtual ~OptimizedAlternative2D();
		OptimizedAlternative2D(Element *_pelement, std::vector<int> _ulist, std::vector<int> _refulist, std::vector<int> _plist, Integration* _pintegration);


		void SetEquation();			//—v‘f\ß“_•û’ö®‚ğİ’è
	};
}