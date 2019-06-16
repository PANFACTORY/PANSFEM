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
		Equation(Element *_pelement, std::vector<int> _ulist, std::vector<double> _parameters, int _DOX, int _DOU, int _NOP);


		const int DOX;		//要素―節点方程式を記述する独立変数自由度：継承時に指定
		const int DOU;		//要素―節点方程式を記述する従属変数自由度：継承時に指定
		const int NOP;		//要素―節点方程式を記述するパラメータの数：継承時に指定


		Element *pelement;	//要素―節点方程式に対応する要素を指すポインタ
		std::vector<int> ueq_to_us;			//要素―節点方程式従属変数番号→系従属変数番号
				

		Eigen::MatrixXd Ke;	//要素―節点方程式の係数行列
		Eigen::VectorXd Fe;	//要素―節点方程式の係数ベクトル


		virtual void SetEquation() = 0;		//要素―節点方程式を設定
	};
}