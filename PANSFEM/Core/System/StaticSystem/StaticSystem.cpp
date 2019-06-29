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
	for (auto pfield : this->pfields) {
		//----------場の初期化----------
		pfield->Initialize();

		//----------Kmapの作成----------
		pfield->MakeKmap();

		//----------要素―節点方程式を計算----------
		for (auto& pequation : pfield->pequations) {
			pequation->SetEquation();
		}

		//----------バンド幅の計算----------
		pfield->GetBandwidth();

		//----------全体―節点方程式を解く----------
		pfield->SolveEquation();
	}
}


void PANSFEM::StaticSystem::Export(std::string _fname){
	std::ofstream fout(_fname + ".vtk");

	//----------Headerの出力----------
	fout << "# vtk DataFile Version 4.1\n";
	fout << "vtk output\n";
	fout << "ASCII\n";
	fout << "DATASET UNSTRUCTURED_GRID\n";

	//----------節点の出力----------
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

	//----------要素の追加----------
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

	//----------要素タイプの設定----------
	//要修正
	fout << "\nCELL_TYPES\t" << this->pelements.size() << "\n";
	for (auto pelement : this->pelements) {
		fout << "9\n";
	}

	//----------節点の値を追加----------
	fout << "\nPOINT_DATA\t" << this->pnodes.size() << "\n";
	for (int i = 0; i < this->pfields.size(); i++) {
		//.....スカラー量.....
		if (this->pfields[i]->uf_to_us.size() == 1) {
			fout << "SCALARS u" << i << " float\n";
			fout << "LOOKUP_TABLE default\n";

			for (auto pnode : this->pnodes) {
				fout << pnode->u[this->pfields[i]->uf_to_us[0]] << std::endl;
			}
		}

		//.....ベクトル量.....
		else {
			fout << "VECTORS u" << i << " float\n";
			if (this->pfields[i]->uf_to_us.size() == 2) {
				for (auto pnode : this->pnodes) {
					fout << pnode->u[this->pfields[i]->uf_to_us[0]] << "\t" << pnode->u[this->pfields[i]->uf_to_us[1]] << "\t" << "0" << std::endl;
				}
			}
			else if(this->pfields[i]->uf_to_us.size() == 3){
				for (auto pnode : this->pnodes) {
					fout << pnode->u[this->pfields[i]->uf_to_us[0]] << "\t" << pnode->u[this->pfields[i]->uf_to_us[1]] << "\t" << pnode->u[this->pfields[i]->uf_to_us[2]] << std::endl;
				}
			}
		}
	}
	
	fout.close();
}