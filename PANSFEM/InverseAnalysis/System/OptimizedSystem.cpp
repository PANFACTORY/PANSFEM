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
		
		//----------�ړI�֐��̌v�Z----------
		double currentvalue = this->pobjectives[0]->value();
		std::cout << "\t" << "Objective function value:" << currentvalue << std::endl;

		//----------��������----------
		if (fabs(currentvalue - previousvalue) < 1.0e-5) {
			//std::cout << "----------System is optimized----------" << std::endl;
			//break;
		}
		
		//----------�݌v���x���v�Z----------
		Eigen::VectorXd objectivesensitivity = this->pobjectives[0]->sensitivitis();
		Eigen::VectorXd constraintsensitivity = this->pconstraints[0]->sensitivitis();
				
		//----------Lagrange�搔��񕪒T��----------
		double lambda0 = 0.0, lambda1 = 1.0e4;
		while ((lambda1 - lambda0)/(lambda1 + lambda0) > 1.0e-3) {
			double lambda = 0.5 * (lambda1 + lambda0);
			Eigen::VectorXd B = pow((-objectivesensitivity.array() / constraintsensitivity.array()).array() / lambda, 0.75);
			
			
		}

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
	fout << "\nPOINT_DATA\t" << this->pnodes.size() << "\n";
	fout << "SCALARS p" << " float\n";
	fout << "LOOKUP_TABLE default\n";

	for (auto pelement : this->pelements) {
		fout << pelement->parameters[this->plist[0]] << std::endl;
	}

	fout.close();
}