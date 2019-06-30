//*****************************************************************************
//Title		:PANSFEM/Core/ShapeFunction/Parametric/Triangle/Triangle.h
//Author	:Tanabe Yuta
//Date		:2019/06/11
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include "../../ShapeFunction.h"


namespace PANSFEM {
	class Triangle :
		public ShapeFunction
	{
	public:
		Triangle();
		virtual ~Triangle();


		Eigen::VectorXd Trial(std::vector<Node*> _pnodes, std::vector<double> _x);		//試行関数の形状関数ベクトル
		Eigen::MatrixXd dTrialdx(std::vector<Node*> _pnodes, std::vector<double> _x);	//試行関数の形状関数のxによる一階微分行列を返す
		Eigen::VectorXd Test(std::vector<Node*> _pnodes, std::vector<double> _x);		//試験関数の形状関数ベクトル
		Eigen::MatrixXd dTestdx(std::vector<Node*> _pnodes, std::vector<double> _x);	//試験関数の形状関数のxによる一階微分行列を返す
	};
}