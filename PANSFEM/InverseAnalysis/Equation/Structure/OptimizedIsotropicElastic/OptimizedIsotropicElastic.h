//***********************************************************************************************************
//Title		:PANSFEM/InverseAnalysis/Equation/Structure/OptimizedIsotropicElastic/OptimizedIsotropicElastic.h
//Author	:Tanabe Yuta
//Date		:2019/09/22
//Copyright	:(C)2019 TanabeYuta
//***********************************************************************************************************


#pragma once
#include "../../../../DirectAnalysis/Equation/Equation.h"


namespace PANSFEM {
	class OptimizedIsotropicElastic :
		public Equation
	{
	public:
		OptimizedIsotropicElastic();
		virtual ~OptimizedIsotropicElastic();
		OptimizedIsotropicElastic(Element *_pelement, std::vector<int> _ulist, std::vector<int> _refulist, std::vector<int> _plist, Integration* _pintegration);
	
	
		void SetEquation();			//óvëfÅ\êﬂì_ï˚íˆéÆÇê›íË
	};
}