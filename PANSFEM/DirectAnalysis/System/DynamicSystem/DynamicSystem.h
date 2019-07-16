//*****************************************************************************
//Title		:PANSFEM/Core/System/DynamicSystem/DynamicSystem.h
//Author	:Tanabe Yuta
//Date		:2019/06/29
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include "../System.h"


namespace PANSFEM {
	class DynamicSystem : 
		public System
	{
	public:
		DynamicSystem();
		virtual ~DynamicSystem();
		DynamicSystem(int _DOX, int _DOU, int _INC, std::string _fname);


		const int INC;				//�C���N�������g��
		const std::string FNAME;	//�o�̓t�@�C�����̃w�b�_����


		void Schedule();
	};
}