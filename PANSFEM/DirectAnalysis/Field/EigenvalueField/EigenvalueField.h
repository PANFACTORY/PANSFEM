//*****************************************************************************
//Title		:PANSFEM/DirectAnalysis/Field/EigenvalueField/EigenvalueField.h
//Author	:Tanabe Yuta
//Date		:2019/07/16
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include "../Field.h"


namespace PANSFEM {
	class EigenvalueField :
		public Field
	{
	public:
		EigenvalueField();
		virtual ~EigenvalueField();
		EigenvalueField(std::vector<int> _ulist);


		int NOE;				//���߂�ŗL�l�̐�


		void SolveEquation();	//�S�́\�ߓ_�������������i�ŗL�l���j
	};
}