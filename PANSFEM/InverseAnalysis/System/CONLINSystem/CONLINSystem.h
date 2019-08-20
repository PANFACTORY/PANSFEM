//*****************************************************************************
//Title		:PANSFEM/InversedAnalysis/System/CONLINSystem/CONLINSystem.h
//Author	:Tanabe Yuta
//Date		:2019/08/15
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include "../OptimizedSystem.h"


namespace PANSFEM {
	class CONLINSystem :
		public OptimizedSystem
	{
	public:
		CONLINSystem();
		virtual ~CONLINSystem();
		CONLINSystem(int _DOX, int _DOU, std::vector<int> _plist);


		void Schedule();				//����́C���x�v�Z���s�����ԁC�^�C�~���O���Ǘ�
	};
}