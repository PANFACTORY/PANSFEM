//*****************************************************************************
//Title		:PANSFEM/Core/ShapeFunction/Parametric/Quadrangle/Quadrangle.h
//Author	:Tanabe Yuta
//Date		:2019/06/11
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include "../../ShapeFunction.h"


namespace PANSFEM {
	class Quadrangle :
		public ShapeFunction
	{
	public:
		Quadrangle();
		virtual ~Quadrangle();
		

		Eigen::VectorXd Trial(std::vector<Node*> _pnodes, std::vector<double> _x);		//���s�֐��̌`��֐��x�N�g��
		Eigen::MatrixXd dTrialdx(std::vector<Node*> _pnodes, std::vector<double> _x);	//���s�֐��̌`��֐���x�ɂ���K�����s���Ԃ�
		Eigen::VectorXd Test(std::vector<Node*> _pnodes, std::vector<double> _x);		//�����֐��̌`��֐��x�N�g��
		Eigen::MatrixXd dTestdx(std::vector<Node*> _pnodes, std::vector<double> _x);	//�����֐��̌`��֐���x�ɂ���K�����s���Ԃ�
	};
}