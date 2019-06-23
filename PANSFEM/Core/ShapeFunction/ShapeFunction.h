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
		

		virtual Eigen::VectorXd Trial(std::vector<Node*> _pnodes, std::vector<double> _x) = 0;		//試行関数の形状関数ベクトル
		virtual Eigen::MatrixXd dTrialdx(std::vector<Node*> _pnodes, std::vector<double> _x) = 0;	//試行関数の形状関数のxによる一階微分行列を返す
		virtual Eigen::VectorXd Test(std::vector<Node*> _pnodes, std::vector<double> _x) = 0;		//試験関数の形状関数ベクトル
		virtual Eigen::MatrixXd dTestdx(std::vector<Node*> _pnodes, std::vector<double> _x) = 0;	//試験関数の形状関数のxによる一階微分行列を返す
	};
}