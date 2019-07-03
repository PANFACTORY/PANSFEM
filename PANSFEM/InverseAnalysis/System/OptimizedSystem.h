//*****************************************************************************
//Title		:PANSFEM/InversedAnalysis/System/OptimizedSystem.h
//Author	:Tanabe Yuta
//Date		:2019/06/30
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include "../../DirectAnalysis/System/System.h"
#include "../Function/Function.h"


namespace PANSFEM {
	class OptimizedSystem :
		public System
	{
	public:
		OptimizedSystem();
		virtual ~OptimizedSystem();
		OptimizedSystem(int _DOX, int _DOU);


		template<class F>
		bool ImportObjective(std::vector<int> _plist);		//�ړI�֐���ǉ�
		template<class F>
		bool ImportConstraint(std::vector<int> _plist);		//����֐���ǉ�


		void Schedule();						//����́C���x�v�Z���s�����ԁC�^�C�~���O���Ǘ�
												//�{���Ȃ�œK����@�ɂ���ĕς���ׂ������C��̓I�ȃC���[�W�����ނ��ߍ��͂�������
		void Export(std::string _fname);		//�œK���̌��ʂ��o��


	protected:
		std::vector<Function*> pobjectives;		//�ړI�֐����w���|�C���^
		std::vector<Function*> pconstraints;	//����֐����w���|�C���^
	};


	template<class F>
	inline bool OptimizedSystem::ImportObjective(std::vector<int> _plist){
		return false;
	}


	template<class F>
	inline bool OptimizedSystem::ImportConstraint(std::vector<int> _plist){
		return false;
	}
}