//*************************************************************************************************
//Title		:PANSFEM/InversedAnalysis/Function/ComplianceAlternative/ComplianceAlternative.h
//Author	:Tanabe Yuta
//Date		:2019/08/03
//Copyright	:(C)2019 TanabeYuta
//*************************************************************************************************


#pragma once
#include "../../Function.h"


namespace PANSFEM {
	class ComplianceAlternative :
		public Function
	{
	public:
		ComplianceAlternative();
		virtual ~ComplianceAlternative();
		ComplianceAlternative(std::vector<int> _plist, std::vector<int> _refulist, std::vector<int> _refplist);


		Eigen::VectorXd sensitivitis();						//関数感度ベクトルを返す
		double value();										//関数値を返す
	};
}