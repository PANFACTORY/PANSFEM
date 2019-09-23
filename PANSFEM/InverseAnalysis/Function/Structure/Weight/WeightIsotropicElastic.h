//*************************************************************************************************
//Title		:PANSFEM/InversedAnalysis/Function/WeightIsotropicElastic/WeightIsotropicElastic.h
//Author	:Tanabe Yuta
//Date		:2019/09/22
//Copyright	:(C)2019 TanabeYuta
//*************************************************************************************************


#pragma once
#include "../../Function.h"


namespace PANSFEM {
	class WeightIsotropicElastic :
		public Function
	{
	public:
		WeightIsotropicElastic();
		virtual ~WeightIsotropicElastic();
		WeightIsotropicElastic(std::vector<int> _plist, std::vector<int> _refulist, std::vector<int> _refplist);


		Eigen::VectorXd sensitivitis();		//�֐����x�x�N�g����Ԃ�
		double value();						//�֐��l��Ԃ�
	};
}
