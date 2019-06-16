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


		Node* pnode;					//Dirichlet�������ۂ��Ă���ߓ_
		std::vector<double> uvalues;	//Dirichlet�����̒l
		std::vector<int> ulist;			//Dirichlet�������ۂ��Ă���u�̔ԍ�
	};
}