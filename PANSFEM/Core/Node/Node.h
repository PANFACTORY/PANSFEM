//*****************************************************************************
//Title		:PANSFEM/Core/Node/Node.h
//Author	:Tanabe Yuta
//Date		:2019/06/08
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include <vector>
#include <Eigen/dense>


namespace PANSFEM {
	class Node
	{
	public:
		Node();
		virtual ~Node();
		Node(std::vector<double> _x, std::vector<int> _ulist);


		const int DOU;					//�ߓ_�Œ�`���ꂽ�]���ϐ��̎���


		Eigen::VectorXd x;				//�Ɨ��ϐ�
		Eigen::VectorXd u;				//�]���ϐ�
		std::vector<int> ulist;			//�ߓ_�\�S�̏]���ϐ��Ή��֌W
	};
}
