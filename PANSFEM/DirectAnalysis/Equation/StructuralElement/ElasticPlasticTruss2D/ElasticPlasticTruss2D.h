//***********************************************************************************************************
//Title		:PANSFEM/DirectAnalysis/Equation/StructualElement/ElasticPlasticTruss2D/ElasticPlasticTruss2D.h
//Author	:Tanabe Yuta
//Date		:2019/08/21
//Copyright	:(C)2019 TanabeYuta
//***********************************************************************************************************


#pragma once
#include "../../Equation.h"


namespace PANSFEM {
	class ElasticPlasticTruss2D :
		public Equation
	{
	public:
		ElasticPlasticTruss2D();
		virtual ~ElasticPlasticTruss2D();
		ElasticPlasticTruss2D(Element *_pelement, std::vector<int> _ulist, std::vector<int> _refulist, std::vector<int> _plist, Integration* _pintegration);


		void SetEquation();			//要素―節点方程式を設定


		double epspn;				//塑性ひずみ
		double epsbarpn;			//硬化パラメータ
	};
}