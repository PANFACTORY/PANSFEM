//*****************************************************************************
//Title		:PANSFEM/Core/Equation/Equation.h
//Author	:Tanabe Yuta
//Date		:2019/06/09
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include <vector>
#include <map>


#include "../Element/Element.h"


namespace PANSFEM {
	class Equation
	{
	public:
		Equation();
		virtual ~Equation();
		Equation(Element *_pelement, std::vector<int> _ulist, std::vector<double> _parameters, int _DOU, int _NOP);


		const int DOU;		//要素―節点方程式を記述する従属変数自由度：継承時に指定
		const int NOP;		//要素―節点方程式を記述するパラメータの数：継承時に指定
		int KEDEGREE;		//要素―節点方程式の次数


		Element *pelement;	//要素―節点方程式に対応する要素を指すポインタ
		std::vector<int> ueq_to_us;			//要素―節点方程式従属変数番号→系従属変数番号
		std::map<Node*, int> Kemap;			//節点を指すポインタ→要素―節点方程式従属変数番号
				

		Eigen::MatrixXd Ke;	//要素―節点方程式の係数行列
		Eigen::VectorXd Fe;	//要素―節点方程式の係数ベクトル


		Eigen::MatrixXd Trial(std::vector<double> _x);		//試行関数ベクトルを返す
		Eigen::MatrixXd dTrialdx(std::vector<double> _x);	//試行関数のxによる一階微分行列を返す
		Eigen::MatrixXd Test(std::vector<double> _x);		//試験関数ベクトルを返す
		Eigen::MatrixXd dTestdx(std::vector<double> _x);	//試験関数のxによる一階微分行列を返す


		virtual void SetEquation() = 0;		//要素―節点方程式を設定
	};
}