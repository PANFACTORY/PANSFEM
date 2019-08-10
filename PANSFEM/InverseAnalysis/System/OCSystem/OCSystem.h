//*****************************************************************************
//Title		:PANSFEM/InversedAnalysis/System/OCSystem/OCSystem.h
//Author	:Tanabe Yuta
//Date		:2019/08/10
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include "../OptimizedSystem.h"


namespace PANSFEM {
	class OCSystem :
		public OptimizedSystem
	{
	public:
		OCSystem();
		virtual ~OCSystem();
		OCSystem(int _DOX, int _DOU, std::vector<int> _plist);


		void Schedule();				//����́C���x�v�Z���s�����ԁC�^�C�~���O���Ǘ�
	};
}