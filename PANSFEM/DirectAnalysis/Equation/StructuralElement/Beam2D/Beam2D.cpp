//*****************************************************************************
//Title		:PANSFEM/Core/Equation/StructualElement/Beam2D/Beam2D.cpp
//Author	:Tanabe Yuta
//Date		:2019/07/14
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "Beam2D.h"


PANSFEM::Beam2D::Beam2D() : Equation(){}


PANSFEM::Beam2D::~Beam2D(){}


PANSFEM::Beam2D::Beam2D(Element * _pelement, std::vector<int> _ulist, std::vector<int> _refulist, std::vector<int> _plist, Integration * _pintegration) : Equation(_pelement, _ulist, _refulist, _plist, _pintegration, 2, 3, 0, 3){
	try {
		if (_pelement->NON != 2) {
			throw std::exception();
		}
	}
	catch (std::exception e) {
		std::cout << "Error in Beam2D" << std::endl;
	}
}


void PANSFEM::Beam2D::SetEquation(){
	//----------‰ñ“]s—ñ‚Ì¶¬----------
	Eigen::VectorXd dx = this->pelement->pnodes[1]->x - this->pelement->pnodes[0]->x;
	double l = dx.norm(), cos = dx(0) / l, sin = dx(1) / l;
	Eigen::MatrixXd T = Eigen::MatrixXd::Zero(6, 6);
	T(0, 0) = cos;	T(0, 1) = sin;	T(0, 2) = 0.0;	T(0, 3) = 0.0;	T(0, 4) = 0.0;	T(0, 5) = 0.0;
	T(1, 0) = -sin;	T(1, 1) = cos;	T(1, 2) = 0.0;	T(1, 3) = 0.0;	T(1, 4) = 0.0;	T(1, 5) = 0.0;
	T(2, 0) = 0.0;	T(2, 1) = 0.0;	T(2, 2) = 1.0;	T(2, 3) = 0.0;	T(2, 4) = 0.0;	T(2, 5) = 0.0;
	T(3, 0) = 0.0;	T(3, 1) = 0.0;	T(3, 2) = 0.0;  T(3, 3) = cos;	T(3, 4) = sin;	T(3, 5) = 0.0;
	T(4, 0) = 0.0;	T(4, 1) = 0.0;	T(4, 2) = 0.0;  T(4, 3) = -sin;	T(4, 4) = cos;	T(4, 5) = 0.0;
	T(5, 0) = 0.0;	T(5, 1) = 0.0;	T(5, 2) = 0.0;  T(5, 3) = 0.0;	T(5, 4) = 0.0;	T(5, 5) = 1.0;

	//----------—v‘fß“_•û’öŽ®‚ð¶¬----------
	double E = this->pelement->parameters[this->peq_to_ps[0]];
	double I = this->pelement->parameters[this->peq_to_ps[1]];
	double A = this->pelement->parameters[this->peq_to_ps[2]];

	Eigen::MatrixXd Kel = Eigen::MatrixXd::Zero(6, 6);
	Kel(0, 0) = A * E / l;	Kel(0, 1) = 0.0;						Kel(0, 2) = 0.0;					Kel(0, 3) = -A * E / l;		Kel(0, 4) = 0.0;						Kel(0, 5) = 0.0;
	Kel(1, 0) = 0.0;		Kel(1, 1) = 12.0*E*I / pow(l, 3.0);		Kel(1, 2) = 6.0*E*I / pow(l, 2.0);	Kel(1, 3) = 0.0;			Kel(1, 4) = -12.0*E*I / pow(l, 3.0);	Kel(1, 5) = 6.0*E*I / pow(l, 2.0);
	Kel(2, 0) = 0.0;		Kel(2, 1) = 6.0*E*I / pow(l, 2.0);		Kel(2, 2) = 4.0*E*I / l;			Kel(2, 3) = 0.0;			Kel(2, 4) = -6.0*E*I / pow(l, 2.0);		Kel(2, 5) = 2.0*E*I / l;
	Kel(3, 0) = -A * E / l;	Kel(3, 1) = 0.0;						Kel(3, 2) = 0.0;					Kel(3, 3) = A * E / l;		Kel(3, 4) = 0.0;						Kel(3, 5) = 0.0;
	Kel(4, 0) = 0.0;		Kel(4, 1) = -12.0*E*I / pow(l, 3.0);	Kel(4, 2) = -6.0*E*I / pow(l, 2.0);	Kel(4, 3) = 0.0;			Kel(4, 4) = 12.0*E*I / pow(l, 3.0);		Kel(4, 5) = -6.0*E*I / pow(l, 2.0);
	Kel(5, 0) = 0.0;		Kel(5, 1) = 6.0*E*I / pow(l, 2.0);		Kel(5, 2) = 2.0*E*I / l;			Kel(5, 3) = 0.0;			Kel(5, 4) = -6.0*E*I / pow(l, 2.0);		Kel(5, 5) = 4.0*E*I / l;

	this->Ke = T.transpose() * Kel * T;
	this->Fe = Eigen::VectorXd::Zero(6);
}


Eigen::MatrixXd PANSFEM::Beam2D::GetKe(std::vector<double> _xi){
	return Eigen::MatrixXd();
}


Eigen::VectorXd PANSFEM::Beam2D::GetFe(std::vector<double> _xi){
	return Eigen::VectorXd();
}