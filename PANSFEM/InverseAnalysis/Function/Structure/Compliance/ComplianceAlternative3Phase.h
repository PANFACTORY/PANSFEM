//***********************************************************************************************************
//Title		:PANSFEM/InversedAnalysis/Function/ComplianceAlternative3Phase/ComplianceAlternative3Phase.h
//Author	:Tanabe Yuta
//Date		:2019/08/07
//Copyright	:(C)2019 TanabeYuta
//***********************************************************************************************************


#pragma once
#include "../../Function.h"


namespace PANSFEM {
	class ComplianceAlternative3Phase :
		public Function
	{
	public:
		ComplianceAlternative3Phase();
		virtual ~ComplianceAlternative3Phase();
		ComplianceAlternative3Phase(std::vector<int> _plist, std::vector<int> _refulist, std::vector<int> _refplist);


		Eigen::VectorXd sensitivitis();						//関数感度ベクトルを返す
		double value();										//関数値を返す
	};
}