//*****************************************************************************
//Title		:PANSFEM/DirectAnalysis/Field/NonLinearField/NonLinearField.h
//Author	:Tanabe Yuta
//Date		:2019/06/12
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include "../Field.h"


namespace PANSFEM {
	class NonLinearField :
		public Field
	{
	public:
		NonLinearField();
		virtual ~NonLinearField();
		NonLinearField(std::vector<int> _ulist);


		void SolveEquation();			//�S�́\�ߓ_�������������i����`�A���������j
	};
}