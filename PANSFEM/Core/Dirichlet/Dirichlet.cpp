///*****************************************************************************
//Title		:PANSFEM/Core/Dirichlet/Dirichlet.cpp
//Author	:Tanabe Yuta
//Date		:2019/06/15
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include <iostream>


#include "Dirichlet.h"


PANSFEM::Dirichlet::Dirichlet(){}


PANSFEM::Dirichlet::~Dirichlet(){}


PANSFEM::Dirichlet::Dirichlet(Node* _pnode, std::vector<double> _uvalues, std::vector<int> _ulist){
	try {
		if (_uvalues.size() != _ulist.size()) {
			throw std::exception();
		}

		this->pnode = _pnode;
		this->uvalues = _uvalues;
		this->ulist = _ulist;
	}
	catch (std::exception e) {
		std::cout << "Error in Dirichlet condition";
	}
}
