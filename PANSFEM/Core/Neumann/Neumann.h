//*****************************************************************************
//Title		:PANSFEM/Core/Neumann/Neumann.h
//Author	:Tanabe Yuta
//Date		:2019/06/22
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include <vector>


#include "../Node/Node.h"


namespace PANSFEM {
	class Neumann
	{
	public:
		Neumann();
		virtual ~Neumann();
		Neumann(Node* _pnode, std::vector<double> _q);


		Node* pnode;				//�Ή�����ߓ_���w���|�C���^
		std::vector<double> q;		//Neumann���E�����̒l
	};
}