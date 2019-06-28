//*****************************************************************************
//Title		:PANSFEM/Core/Field/LinearField/LinearField.h
//Author	:Tanabe Yuta
//Date		:2019/06/12
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include <vector>


#include "../Field.h"


namespace PANSFEM {
	class LinearField : 
		public Field
	{
	public:
		LinearField();
		virtual ~LinearField();
		LinearField(std::vector<int> _ulist);
	};
}