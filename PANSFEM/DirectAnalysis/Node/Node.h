//*****************************************************************************
//Title		:PANSFEM/Core/Node/Node.h
//Author	:Tanabe Yuta
//Date		:2019/06/08
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include <vector>
#include <map>
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
		Eigen::VectorXd q;				//�]���ϐ��̓����t���b�N�X
		std::vector<bool> is_ufixed;	//�]���ϐ����Œ肳��Ă��邩
		std::vector<bool> is_qfixed;	//�]���ϐ��̓����t���b�N�X���Œ肳��Ă��邩
		std::map<int, int> us_to_un;	//�n�]���ϐ��ԍ����ߓ_�]���ϐ��ԍ�
	};
}
