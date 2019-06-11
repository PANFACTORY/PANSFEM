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
	//----------‘S‘Ì\ß“_•û’öŽ®‚ð‰ð‚­----------
	this->pfields[0]->SolveEquation();
}