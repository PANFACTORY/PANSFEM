//*****************************************************************************
//Title		:PANSFEM/Phenomenon/Structure/PlaneStrainVibration/PlaneStrainVibration.h
//Author	:Tanabe Yuta
//Date		:2019/07/18
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include <vector>


#include "../../../Equation.h"


namespace PANSFEM {
	class PlaneStrainVibration :
		public Equation
	{
	public:
		PlaneStrainVibration();
		virtual ~PlaneStrainVibration();
		PlaneStrainVibration(Element *_pelement, std::vector<int> _ulist, std::vector<int> _refulist, std::vector<int> _plist, Integration* _pintegration);


		void SetEquation();			//óvëfÅ\êﬂì_ï˚íˆéÆÇê›íË
	};
}