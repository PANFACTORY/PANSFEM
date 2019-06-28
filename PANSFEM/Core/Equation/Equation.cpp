//*****************************************************************************
//Title		:PANSFEM/Core/Equation/Equation.cpp
//Author	:Tanabe Yuta
//Date		:2019/06/09
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include <iostream>


#include "Equation.h"


PANSFEM::Equation::Equation() : DOX(0), DOU(0), NOP(0){}


PANSFEM::Equation::~Equation(){
	delete this->pintegration;
}


PANSFEM::Equation::Equation(Element * _pelement, std::vector<int> _ulist, std::vector<double> _parameters, Integration* _pintegration, int _DOX, int _DOU, int _NOP) : DOX(_DOX), DOU(_DOU), NOP(_NOP){
	try {
		//----------DOUとNOPの検証----------
		if (_ulist.size() != _DOU || _parameters.size() != _NOP) {
			throw std::exception();
		}

		//----------パラメータ代入----------
		this->pelement = _pelement;
		this->ueq_to_us = _ulist;
		this->pintegration = _pintegration;
	}
	catch (std::exception e) {
		std::cout << "Error in Equation" << std::endl;
	}
}


void PANSFEM::Equation::SetEquation(){
	//----------[Ke]を計算----------
	std::function<Eigen::MatrixXd(std::vector<double>)> ke = [&](std::vector<double> _xi) {return GetKe(_xi); };
	this->Ke = this->pintegration->Integrate(ke);

	//----------{Fe}を計算----------
	std::function<Eigen::VectorXd(std::vector<double>)> fe = [&](std::vector<double> _xi) {return GetFe(_xi); };
	this->Fe = this->pintegration->Integrate(fe);
}