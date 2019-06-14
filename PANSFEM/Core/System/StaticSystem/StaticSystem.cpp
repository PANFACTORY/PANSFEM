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
	//----------Kmap�̍쐬----------
	this->pfields[0]->MakeKmap();

	//----------�v�f�\�ߓ_���������v�Z----------
	for (auto& pequation : this->pfields[0]->pequations) {
		pequation->SetEquation();
	}

	//----------�o���h���̌v�Z----------
	this->pfields[0]->GetBandwidth();

	//----------�S�́\�ߓ_������������----------
	this->pfields[0]->SolveEquation();
}