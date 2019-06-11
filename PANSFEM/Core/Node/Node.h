//*****************************************************************************
//Title		:PANSFEM/Core/Node/Node.h
//Author	:Tanabe Yuta
//Date		:2019/06/08
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include <vector>
#include <Eigen/dense>


namespace PANSFEM {
	class Node
	{
	public:
		Node();
		virtual ~Node();
		Node(std::vector<double> _x, std::vector<int> _ulist);


		const int DOU;					//節点で定義された従属変数の次元


		Eigen::VectorXd x;				//独立変数
		Eigen::VectorXd u;				//従属変数
		std::vector<int> ulist;			//節点―全体従属変数対応関係
	};
}
