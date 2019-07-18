//*****************************************************************************
//Title		:PANSFEM/Core/Equation/StructualElement/Beam2D/Beam2D.h
//Author	:Tanabe Yuta
//Date		:2019/07/14
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include <vector>


#include "../../Equation.h"


namespace PANSFEM {
	class Beam2D :
		public Equation
	{
	public:
		Beam2D();
		virtual ~Beam2D();
		Beam2D(Element *_pelement, std::vector<int> _ulist, std::vector<int> _refulist, std::vector<int> _plist, Integration* _pintegration);


		void SetEquation();			//—v‘f\ß“_•û’ö®‚ğİ’è
	};
}