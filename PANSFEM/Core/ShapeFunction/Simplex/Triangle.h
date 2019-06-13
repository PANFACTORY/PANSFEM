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


		Eigen::VectorXd Trial(std::vector<double> _x);		//試行関数の形状関数ベクトル
		Eigen::MatrixXd dTrialdx(std::vector<double> _x);	//試行関数の形状関数のxによる一階微分行列を返す
		Eigen::VectorXd Test(std::vector<double> _x);		//試験関数の形状関数ベクトル
		Eigen::MatrixXd dTestdx(std::vector<double> _x);	//試験関数の形状関数のxによる一階微分行列を返す
	};
}