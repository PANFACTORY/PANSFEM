//*****************************************************************************
//Title		:PANSFEM/Core/System/DynamicSystem/DynamicSystem.cpp
//Author	:Tanabe Yuta
//Date		:2019/06/29
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "DynamicSystem.h"


PANSFEM::DynamicSystem::DynamicSystem() : System(), INC(0) {}


PANSFEM::DynamicSystem::~DynamicSystem(){}


PANSFEM::DynamicSystem::DynamicSystem(int _DOX, int _DOU, int _INC) : System(_DOX, _DOU), INC(_INC){}


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
			this->Export("Data/Output/model6/model6-" + std::to_string(i));
		}
	}
}


void PANSFEM::DynamicSystem::Export(std::string _fname){
	std::ofstream fout(_fname + ".vtk");

	//----------Header�̏o��----------
	fout << "# vtk DataFile Version 4.1\n";
	fout << "vtk output\n";
	fout << "ASCII\n";
	fout << "DATASET UNSTRUCTURED_GRID\n";

	//----------�ߓ_�̏o��----------
	fout << "\nPOINTS\t" << this->pnodes.size() << "\tfloat\n";
	for (auto pnode : this->pnodes) {
		int i = 0;
		for (; i < this->DOX; i++) {
			fout << pnode->x(i) << "\t";
		}
		for (; i < 3; i++) {
			fout << "0.0" << "\t";
		}
		fout << std::endl;
	}

	//----------�v�f�̒ǉ�----------
	int datanum = 0;
	for (auto pelement : this->pelements) {
		datanum += pelement->NON + 1;
	}

	std::map<Node*, int> nodelist;
	int nodenum = 0;
	for (auto pnode : this->pnodes) {
		nodelist[pnode] = nodenum;
		nodenum++;
	}

	fout << "\nCELLS " << this->pelements.size() << "\t" << datanum << "\n";
	for (auto pelement : this->pelements) {
		fout << pelement->NON << "\t";
		for (auto pnode : pelement->pnodes) {
			fout << nodelist[pnode] << "\t";
		}
		fout << std::endl;
	}

	//----------�v�f�^�C�v�̐ݒ�----------
	//�v�C��
	fout << "\nCELL_TYPES\t" << this->pelements.size() << "\n";
	for (auto pelement : this->pelements) {
		fout << "9\n";
	}

	//----------�ߓ_�̒l��ǉ�----------
	fout << "\nPOINT_DATA\t" << this->pnodes.size() << "\n";
	for (int i = 0; i < this->pfields.size(); i++) {
		//.....�X�J���[��.....
		if (this->pfields[i]->uf_to_us.size() == 1) {
			fout << "SCALARS u" << i << " float\n";
			fout << "LOOKUP_TABLE default\n";

			for (auto pnode : this->pnodes) {
				fout << pnode->u[this->pfields[i]->uf_to_us[0]] << std::endl;
			}
		}

		//.....�x�N�g����.....
		else {
			fout << "VECTORS u" << i << " float\n";
			if (this->pfields[i]->uf_to_us.size() == 2) {
				for (auto pnode : this->pnodes) {
					fout << pnode->u[this->pfields[i]->uf_to_us[0]] << "\t" << pnode->u[this->pfields[i]->uf_to_us[1]] << "\t" << "0" << std::endl;
				}
			}
			else if (this->pfields[i]->uf_to_us.size() == 3) {
				for (auto pnode : this->pnodes) {
					fout << pnode->u[this->pfields[i]->uf_to_us[0]] << "\t" << pnode->u[this->pfields[i]->uf_to_us[1]] << "\t" << pnode->u[this->pfields[i]->uf_to_us[2]] << std::endl;
				}
			}
		}
	}

	fout.close();
}