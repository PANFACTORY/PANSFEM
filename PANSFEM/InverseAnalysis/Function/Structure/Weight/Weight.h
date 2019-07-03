//*****************************************************************************
//Title		:PANSFEM/InversedAnalysis/Function/Weight/Weight.h
//Author	:Tanabe Yuta
//Date		:2019/07/02
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include "../../Function.h"


namespace PANSFEM {
	class Weight :
		public Function
	{
	public:
		Weight();
		virtual ~Weight();
		Weight(std::vector<double> _plist);


		Eigen::VectorXd sensitivitis();		//�֐����x�x�N�g����Ԃ�
		double value();						//�֐��l��Ԃ�
	};
}