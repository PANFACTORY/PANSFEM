//*****************************************************************************
//Title		:PANSFEM/Core/Neumann/Neumann.h
//Author	:Tanabe Yuta
//Date		:2019/06/22
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include <iostream>


#include "Neumann.h"


PANSFEM::Neumann::Neumann(){}


PANSFEM::Neumann::~Neumann(){}


PANSFEM::Neumann::Neumann(Node * _pnode, std::vector<double> _q, std::vector<int> _qlist){
	try {
		this->pnode = _pnode;

		if (_q.size() != _qlist.size()) {
			throw std::exception();
		}

		for (int i = 0; i < _qlist.size(); i++) {
			this->q[_qlist[i]] = _q[i];
		}
	}
	catch (std::exception e) {
		std::cout << "Error in Neumann condition setting" << std::endl;
	}
}