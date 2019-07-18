//***********************************************************************************************************
//Title		:PANSFEM/InverseAnalysis/Equation/Structure/OptimizedPlaneStrain/OptimizedPlaneStrain.h
//Author	:Tanabe Yuta
//Date		:2019/07/07
//Copyright	:(C)2019 TanabeYuta
//***********************************************************************************************************


#pragma once
#include "../../../../DirectAnalysis/Equation/Equation.h"


namespace PANSFEM {
	class OptimizedPlaneStrain : 
		public Equation
	{
	public:
		OptimizedPlaneStrain();
		virtual ~OptimizedPlaneStrain();
		OptimizedPlaneStrain(Element *_pelement, std::vector<int> _ulist, std::vector<int> _refulist, std::vector<int> _plist, Integration* _pintegration);

		
		void SetEquation();			//óvëfÅ\êﬂì_ï˚íˆéÆÇê›íË
	};
}