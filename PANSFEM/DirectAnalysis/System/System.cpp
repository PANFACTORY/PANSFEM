//*****************************************************************************
//Title		:PANSFEM/Core/System/System.h
//Author	:Tanabe Yuta
//Date		:2019/06/08
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "System.h"


PANSFEM::System::System() : DOX(0), DOU(0){}


PANSFEM::System::~System(){
	for (auto& pnode : this->pnodes) {
		delete pnode;
	}
	for (auto& pelement : this->pelements) {
		delete pelement;
	}
	for (auto& pfield : this->pfields) {
		delete pfield;
	}
}


PANSFEM::System::System(int _DOX, int _DOU) : DOX(_DOX), DOU(_DOU){}


bool PANSFEM::System::ImportNode(std::string _fname){
	std::ifstream ifs(_fname);

	if (!ifs.is_open()) {
		std::cout << "Node file " << _fname << " open error!" << std::endl;
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

		//.....節点IDを読み飛ばす.....
		std::getline(sbuf, str, ',');

		//.....節点の独立変数の値を読み込む.....
		std::vector<double> xs(this->DOX);
		for (auto& x : xs) {
			std::getline(sbuf, str, ',');
			x = stod(str);
		}

		//.....節点の従属変数の対応番号を読み込む.....
		std::vector<int> ulist;
		while (std::getline(sbuf, str, ',')) {
			ulist.push_back(stoi(str));
		}

		//.....節点を追加.....
		this->pnodes.push_back(new Node(xs, ulist));
	}

	ifs.close();
	
	return true;
}


bool PANSFEM::System::ImportParameter(std::vector<int> _plist, std::string _fname) {
	std::ifstream ifs(_fname);

	if (!ifs.is_open()) {
		std::cout << "Parameter file " << _fname << " open error!" << std::endl;
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

		//.....パラメータIDを読み飛ばす.....
		std::getline(sbuf, str, ',');

		//.....対応する要素を指すポインタを取得.....
		std::getline(sbuf, str, ',');
		Element* tmppelement = this->pelements[stoi(str)];

		//.....パラメータの値を読み込む.....
		std::vector<double> ps;
		while (std::getline(sbuf, str, ',')) {
			ps.push_back(stod(str));
		}

		//.....要素にパラメータを代入.....
		tmppelement->SetParameter(ps, _plist);
	}

	ifs.close();

	return true;
}


bool PANSFEM::System::ImportDirichlet(std::string _fname){
	std::ifstream ifs(_fname);

	if (!ifs.is_open()) {
		std::cout << "Dirichlet Condition file " << _fname << " open error!" << std::endl;
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

		//.....境界条件IDを読み飛ばす.....
		std::getline(sbuf, str, ',');

		//.....対応する節点を指すポインタを取得.....
		std::getline(sbuf, str, ',');
		Node* pnode = this->pnodes[stoi(str)];

		//.....節点の従属変数の値を読み込む.....
		for (int i = 0; i < this->DOU; i++) {
			std::getline(sbuf, str, ',');
			if (str != "free") {
				if (pnode->us_to_un.find(i) != pnode->us_to_un.end()) {
					pnode->is_ufixed[pnode->us_to_un[i]] = true;
					pnode->u[pnode->us_to_un[i]] = stod(str);
				}				
			}
		}
	}

	ifs.close();

	return true;
}


bool PANSFEM::System::ImportNeumann(int _fi, std::string _fname){
	//----------Neumann境界条件を読み込む----------
	std::ifstream ifs(_fname);

	if (!ifs.is_open()) {
		std::cout << "Neumann Condition file " << _fname << " open error!" << std::endl;
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

		//.....境界条件IDを読み飛ばす.....
		std::getline(sbuf, str, ',');

		//.....対応する節点を指すポインタを取得.....
		std::getline(sbuf, str, ',');
		Node* pnode = this->pnodes[stoi(str)];

		//.....Neumann境界値を定める.....
		std::vector<double> q;
		std::vector<int> qlist;
		for (int i = 0; i < this->DOU; i++) {
			std::getline(sbuf, str, ',');
			if (str != "free") {
				if (pnode->us_to_un.find(i) != pnode->us_to_un.end()) {
					qlist.push_back(i);
					q.push_back(stod(str));
				}
			}
		}

		//.....節点のNeumann境界条件値を読み込む.....
		this->pfields[_fi]->pneumanns.push_back(new Neumann(pnode, q, qlist));
	}

	ifs.close();

	return true;
}


void PANSFEM::System::Show(){
	for (auto pnode : this->pnodes) {
		std::cout << pnode->x.transpose() << "\t" << pnode->u.transpose() << std::endl;
	}
}


void PANSFEM::System::Export(std::string _fname) {
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
	fout << "\nCELL_TYPES\t" << this->pelements.size() << "\n";
	for (auto pelement : this->pelements) {
		fout << pelement->PARAVIEWTYPE << "\n";
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
			else if (this->pfields[i]->uf_to_us.size() == 3) {
				for (auto pnode : this->pnodes) {
					fout << pnode->u[this->pfields[i]->uf_to_us[0]] << "\t" << pnode->u[this->pfields[i]->uf_to_us[1]] << "\t" << pnode->u[this->pfields[i]->uf_to_us[2]] << std::endl;
				}
			}
		}
	}

	fout.close();
}