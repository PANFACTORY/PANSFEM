//*****************************************************************************
//Title		:PANSFEM/InversedAnalysis/System/OptimizedSystem.h
//Author	:Tanabe Yuta
//Date		:2019/06/30
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include "../../DirectAnalysis/System/System.h"
#include "../Function/Function.h"


namespace PANSFEM {
	class OptimizedSystem :
		public System
	{
	public:
		OptimizedSystem();
		virtual ~OptimizedSystem();
		OptimizedSystem(int _DOX, int _DOU);


		void Schedule();						//順解析，感度計算を行う順番，タイミングを管理
												//本来なら最適化手法によって変えるべき部分，具体的なイメージをつかむため今はこうする
		void Export(std::string _fname);		//最適化の結果を出力


	protected:
		std::vector<Function*> pobjectives;		//目的関数を指すポインタ
		std::vector<Function*> pconstraints;	//制約条件を指すポインタ
	};
}