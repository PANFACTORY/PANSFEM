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

	//.....��s�ǂݔ�΂�.....
	std::string str0;
	std::getline(ifs, str0);

	while (!ifs.eof()) {
		//.....��s���ǂݍ���.....
		std::string buf;
		ifs >> buf;
		std::istringstream sbuf(buf);
		std::string str;

		//.....�ߓ_ID��ǂݔ�΂�.....
		std::getline(sbuf, str, ',');

		//.....�ߓ_�̓Ɨ��ϐ��̒l��ǂݍ���.....
		std::vector<double> xs(this->DOX);
		for (auto& x : xs) {
			std::getline(sbuf, str, ',');
			x = stod(str);
		}

		//.....�ߓ_�̏]���ϐ��̑Ή��ԍ���ǂݍ���.....
		std::vector<int> ulist;
		while (std::getline(sbuf, str, ',')) {
			ulist.push_back(stoi(str));
		}

		//.....�ߓ_��ǉ�.....
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

	//.....��s�ǂݔ�΂�.....
	std::string str0;
	std::getline(ifs, str0);

	while (!ifs.eof()) {
		//.....��s���ǂݍ���.....
		std::string buf;
		ifs >> buf;
		std::istringstream sbuf(buf);
		std::string str;

		//.....�p�����[�^ID��ǂݔ�΂�.....
		std::getline(sbuf, str, ',');

		//.....�Ή�����v�f���w���|�C���^���擾.....
		std::getline(sbuf, str, ',');
		Element* tmppelement = this->pelements[stoi(str)];

		//.....�p�����[�^�̒l��ǂݍ���.....
		std::vector<double> ps;
		while (std::getline(sbuf, str, ',')) {
			ps.push_back(stod(str));
		}

		//.....�v�f�Ƀp�����[�^����.....
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

	//.....��s�ǂݔ�΂�.....
	std::string str0;
	std::getline(ifs, str0);

	while (!ifs.eof()) {
		//.....��s���ǂݍ���.....
		std::string buf;
		ifs >> buf;
		std::istringstream sbuf(buf);
		std::string str;

		//.....���E����ID��ǂݔ�΂�.....
		std::getline(sbuf, str, ',');

		//.....�Ή�����ߓ_���w���|�C���^���擾.....
		std::getline(sbuf, str, ',');
		Node* pnode = this->pnodes[stoi(str)];

		//.....�ߓ_�̏]���ϐ��̒l��ǂݍ���.....
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
	//----------Neumann���E������ǂݍ���----------
	std::ifstream ifs(_fname);

	if (!ifs.is_open()) {
		std::cout << "Neumann Condition file " << _fname << " open error!" << std::endl;
		return false;
	}

	//.....��s�ǂݔ�΂�.....
	std::string str0;
	std::getline(ifs, str0);

	while (!ifs.eof()) {
		//.....��s���ǂݍ���.....
		std::string buf;
		ifs >> buf;
		std::istringstream sbuf(buf);
		std::string str;

		//.....���E����ID��ǂݔ�΂�.....
		std::getline(sbuf, str, ',');

		//.....�Ή�����ߓ_���w���|�C���^���擾.....
		std::getline(sbuf, str, ',');
		Node* pnode = this->pnodes[stoi(str)];

		//.....Neumann���E�l���߂�.....
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

		//.....�ߓ_��Neumann���E�����l��ǂݍ���.....
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