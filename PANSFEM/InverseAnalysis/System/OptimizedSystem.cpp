//*****************************************************************************
//Title		:PANSFEM/InversedAnalysis/System/OptimizedSystem.cpp
//Author	:Tanabe Yuta
//Date		:2019/06/30
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "OptimizedSystem.h"


PANSFEM::OptimizedSystem::OptimizedSystem() : System(){}


PANSFEM::OptimizedSystem::~OptimizedSystem(){}


PANSFEM::OptimizedSystem::OptimizedSystem(int _DOX, int _DOU, std::vector<int> _plist) : System(_DOX, _DOU){
	this->plist = _plist;
}


bool PANSFEM::OptimizedSystem::ImportOptimizedElement(std::string _fname){
	std::ifstream ifs(_fname);

	if (!ifs.is_open()) {
		std::cout << "Element file " << _fname << " open error!" << std::endl;
		return false;
	}

	//.....一行読み飛ばす.....
	std::string str0;
	std::getline(ifs, str0);

	while (!ifs.eof()) {
		//.....一行分読み込む.....
		std::string buf;
		ifs >> buf;
		std::istringstream sbuf(buf);
		std::string str;

		//.....IDを読み飛ばす.....
		std::getline(sbuf, str, ',');

		//.....対応する要素を指すポインタを最適化対象要素のリストに追加.....
		std::getline(sbuf, str, ',');
		this->poptimizedelements.push_back(this->pelements[stoi(str)]);
	}

	ifs.close();

	return true;
}


void PANSFEM::OptimizedSystem::Export(std::string _fname){
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
		fout << "25\n";
	}

	//----------設計変数の値を追加----------
	fout << "\nCELL_DATA\t" << this->pelements.size() << "\n";
	for (int i = 0; i < this->plist.size(); i++) {
		fout << "SCALARS Rho" << i << " float\n";
		fout << "LOOKUP_TABLE default\n";

		for (auto pelement : this->pelements) {
			fout << pelement->parameters[this->plist[i]] << std::endl;
		}
		fout << std::endl;
	}	

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
			else if (this->pfields[i]->uf_to_us.size() == 3) {
				for (auto pnode : this->pnodes) {
					fout << pnode->u[this->pfields[i]->uf_to_us[0]] << "\t" << pnode->u[this->pfields[i]->uf_to_us[1]] << "\t" << pnode->u[this->pfields[i]->uf_to_us[2]] << std::endl;
				}
			}
		}
	}

	fout.close();
}