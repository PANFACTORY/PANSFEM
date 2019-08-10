//*****************************************************************************
//Title		:PANSFEM/Core/Field/Field.h
//Author	:Tanabe Yuta
//Date		:2019/06/08
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include <vector>
#include <map>
#include <fstream>


#include "../Equation/Equation.h"
#include "../Neumann/Neumann.h"


namespace PANSFEM {
	class Field
	{
	public:
		Field();
		virtual ~Field();
		Field(std::vector<int> _ulist);


		int KDEGREE;		//場の全体―節点方程式の次数
		int BANDWIDTHU;		//上バンド幅
		int BANDWIDTHL;		//下バンド幅


		std::vector<int> uf_to_us;				//場の従属変数番号→系の従属変数番号
		std::map<Node*, std::pair<int, int> > Kmap;		//節点を指すポインタ→全体―節点方程式位置，幅


		void Initialize();						//場に属する要素、節点を指すポインタを集める
		void MakeKmap();						//Kmapの作成
		void GetBandwidth();					//バンド幅を計算
		virtual void SolveEquation() = 0;		//全体―節点方程式を解く（線形連立方程式）
		

		std::vector<Equation*> pequations;		//場を記述する要素―節点方程式を指すポインタ
		std::vector<Node*> pnodes;				//場に属する節点を指すポインタ
		std::vector<Neumann*> pneumanns;		//場を記述するNeumann境界条件
	};
}