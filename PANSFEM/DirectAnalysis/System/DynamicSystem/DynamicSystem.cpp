//*****************************************************************************
//Title		:PANSFEM/Core/System/DynamicSystem/DynamicSystem.cpp
//Author	:Tanabe Yuta
//Date		:2019/06/29
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "DynamicSystem.h"


PANSFEM::DynamicSystem::DynamicSystem() : System(), INC(0), FNAME("") {}


PANSFEM::DynamicSystem::~DynamicSystem(){}


PANSFEM::DynamicSystem::DynamicSystem(int _DOX, int _DOU, int _INC, std::string _fname) : System(_DOX, _DOU), INC(_INC), FNAME(_fname) {}


void PANSFEM::DynamicSystem::Schedule(){
	//----------場のセッティング----------
	for (auto pfield : this->pfields) {
		//.....場の初期化.....
		pfield->Initialize();

		//.....Kmapの作成.....
		pfield->MakeKmap();

		//.....バンド幅の計算.....
		pfield->GetBandwidth();
	}

	//----------時間発展計算-----------
	for (int i = 0; i < this->INC; i++) {
		std::cout << "inclement:\t" << i << std::endl;
		for (auto pfield : this->pfields) {
			//.....要素―節点方程式を計算.....
			for (auto& pequation : pfield->pequations) {
				pequation->SetEquation();
			}

			//.....全体―節点方程式を解く.....
			pfield->SolveEquation();

			//.....結果の出力.....
			this->Export(this->FNAME + std::to_string(i));
		}
	}
}