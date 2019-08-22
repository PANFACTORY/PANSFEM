//*****************************************************************************
//Title		:PANSFEM/Core/System/StaticSystem/StaticSystem.cpp
//Author	:Tanabe Yuta
//Date		:2019/06/11
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include <chrono>


#include "StaticSystem.h"


PANSFEM::StaticSystem::StaticSystem() : System() {}


PANSFEM::StaticSystem::~StaticSystem(){}


PANSFEM::StaticSystem::StaticSystem(int _DOX, int _DOU) : System(_DOX, _DOU){}


void PANSFEM::StaticSystem::Schedule(){
	for (auto pfield : this->pfields) {
		//----------場の初期化----------
		pfield->Initialize();

		//----------Kmapの作成----------
		pfield->MakeKmap();
				
		//----------バンド幅の計算----------
		pfield->GetBandwidth();

		//----------全体―節点方程式を解く----------
		pfield->SolveEquation();
	}
}