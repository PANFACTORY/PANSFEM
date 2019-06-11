//*****************************************************************************
//Title		:PANSFEM/Core/Element/Element.h
//Author	:Tanabe Yuta
//Date		:2019/06/08
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "Element.h"


PANSFEM::Element::Element() : NON(0), DOU(0){}


PANSFEM::Element::~Element(){
	for (auto& pshapefunction : this->pshapefunctions) {
		delete pshapefunction;
	}
}


PANSFEM::Element::Element(std::vector<Node*> _pnodes, std::vector<int> _ulist) : NON(_pnodes.size()), DOU(_ulist.size()){
	this->pnodes = _pnodes;
	for (int i = 0; i < _ulist.size(); i++) {
		this->us_to_uel[_ulist[i]] = i;
	}
}