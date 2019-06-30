//*****************************************************************************
//Title		:PANSFEM/InversedAnalysis/System/OptimizedSystem.cpp
//Author	:Tanabe Yuta
//Date		:2019/06/30
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "OptimizedSystem.h"


PANSFEM::OptimizedSystem::OptimizedSystem() : System(){}


PANSFEM::OptimizedSystem::~OptimizedSystem(){}


PANSFEM::OptimizedSystem::OptimizedSystem(int _DOX, int _DOU) : System(_DOX, _DOU){}


void PANSFEM::OptimizedSystem::Schedule(){
	//----------設計変数の初期化----------

	//----------最適化の反復計算----------
	for (int itr = 0; itr < 10; itr++) {
		//----------順解析----------

		//----------目的関数の計算----------

		//----------収束判定----------

		//----------設計感度を計算----------

		//----------Lagrange乗数を求める反復計算----------

		//----------設計変数の更新----------

		//----------目的関数の値を更新----------
		
	}
}


void PANSFEM::OptimizedSystem::Export(std::string _fname){
}