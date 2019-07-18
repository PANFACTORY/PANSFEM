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
#include "../Integration/Integration.h"


namespace PANSFEM {
	class Equation
	{
	public:
		Equation();
		virtual ~Equation();
		Equation(Element *_pelement, std::vector<int> _ulist, std::vector<int> _refulist, std::vector<int> _plist, Integration* _pintegration, int _DOX, int _DOU, int _DOREFU, int _NOP);


		const int DOX;		//要素―節点方程式を記述する独立変数自由度：継承時に指定
		const int DOU;		//要素―節点方程式を記述する従属変数自由度：継承時に指定
		const int DOREFU;	//要素―節点方程式が参照する従属変数自由度：継承時に指定
		const int NOP;		//要素―節点方程式を記述するパラメータの数：継承時に指定


		Element *pelement;	//要素―節点方程式に対応する要素を指すポインタ
		std::vector<int> ueq_to_us;			//要素―節点方程式従属変数番号→系従属変数番号
		std::vector<int> refueq_to_us;		//参照される要素―節点方程式従属変数番号→系従属変数番号
		std::vector<int> peq_to_ps;			//要素―節点方程式パラメータ番号→系パラメータ番号
		Integration* pintegration;			//要素―節点方程式を生成するときの積分法
				

		Eigen::MatrixXd Ke;	//要素―節点方程式の係数行列
		Eigen::VectorXd Fe;	//要素―節点方程式の係数ベクトル


		virtual void SetEquation() = 0;		//要素―節点方程式を設定
	};
}