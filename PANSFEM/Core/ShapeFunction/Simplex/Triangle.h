//*****************************************************************************
//Title		:PANSFEM/Core/ShapeFunction/Triangle/Triangle.h
//Author	:Tanabe Yuta
//Date		:2019/06/11
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include "../ShapeFunction.h"


namespace PANSFEM {
	class Triangle :
		public ShapeFunction
	{
	public:
		Triangle();
		virtual ~Triangle();
		Triangle(Element* _pelement);


		Eigen::VectorXd Trial(std::vector<double> _x);		//���s�֐��̌`��֐��x�N�g��
		Eigen::MatrixXd dTrialdx(std::vector<double> _x);	//���s�֐��̌`��֐���x�ɂ���K�����s���Ԃ�
		Eigen::VectorXd Test(std::vector<double> _x);		//�����֐��̌`��֐��x�N�g��
		Eigen::MatrixXd dTestdx(std::vector<double> _x);	//�����֐��̌`��֐���x�ɂ���K�����s���Ԃ�
	};
}