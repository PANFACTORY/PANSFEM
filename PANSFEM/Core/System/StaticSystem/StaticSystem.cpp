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
	if (this->DOU == 1) {
		fout << "SCALARS u float\n";
		fout << "LOOKUP_TABLE default\n";

		for (auto pnode : this->pnodes) {
			fout << pnode->u(0) << std::endl;
		}
	}
	else if (this->DOU == 2) {
		fout << "VECTORS u float\n";

		for (auto pnode : this->pnodes) {
			fout << pnode->u(0) << "\t" << pnode->u(1) << "\t" << "0.0" <<  std::endl;
		}
	}
	else if (this->DOU == 3) {
		fout << "VECTORS u float\n";

		for (auto pnode : this->pnodes) {
			fout << pnode->u(0) << "\t" << pnode->u(1) << "\t" << pnode->u(2) << std::endl;
		}
	}
	
	fout.close();
}