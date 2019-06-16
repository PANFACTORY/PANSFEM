///*****************************************************************************
//Title		:PANSFEM/Core/Dirichlet/Dirichlet.h
//Author	:Tanabe Yuta
//Date		:2019/06/15
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include <vector>


#include "../Node/Node.h"


namespace PANSFEM {
	class Dirichlet
	{
	public:
		Dirichlet();
		virtual ~Dirichlet();
		Dirichlet(Node*, std::vector<double> _uvalue, std::vector<int> _ulist);


		Node* pnode;					//Dirichlet条件を課している節点
		std::vector<double> uvalues;	//Dirichlet条件の値
		std::vector<int> ulist;			//Dirichlet条件を課しているuの番号
	};
}