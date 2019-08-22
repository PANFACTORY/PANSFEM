//*****************************************************************************
//Title		:PANSFEM/InversedAnalysis/System/OCSystem/OCSystem.cpp
//Author	:Tanabe Yuta
//Date		:2019/08/10
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "OCSystem.h"


PANSFEM::OCSystem::OCSystem() : OptimizedSystem(){}


PANSFEM::OCSystem::~OCSystem(){}


PANSFEM::OCSystem::OCSystem(int _DOX, int _DOU, std::vector<int> _plist) : OptimizedSystem(_DOX, _DOU, _plist){}


void PANSFEM::OCSystem::Schedule(){
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
	const double valueconvergence = 1.0e-5;		//�ړI�֐��̎�������l
	const double lambdaconvergence = 1.0e-9;	//Lagrange�搔�ɂ̎�������l
	const double mvlmt = 0.15;					//���[�u���~�b�g
	const double iota = 0.75;					//�_���s���O�W��

	//----------�œK���̔����v�Z----------
	double previousvalue = 0.0;		//1�X�e�b�v�O�̖ړI�֐��̒l
	for (int itr = 0; itr < iterationmax; itr++) {
		std::cout << "Iteration:" << itr << std::endl;

		//----------�����----------
		std::cout << "\t" << "Solution";
		for (auto pfield : this->pfields) {
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
		double lambda0 = 1.0e-10, lambda1 = 1.0e10;
		while ((lambda1 - lambda0) / (lambda1 + lambda0) > lambdaconvergence) {
			double lambda = 0.5 * (lambda1 + lambda0);
			Eigen::VectorXd B = pow((-objectivesensitivity.array() / constraintsensitivity.array()).array() / lambda, iota).array()*rho.array();
			Eigen::VectorXd rhonew = (((B.array().min(rho.array() + mvlmt)).array().min(Eigen::VectorXd::Ones(rholen).array())).array().max(rho.array() - mvlmt)).array().max(Eigen::VectorXd::Constant(rholen, 1.0e-10).array());
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


/*void PANSFEM::OptimizedSystem::Schedule() {
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
	const int iterationmax = 50;				//�œK�����[�v�̍ő唽����
	const double valueconvergence = 1.0e-5;		//�ړI�֐��̎�������l
	const double lambdaconvergence = 1.0e-9;	//Lagrange�搔�ɂ̎�������l
	const double mvlmt = 0.15;					//���[�u���~�b�g
	const double iota = 0.75;					//�_���s���O�W��

	const double beta = 50.0;					//Heaviside Filter�̃p�����[�^
	const double R = 3.6;						//Heaviside Filter�̃t�B���^���a

	std::map<Element*, std::vector<Element*>> relation;
	for (auto pelementi : this->poptimizedelements) {
		relation[pelementi] = std::vector<Element*>(0);

		//�v�fi�̏d�S���v�Z
		Eigen::VectorXd centeri = Eigen::VectorXd::Zero(this->DOX);
		for (auto pnode : pelementi->pnodes) {
			centeri += pnode->x;
		}
		centeri /= (double)pelementi->pnodes.size();

		for (auto pelementj : this->poptimizedelements) {
			//�v�fj�̏d�S���v�Z
			Eigen::VectorXd centerj = Eigen::VectorXd::Zero(this->DOX);
			for (auto pnode : pelementj->pnodes) {
				centerj += pnode->x;
			}
			centerj /= (double)pelementj->pnodes.size();

			double rij = (centerj - centeri).norm();
			if (rij < R) {
				relation[pelementi].push_back(pelementj);
			}
		}
	}

	std::map<Element*, Eigen::VectorXd> center;
	for (auto pelement : this->poptimizedelements) {
		center[pelement] = Eigen::VectorXd::Zero(this->DOX);
		for (auto pnode : pelement->pnodes) {
			center[pelement] += pnode->x;
		}
		center[pelement] /= (double)pelement->pnodes.size();
	}

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
		if (fabs((currentvalue - previousvalue) / (currentvalue + previousvalue)) < valueconvergence && constraintvalue <= 0.0) {
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
		double lambda0 = 1.0e-10, lambda1 = 1.0e10;
		while ((lambda1 - lambda0) / (lambda1 + lambda0) > lambdaconvergence) {
			double lambda = 0.5 * (lambda1 + lambda0);
			Eigen::VectorXd B = pow((-objectivesensitivity.array() / constraintsensitivity.array()).array() / lambda, iota).array()*rho.array();
			Eigen::VectorXd rhonew = (((B.array().min(rho.array() + mvlmt)).array().min(Eigen::VectorXd::Ones(rholen).array())).array().max(rho.array() - mvlmt)).array().max(Eigen::VectorXd::Constant(rholen, 1.0e-10).array());

			//----------Heaviside Filter----------
			Eigen::VectorXd rhof = Eigen::VectorXd::Zero(rholen);
			int pos3 = 0;
			for (auto pelementi : this->poptimizedelements) {
				std::vector<double> sumw = std::vector<double>(this->plist.size(), 0.0);
				std::vector<double> sumws = std::vector<double>(this->plist.size(), 0.0);

				for (auto pelementj : relation[pelementi]) {
					double w = R - (center[pelementj] - center[pelementi]).norm();
					auto iter = std::find(this->poptimizedelements.begin(), this->poptimizedelements.end(), pelementj);
					int index = std::distance(this->poptimizedelements.begin(), iter);

					for (int k = 0; k < this->plist.size(); k++) {
						sumw[k] += w;
						sumws[k] += w*rhonew[this->plist.size()*index+k];
					}
				}

				for (int k = 0; k < this->plist.size(); k++) {
					rhof[pos3] = sumws[k] / sumw[k];
					pos3++;
				}
			}
			//Eigen::VectorXd rhomod = 1.0 - (-rhof.array()*beta).exp() + rhof.array()*exp(-beta);
			Eigen::VectorXd rhomod = 1.0 / (1.0 + (-(rhof.array() - 0.5)*beta).exp());

			int pos2 = 0;
			for (auto pelement : this->poptimizedelements) {
				for (auto pi : this->plist) {
					pelement->parameters[pi] = rhomod(pos2);
					pos2++;
				}
			}
			if (this->pconstraints[0]->value() > 0.0) {
				lambda0 = lambda;
			}
			else {
				lambda1 = lambda;
			}
			pos2 = 0;
			for (auto pelement : this->poptimizedelements) {
				for (auto pi : this->plist) {
					pelement->parameters[pi] = rhonew(pos2);
					pos2++;
				}
			}
		}
		std::cout << "\t" << "Lagrange value�F" << lambda0 << std::endl;

		//----------�ړI�֐��̒l���X�V----------
		previousvalue = currentvalue;
	}
}*/