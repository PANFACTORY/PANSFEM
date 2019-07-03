//*****************************************************************************
//Title		:PANSFEM/Core/Integration/Integration.h
//Author	:Tanabe Yuta
//Date		:2019/06/08
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include <vector>
#include <Eigen/dense>


namespace PANSFEM {
	class Integration
	{
	public:
		Integration();
		virtual ~Integration();

		
		virtual Eigen::MatrixXd Integrate(std::function<Eigen::MatrixXd(std::vector<double>)>) = 0;		//行列を返す関数の積分
		virtual Eigen::VectorXd Integrate(std::function<Eigen::VectorXd(std::vector<double>)>) = 0;		//ベクトルを返す関数の積分
		virtual double Integrate(std::function<double(std::vector<double>)>) = 0;						//スカラーを返す関数の積分
	};
}