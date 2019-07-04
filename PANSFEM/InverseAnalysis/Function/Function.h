//*****************************************************************************
//Title		:PANSFEM/InversedAnalysis/Function/Function.h
//Author	:Tanabe Yuta
//Date		:2019/06/30
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include <vector>


#include "../../DirectAnalysis/Element/Element.h"
#include "../../DirectAnalysis/Integration/Integration.h"


namespace PANSFEM {
	class Function
	{
	public:
		Function();
		virtual ~Function();
		Function(std::vector<int> _plist, std::vector<int> _refulist, std::vector<int> _refplist, int _NOP, int _DOREFU, int _NOREFP);


		const int NOP;										//��v�f������̐݌v�ϐ��̐�
		const int DOREFU;									//�Q�Ƃ���]���ϐ��̐�
		const int NOREFP;									//�Q�Ƃ���p�����[�^�̐�


		std::vector<int> pf_to_ps;							//�݌v�ϐ��̊֐����ԍ����n�̃p�����[�^�ԍ�
		std::vector<int> refuf_to_us;						//�֐����Q�Ƃ���]���ϐ��̊֐����ԍ����n�̏]���ϐ��ԍ�
		std::vector<int> refpf_to_us;						//�֐����Q�Ƃ���p�����[�^�̊֐����ԍ����n�̃p�����[�^�ԍ�
		std::vector<Element*> pelements;					//�݌v�ϐ��̒�`���ꂽ�v�f
		std::map<Element*, Integration*> pintegrations;		//�v�f���̐ϕ��@


		virtual Eigen::VectorXd sensitivitis() = 0;			//�֐����x�x�N�g����Ԃ�
		virtual double value() = 0;							//�֐��l��Ԃ�		
	};
}