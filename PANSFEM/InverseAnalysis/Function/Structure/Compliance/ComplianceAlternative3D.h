//*************************************************************************************************
//Title		:PANSFEM/InversedAnalysis/Function/ComplianceAlternative3D/ComplianceAlternative3D.h
//Author	:Tanabe Yuta
//Date		:2019/09/27
//Copyright	:(C)2019 TanabeYuta
//*************************************************************************************************


#pragma once
#include "../../Function.h"


namespace PANSFEM {
	class ComplianceAlternative3D :
		public Function
	{
	public:
		ComplianceAlternative3D();
		virtual ~ComplianceAlternative3D();
		ComplianceAlternative3D(std::vector<int> _plist, std::vector<int> _refulist, std::vector<int> _refplist);


		Eigen::VectorXd sensitivitis();						//関数感度ベクトルを返す
		double value();										//関数値を返す
	};
}