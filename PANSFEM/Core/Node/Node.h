//*****************************************************************************
//Title		:PANSFEM/Core/Node/Node.h
//Author	:Tanabe Yuta
//Date		:2019/06/08
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include <vector>
#include <map>
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
		std::vector<bool> is_ufixed;	//従属変数が固定されているか
		std::map<int, int> us_to_un;	//系従属変数番号→節点従属変数番号
	};
}
