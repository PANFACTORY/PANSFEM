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


		virtual Eigen::VectorXd N(std::vector<double> _x);		//�`��֐��x�N�g��
		virtual Eigen::MatrixXd dNdx(std::vector<double> _x);	//�`��֐���x�ɂ���K�����s���Ԃ�
	};
}