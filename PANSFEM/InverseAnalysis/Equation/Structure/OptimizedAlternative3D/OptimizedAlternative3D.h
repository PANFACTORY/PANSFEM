//*************************************************************************************************
//Title		:PANSFEM/InversedAnalysis/Structure/OptimizedAlternative3D/OptimizedAltanative3D.h
//Author	:Tanabe Yuta
//Date		:2019/09/27
//Copyright	:(C)2019 TanabeYuta
//*************************************************************************************************


#pragma once
#include "../../../../DirectAnalysis/Equation/Equation.h"


namespace PANSFEM {
	class OptimizedAlternative3D :
		public Equation
	{
	public:
		OptimizedAlternative3D();
		virtual ~OptimizedAlternative3D();
		OptimizedAlternative3D(Element *_pelement, std::vector<int> _ulist, std::vector<int> _refulist, std::vector<int> _plist, Integration* _pintegration);


		void SetEquation();			//—v‘f\ß“_•û’ö®‚ğİ’è
	};
}