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

		//.....節点自由度を取得.....
		std::getline(sbuf, str, ',');
		int DOX = stoi(str);

		//.....節点の独立変数の値を読み込む.....
		std::vector<double> xs(DOX);
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
		std::vector<bool> is_fixed = std::vector<bool>(this->DOU, false);
		std::vector<double> fixedvalue = std::vector<double>(this->DOU, false);
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


bool PANSFEM::System::ImportNeumann(std::string _fname){
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
		while (std::getline(sbuf, str, ',')) {
			q.push_back(stod(str));
		}

		//.....節点のNeumann境界条件値を読み込む.....
		this->pneumanns.push_back(new Neumann(pnode, q));
	}

	ifs.close();

	//----------各場に対応するNeumann境界条件インスタンスのリストを渡す----------
	for (auto& pfield : this->pfields) {
		for (auto pneumann : this->pneumanns) {
			if (std::find(pfield->pnodes.begin(), pfield->pnodes.end(), pneumann->pnode) != pfield->pnodes.end()) {
				pfield->pneumanns.push_back(pneumann);
			}
		}
	}

	return true;
}


void PANSFEM::System::Show(){
	for (auto pnode : this->pnodes) {
		std::cout << pnode->x.transpose() << "\t" << pnode->u.transpose() << std::endl;
	}
}