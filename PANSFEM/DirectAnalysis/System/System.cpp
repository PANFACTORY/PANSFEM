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


void PANSFEM::System::Export(std::string _fname) {
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
	fout << "\nCELL_TYPES\t" << this->pelements.size() << "\n";
	for (auto pelement : this->pelements) {
		fout << pelement->PARAVIEWTYPE << "\n";
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