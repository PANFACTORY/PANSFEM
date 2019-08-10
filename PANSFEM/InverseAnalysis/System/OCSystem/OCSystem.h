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


		void Schedule();				//順解析，感度計算を行う順番，タイミングを管理
	};
}