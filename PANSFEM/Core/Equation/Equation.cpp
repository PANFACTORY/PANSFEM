//*****************************************************************************
//Title		:PANSFEM/Core/Equation/Equation.cpp
//Author	:Tanabe Yuta
//Date		:2019/06/09
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include <iostream>


#include "Equation.h"


PANSFEM::Equation::Equation() : DOU(0), NOP(0){
	this->KEDEGREE = 0;
}


PANSFEM::Equation::~Equation(){}


PANSFEM::Equation::Equation(Element * _pelement, std::vector<int> _ulist, std::vector<double> _parameters, int _DOU, int _NOP) : DOU(_DOU), NOP(_NOP){
	try {
		//----------DOUとNOPの検証----------
		if (_ulist.size() != _DOU || _parameters.size() != _NOP) {
			throw std::exception();
		}

		//----------パラメータ代入----------
		this->pelement = _pelement;
		this->ueq_to_us = _ulist;

		//----------要素―節点方程式の次数を計算----------
		this->KEDEGREE = 0;
		for (auto pnode : this->pelement->pnodes) {
			this->Kemap[pnode] = this->KEDEGREE;

			for (auto uf : this->ueq_to_us) {
				if (std::find(pnode->ulist.begin(), pnode->ulist.end(), uf) != pnode->ulist.end()) {
					this->KEDEGREE++;
				}
			}
		}
	}
	catch (std::exception e) {
		std::cout << "Error in Equation" << std::endl;
	}
}


Eigen::MatrixXd PANSFEM::Equation::Trial(std::vector<double> _x){
	Eigen::MatrixXd trial = Eigen::MatrixXd::Zero(this->DOU, this->KEDEGREE);
	std::vector<int> npos = std::vector<int>(this->pelement->NON, 0);

	for (int i = 0; i < this->DOU; i++) {
		int us = this->ueq_to_us[i];
		int uel = this->pelement->us_to_uel[us];
		Eigen::VectorXd N = this->pelement->pshapefunctions[uel]->Trial(_x);

		int ni = 0;
		for (int j = 0; j < this->pelement->NON; j++) {
			Node* pnode = this->pelement->pnodes[j];
			if (std::find(pnode->ulist.begin(), pnode->ulist.end(), us) != pnode->ulist.end()) {
				trial(i, this->Kemap[pnode] + npos[j]) = N(ni);
				ni++;
				npos[j]++;
			}
		}
	}
	
	return trial;
}


Eigen::MatrixXd PANSFEM::Equation::dTrialdx(std::vector<double> _x){
	int DOX = 2;

	Eigen::MatrixXd dtrialdx = Eigen::MatrixXd::Zero(this->DOU * DOX, this->KEDEGREE);
	std::vector<int> npos = std::vector<int>(this->pelement->NON, 0);

	for (int i = 0; i < this->DOU; i++) {
		int us = this->ueq_to_us[i];
		int uel = this->pelement->us_to_uel[us];
		Eigen::MatrixXd N = this->pelement->pshapefunctions[uel]->dTrialdx(_x);

		int ni = 0;
		for (int j = 0; j < this->pelement->NON; j++) {
			Node* pnode = this->pelement->pnodes[j];
			if (std::find(pnode->ulist.begin(), pnode->ulist.end(), us) != pnode->ulist.end()) {
				

				for (int k = 0; k < DOX; k++) {
					int posi = k * DOX + i;
					int posj = this->Kemap[pnode] + npos[j];
					dtrialdx(posi, posj) = N(k, ni);
				}

				ni++;
				npos[j]++;
			}
		}
	}

	return dtrialdx;
}


Eigen::MatrixXd PANSFEM::Equation::Test(std::vector<double> _x){
	Eigen::MatrixXd test = Eigen::MatrixXd::Zero(this->DOU, this->KEDEGREE);
	std::vector<int> npos = std::vector<int>(this->pelement->NON, 0);

	for (int i = 0; i < this->DOU; i++) {
		int us = this->ueq_to_us[i];
		int uel = this->pelement->us_to_uel[us];
		Eigen::VectorXd N = this->pelement->pshapefunctions[uel]->Test(_x);

		int ni = 0;
		for (int j = 0; j < this->pelement->NON; j++) {
			Node* pnode = this->pelement->pnodes[j];
			if (std::find(pnode->ulist.begin(), pnode->ulist.end(), us) != pnode->ulist.end()) {
				test(i, this->Kemap[pnode] + npos[j]) = N(ni);
				ni++;
				npos[j]++;
			}
		}
	}

	return test;
}


Eigen::MatrixXd PANSFEM::Equation::dTestdx(std::vector<double> _x){
	int DOX = 2;

	Eigen::MatrixXd dtestdx = Eigen::MatrixXd::Zero(this->DOU * DOX, this->KEDEGREE);
	std::vector<int> npos = std::vector<int>(this->pelement->NON, 0);

	for (int i = 0; i < this->DOU; i++) {
		int us = this->ueq_to_us[i];
		int uel = this->pelement->us_to_uel[us];
		Eigen::MatrixXd N = this->pelement->pshapefunctions[uel]->dTestdx(_x);

		int ni = 0;
		for (int j = 0; j < this->pelement->NON; j++) {
			Node* pnode = this->pelement->pnodes[j];
			if (std::find(pnode->ulist.begin(), pnode->ulist.end(), us) != pnode->ulist.end()) {


				for (int k = 0; k < DOX; k++) {
					int posi = k * DOX + i;
					int posj = this->Kemap[pnode] + npos[j];
					dtestdx(posi, posj) = N(k, ni);
				}

				ni++;
				npos[j]++;
			}
		}
	}

	return dtestdx;
}