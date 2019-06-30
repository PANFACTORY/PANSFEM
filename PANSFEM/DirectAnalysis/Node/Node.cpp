//*****************************************************************************
//Title		:PANSFEM/Core/Node/Node.cpp
//Author	:Tanabe Yuta
//Date		:2019/06/08
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include <iostream>


#include "Node.h"


PANSFEM::Node::Node() : DOU(0){}


PANSFEM::Node::~Node(){}


PANSFEM::Node::Node(std::vector<double> _x, std::vector<int> _ulist) : DOU(_ulist.size()){
	this->x = Eigen::Map<Eigen::VectorXd>(&_x[0], _x.size());
	this->u = Eigen::VectorXd::Zero(_ulist.size());
	this->is_ufixed = std::vector<bool>(this->DOU, false);
	for (int i = 0; i < this->DOU; i++) {
		this->us_to_un[_ulist[i]] = i;
	}
}