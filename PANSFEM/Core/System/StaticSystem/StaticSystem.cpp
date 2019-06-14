//*****************************************************************************
//Title		:PANSFEM/Core/System/StaticSystem/StaticSystem.cpp
//Author	:Tanabe Yuta
//Date		:2019/06/11
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "StaticSystem.h"


PANSFEM::StaticSystem::StaticSystem() : System() {}


PANSFEM::StaticSystem::~StaticSystem(){}


PANSFEM::StaticSystem::StaticSystem(int _DOX, int _DOU) : System(_DOX, _DOU){}


void PANSFEM::StaticSystem::Schedule(){
	//----------Kmapの作成----------
	this->pfields[0]->MakeKmap();

	//----------要素―節点方程式を計算----------
	for (auto& pequation : this->pfields[0]->pequations) {
		pequation->SetEquation();
	}

	//----------バンド幅の計算----------
	this->pfields[0]->GetBandwidth();

	//----------全体―節点方程式を解く----------
	this->pfields[0]->SolveEquation();
}