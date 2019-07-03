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
	//----------����͂̐ݒ�----------
	for (auto pfield : this->pfields) {
		//.....��̏�����.....
		pfield->Initialize();

		//.....Kmap�̍쐬.....
		pfield->MakeKmap();

		//.....�o���h���̌v�Z.....
		pfield->GetBandwidth();
	}
	
	//----------�݌v�ϐ��̏�����----------

	//----------�œK���̔����v�Z----------
	double previousvalue = 0.0;		//1�X�e�b�v�O�̖ړI�֐��̒l
	for (int itr = 0; itr < 10; itr++) {
		//----------�����----------
		for (auto pfield : this->pfields) {
			//.....�v�f�\�ߓ_���������v�Z.....
			for (auto& pequation : pfield->pequations) {
				pequation->SetEquation();
			}

			//.....�S�́\�ߓ_������������.....
			pfield->SolveEquation();
		}

		//----------�ړI�֐��̌v�Z----------

		//----------��������----------

		//----------�݌v���x���v�Z----------

		//----------Lagrange�搔�����߂锽���v�Z----------

		//----------�݌v�ϐ��̍X�V----------

		//----------�ړI�֐��̒l���X�V----------
		
	}
}


void PANSFEM::OptimizedSystem::Export(std::string _fname){
}