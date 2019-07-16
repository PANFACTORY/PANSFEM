//*****************************************************************************
//Title		:PANSFEM/Core/System/StaticSystem/StaticSystem.h
//Author	:Tanabe Yuta
//Date		:2019/06/11
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include "../System.h"


namespace PANSFEM {
	class StaticSystem : 
		public System
	{
	public:
		StaticSystem();
		virtual ~StaticSystem();
		StaticSystem(int _DOX, int _DOU);


		void Schedule();						//場の方程式を解く順番，タイミングを管理
	};
}