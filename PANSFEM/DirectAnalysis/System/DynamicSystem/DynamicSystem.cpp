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
	//----------��̃Z�b�e�B���O----------
	for (auto pfield : this->pfields) {
		//.....��̏�����.....
		pfield->Initialize();

		//.....Kmap�̍쐬.....
		pfield->MakeKmap();

		//.....�o���h���̌v�Z.....
		pfield->GetBandwidth();
	}

	//----------���Ԕ��W�v�Z-----------
	for (int i = 0; i < this->INC; i++) {
		std::cout << "inclement:\t" << i << std::endl;
		for (auto pfield : this->pfields) {
			//.....�v�f�\�ߓ_���������v�Z.....
			for (auto& pequation : pfield->pequations) {
				pequation->SetEquation();
			}

			//.....�S�́\�ߓ_������������.....
			pfield->SolveEquation();

			//.....���ʂ̏o��.....
			this->Export(this->FNAME + std::to_string(i));
		}
	}
}