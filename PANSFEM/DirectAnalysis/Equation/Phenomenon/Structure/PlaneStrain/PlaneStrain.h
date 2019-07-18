//*****************************************************************************
//Title		:PANSFEM/Phenomenon/Structure/PlaneStrain/PlaneStrain.h
//Author	:Tanabe Yuta
//Date		:2019/06/10
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include <vector>


#include "../../../Equation.h"


namespace PANSFEM {
	class PlaneStrain :
		public Equation
	{
	public:
		PlaneStrain();
		virtual ~PlaneStrain();
		PlaneStrain(Element *_pelement, std::vector<int> _ulist, std::vector<int> _refulist, std::vector<int> _plist, Integration* _pintegration);


		void SetEquation();			//—v‘f\ß“_•û’ö®‚ğİ’è
	};
}