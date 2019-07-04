//*****************************************************************************
//Title		:PANSFEM/InversedAnalysis/Function/Function.h
//Author	:Tanabe Yuta
//Date		:2019/06/30
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include <vector>


#include "../../DirectAnalysis/Element/Element.h"
#include "../../DirectAnalysis/Integration/Integration.h"


namespace PANSFEM {
	class Function
	{
	public:
		Function();
		virtual ~Function();
		Function(std::vector<int> _plist, std::vector<int> _refulist, std::vector<int> _refplist, int _NOP, int _DOREFU, int _NOREFP);


		const int NOP;										//一要素当たりの設計変数の数
		const int DOREFU;									//参照する従属変数の数
		const int NOREFP;									//参照するパラメータの数


		std::vector<int> pf_to_ps;							//設計変数の関数内番号→系のパラメータ番号
		std::vector<int> refuf_to_us;						//関数が参照する従属変数の関数内番号→系の従属変数番号
		std::vector<int> refpf_to_us;						//関数が参照するパラメータの関数内番号→系のパラメータ番号
		std::vector<Element*> pelements;					//設計変数の定義された要素
		std::map<Element*, Integration*> pintegrations;		//要素毎の積分法


		virtual Eigen::VectorXd sensitivitis() = 0;			//関数感度ベクトルを返す
		virtual double value() = 0;							//関数値を返す		
	};
}