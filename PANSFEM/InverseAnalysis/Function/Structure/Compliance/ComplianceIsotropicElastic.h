//***********************************************************************************************************
//Title		:PANSFEM/InversedAnalysis/Function/ComplianceIsotropicElastic/ComplianceIsotropicElastic.h
//Author	:Tanabe Yuta
//Date		:2019/09/22
//Copyright	:(C)2019 TanabeYuta
//***********************************************************************************************************


#pragma once
#include "../../Function.h"

namespace PANSFEM {
	class ComplianceIsotropicElastic :
		public Function
	{
	public:
		ComplianceIsotropicElastic();
		virtual ~ComplianceIsotropicElastic();
		ComplianceIsotropicElastic(std::vector<int> _plist, std::vector<int> _refulist, std::vector<int> _refplist);


		Eigen::VectorXd sensitivitis();						//�֐����x�x�N�g����Ԃ�
		double value();										//�֐��l��Ԃ�
	};
}
