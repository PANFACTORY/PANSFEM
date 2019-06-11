//*****************************************************************************
//Title		:PANSFEM/Core/Field/Field.h
//Author	:Tanabe Yuta
//Date		:2019/06/08
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include <vector>
#include <map>


#include "../Equation/Equation.h"


namespace PANSFEM {
	class Field
	{
	public:
		Field();
		virtual ~Field();
		Field(std::vector<int> _ulist, std::vector<Equation*> _pequations);


		int KDEGREE;		//場の全体―節点方程式の次数
		int BANDWIDTHU;		//上バンド幅
		int BANDWIDTHL;		//下バンド幅


		std::vector<int> uf_to_us;				//場の従属変数番号→系の従属変数番号
		std::map<Node*, std::pair<int, int> > Kmap;		//節点を指すポインタ→全体―節点方程式位置，幅


		void MakeKmap();						//Kmapの作成
		void GetBandwidth();					//バンド幅を計算
		void SolveEquation();					//全体―節点方程式を解く（連立方程式）
		//void SolveEigen();						//全体―節点方程式を解く（固有値問題）


	protected:
		std::vector<Equation*> pequations;		//場を記述する要素―節点方程式を指すポインタ
		std::vector<Node*> pnodes;				//場に属する節点を指すポインタ
	};
}