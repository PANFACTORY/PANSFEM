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

	//.....��s�ǂݔ�΂�.....
	std::string str0;
	std::getline(ifs, str0);

	while (!ifs.eof()) {
		//.....��s���ǂݍ���.....
		std::string buf;
		ifs >> buf;
		std::istringstream sbuf(buf);
		std::string str;

		//.....ID��ǂݔ�΂�.....
		std::getline(sbuf, str, ',');

		//.....�Ή�����v�f���w���|�C���^���œK���Ώۗv�f�̃��X�g�ɒǉ�.....
		std::getline(sbuf, str, ',');
		this->poptimizedelements.push_back(this->pelements[stoi(str)]);
	}

	ifs.close();

	return true;
}


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
	const int rholen = this->poptimizedelements.size() * this->plist.size();	//�݌v�ϐ��x�N�g���̗v�f��
	const int iterationmax = 100;				//�œK�����[�v�̍ő唽����
	const double valueconvergence = 1.0e-6;		//�ړI�֐��̎�������l
	const double lambdaconvergence = 1.0e-3;	//Lagrange�搔�ɂ̎�������l
	const double mvlmt = 0.15;					//���[�u���~�b�g
	const double iota = 0.75;					//�_���s���O�W��

	//----------�œK���̔����v�Z----------
	double previousvalue = 0.0;		//1�X�e�b�v�O�̖ړI�֐��̒l
	for (int itr = 0; itr < iterationmax; itr++) {
		std::cout << "Iteration:" << itr << std::endl;

		//----------�����----------
		std::cout << "\t" << "Solution";
		for (auto pfield : this->pfields) {
			//.....�v�f�\�ߓ_���������v�Z.....
			for (auto& pequation : pfield->pequations) {
				pequation->SetEquation();
			}

			//.....�S�́\�ߓ_������������.....
			pfield->SolveEquation();
		}
		std::cout << " is done." << std::endl;
		this->Export("Data/Output/a");


		//----------�ړI�֐��C��������̌v�Z----------
		double currentvalue = this->pobjectives[0]->value();
		std::cout << "\t" << "Objective function value:" << currentvalue << std::endl;
		double constraintvalue = this->pconstraints[0]->value();
		std::cout << "\t" << "Constraint function value:" << constraintvalue << std::endl;

		//----------��������----------
		if (fabs((currentvalue - previousvalue) / (currentvalue + previousvalue)) < valueconvergence) {
			std::cout << "----------System is optimized----------" << std::endl;
			break;
		}
	
		//----------�݌v���x���v�Z----------
		Eigen::VectorXd objectivesensitivity = this->pobjectives[0]->sensitivitis();
		Eigen::VectorXd constraintsensitivity = this->pconstraints[0]->sensitivitis();

		//----------���݂̐݌v�ϐ��x�N�g���𐶐�----------
		Eigen::VectorXd rho = Eigen::VectorXd(rholen);
		int pos = 0;
		for (auto pelement : this->poptimizedelements) {
			for (auto pi : this->plist) {
				rho(pos) = pelement->parameters[pi];
				pos++;
			}
		}
						
		//----------Lagrange�搔��񕪒T��----------
		double lambda0 = 0.0, lambda1 = 1.0e4;
		while ((lambda1 - lambda0)/(lambda1 + lambda0) > lambdaconvergence) {
			double lambda = 0.5 * (lambda1 + lambda0);
			Eigen::VectorXd B = pow((-objectivesensitivity.array() / constraintsensitivity.array()).array() / lambda, iota).array()*rho.array();
			Eigen::VectorXd rhonew = (((B.array().min(rho.array() * (1.0 + mvlmt))).array().min(Eigen::VectorXd::Ones(rholen).array())).array().max(rho.array() * (1.0 - mvlmt))).array().max(Eigen::VectorXd::Constant(rholen, 1.0e-10).array());
			int pos2 = 0;
			for (auto pelement : this->poptimizedelements) {
				for (auto pi : this->plist) {
					pelement->parameters[pi] = rhonew(pos2);
					pos2++;
				}
			}
			if (this->pconstraints[0]->value() > 0.0) {	
				lambda0 = lambda;
			}
			else {
				lambda1 = lambda;
			}
		}
		std::cout << "\t" << "Lagrange value�F" << lambda0 << std::endl;

		//----------�ړI�֐��̒l���X�V----------
		previousvalue = currentvalue;
	}
}


void PANSFEM::OptimizedSystem::Export(std::string _fname){
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
	fout << "\nCELL_DATA\t" << this->pelements.size() << "\n";
	fout << "SCALARS Rho float\n";
	fout << "LOOKUP_TABLE default\n";

	for (auto pelement : this->pelements) {
		fout << pelement->parameters[this->plist[0]] << std::endl;
	}

	fout.close();
}