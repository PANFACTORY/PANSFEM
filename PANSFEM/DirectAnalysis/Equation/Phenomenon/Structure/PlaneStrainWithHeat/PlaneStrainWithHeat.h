//***************************************************************************************
//Title		:PANSFEM/Phenomenon/Structure/PlaneStrainWithHeat/PlaneStrainWithHeat.h
//Author	:Tanabe Yuta
//Date		:2019/06/29
//Copyright	:(C)2019 TanabeYuta
//***************************************************************************************


#pragma once
#include "../../../Equation.h"


namespace PANSFEM {
	class PlaneStrainWithHeat : 
		public Equation
	{
	public:
		PlaneStrainWithHeat();
		virtual ~PlaneStrainWithHeat();
		PlaneStrainWithHeat(Element *_pelement, std::vector<int> _ulist, std::vector<int> _refulist, std::vector<int> _plist, Integration* _pintegration);


		void SetEquation();			//—v‘f\ß“_•û’ö®‚ğİ’è
	};
}