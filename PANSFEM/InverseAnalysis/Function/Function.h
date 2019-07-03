//*****************************************************************************
//Title		:PANSFEM/InversedAnalysis/Function/Function.h
//Author	:Tanabe Yuta
//Date		:2019/06/30
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include <vector>


#include "../../DirectAnalysis/Element/Element.h"


namespace PANSFEM {
	class Function
	{
	public:
		Function();
		virtual ~Function();
		Function(std::vector<int> _plist);


		std::vector<int> pf_to_ps;						//�݌v�ϐ��̊֐����ԍ����n�̃p�����[�^�ԍ�
		std::vector<Element*> pelements;				//�݌v�ϐ��̒�`���ꂽ�v�f


		virtual Eigen::VectorXd sensitivitis() = 0;		//�֐����x�x�N�g����Ԃ�
		virtual double value() = 0;						//�֐��l��Ԃ�		
	};
}