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


		virtual Eigen::VectorXd N(std::vector<double> _x);		//形状関数ベクトル
		virtual Eigen::MatrixXd dNdx(std::vector<double> _x);	//形状関数のxによる一階微分行列を返す
	};
}