//*****************************************************************************
//Title		:PANSFEM/Core/Field/Field.h
//Author	:Tanabe Yuta
//Date		:2019/06/08
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include <vector>


#include "../Equation/Equation.h"


namespace PANSFEM {
	class Field
	{
	public:
		Field();
		virtual ~Field();
		Field(std::vector<int> _ulist, std::vector<Equation*> _pequations);


		std::vector<Equation*> pequations;			//場を記述する要素―節点方程式
	};
}