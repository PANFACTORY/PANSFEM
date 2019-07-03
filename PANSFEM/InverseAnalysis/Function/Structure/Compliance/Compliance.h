//*****************************************************************************
//Title		:PANSFEM/InversedAnalysis/Function/Compliance/Compliance.h
//Author	:Tanabe Yuta
//Date		:2019/07/02
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include "../../Function.h"


namespace PANSFEM {
	class Compliance :
		public Function
	{
	public:
		Compliance();
		virtual ~Compliance();
		Compliance(std::vector<int> _plist);


		Eigen::VectorXd sensitivitis();		//�֐����x�x�N�g����Ԃ�
		double value();						//�֐��l��Ԃ�
	};
}