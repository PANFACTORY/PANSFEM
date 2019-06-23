//*****************************************************************************
//Title		:PANSFEM/Core/ShapeFunction/ShapeFunction.h
//Author	:Tanabe Yuta
//Date		:2019/06/08
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include <vector>
#include <Eigen/dense>


#include "../Node/Node.h"


namespace PANSFEM {
	class ShapeFunction
	{
	public:
		ShapeFunction();
		virtual ~ShapeFunction();
		

		virtual Eigen::VectorXd Trial(std::vector<Node*> _pnodes, std::vector<double> _x) = 0;		//���s�֐��̌`��֐��x�N�g��
		virtual Eigen::MatrixXd dTrialdx(std::vector<Node*> _pnodes, std::vector<double> _x) = 0;	//���s�֐��̌`��֐���x�ɂ���K�����s���Ԃ�
		virtual Eigen::VectorXd Test(std::vector<Node*> _pnodes, std::vector<double> _x) = 0;		//�����֐��̌`��֐��x�N�g��
		virtual Eigen::MatrixXd dTestdx(std::vector<Node*> _pnodes, std::vector<double> _x) = 0;	//�����֐��̌`��֐���x�ɂ���K�����s���Ԃ�
	};
}