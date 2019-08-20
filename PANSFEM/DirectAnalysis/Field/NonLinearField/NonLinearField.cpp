//*****************************************************************************
//Title		:PANSFEM/DirectAnalysis/Field/NonLinearField/NonLinearField.cpp
//Author	:Tanabe Yuta
//Date		:2019/06/12
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "NonLinearField.h"


PANSFEM::NonLinearField::NonLinearField() : LinearField() {}


PANSFEM::NonLinearField::~NonLinearField() {}


PANSFEM::NonLinearField::NonLinearField(std::vector<int> _ulist) : LinearField(_ulist) {}


void PANSFEM::NonLinearField::SolveEquation() {
	//----------荷重増分のループ----------
	for (int incf = 0; incf < 1; incf) {
		//----------荷重増分の設定----------
		//----------荷重更新----------
		//----------残差計算----------
		//----------Newton-Raphson法のループ----------
		do {
			//----------全体増分方程式の求解----------
			//----------配置の更新----------
			//----------載荷/除荷の判定----------
			//----------塑性ひずみ，硬化パラメータ----------
			//----------応力，接線係数の算出----------
			//----------
		} while (1);
	}
}