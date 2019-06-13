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
#include "../Element/Element.h"


namespace PANSFEM {
	class Element;

	class ShapeFunction
	{
	public:
		ShapeFunction();
		virtual ~ShapeFunction();
		ShapeFunction(Element* _pelement);


		Element* pparent;		//�e�v�f���w���|�C���^


		virtual Eigen::VectorXd Trial(std::vector<double> _x) = 0;		//���s�֐��̌`��֐��x�N�g��
		virtual Eigen::MatrixXd dTrialdx(std::vector<double> _x) = 0;	//���s�֐��̌`��֐���x�ɂ���K�����s���Ԃ�
		virtual Eigen::VectorXd Test(std::vector<double> _x) = 0;		//�����֐��̌`��֐��x�N�g��
		virtual Eigen::MatrixXd dTestdx(std::vector<double> _x) = 0;	//�����֐��̌`��֐���x�ɂ���K�����s���Ԃ�
	};
}