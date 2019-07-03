//*****************************************************************************
//Title		:PANSFEM/InversedAnalysis/Function/Function.h
//Author	:Tanabe Yuta
//Date		:2019/06/30
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include <vector>


#include "../../DirectAnalysis/Element/Element.h"


namespace PANSFEM {
	class Function
	{
	public:
		Function();
		virtual ~Function();
		Function(std::vector<int> _plist);


		std::vector<int> pf_to_ps;						//設計変数の関数内番号→系のパラメータ番号
		std::vector<Element*> pelements;				//設計変数の定義された要素


		virtual Eigen::VectorXd sensitivitis() = 0;		//関数感度ベクトルを返す
		virtual double value() = 0;						//関数値を返す		
	};
}