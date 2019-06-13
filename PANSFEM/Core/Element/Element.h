//*****************************************************************************
//Title		:PANSFEM/Core/Element/Element.h
//Author	:Tanabe Yuta
//Date		:2019/06/08
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include <vector>
#include <map>
#include <iostream>


#include "../Node/Node.h" 
#include "../ShapeFunction/ShapeFunction.h"


namespace PANSFEM {
	class ShapeFunction;

	class Element
	{
	public:
		Element();
		virtual ~Element();
		Element(std::vector<Node*> _pnodes, std::vector<int> _ulist);


		const int DOU;									//要素内で定義された従属変数の次元
		const int NON;									//要素を構成する節点数（＝形状関数の数）


		std::vector<Node*> pnodes;						//要素を構成する節点を指すポインタ
		std::map<int, int> us_to_uel;					//系従属変数番号→要素従属変数番号
				
		
		template<class N0, class N1, class ...Ns>
		void SetShapeFunction();						//形状関数を指定
		template<class N0>
		void SetShapeFunction();


		Eigen::MatrixXd Trial(std::vector<int> _ulist, std::vector<int> _x);		//試行関数行列を返す
		Eigen::MatrixXd dTrialdx(std::vector<int> _ulist, std::vector<int> _x);		//試行関数のxによる一階微分行列を返す
		Eigen::MatrixXd Test(std::vector<int> _ulist, std::vector<int> _x);			//試験関数行列を返す
		Eigen::MatrixXd dTestdx(std::vector<int> _ulist, std::vector<int> _x);		//試験関数のxによる一階微分行列を返す
		

	protected:
		std::vector<ShapeFunction*> pshapefunctions;	//形状関数の定義
	};

	 
	template<class N0, class N1, class ...Ns>
	inline void Element::SetShapeFunction()	{
		this->pshapefunctions.push_back(new N0(this));
		this->SetShapeFunction<N1, Ns...>();
	}


	template<class N0>
	inline void Element::SetShapeFunction(){
		this->pshapefunctions.push_back(new N0(this));
		//----------形状関数の数の検証----------
		try {
			if (this->DOU != this->pshapefunctions.size()) {
				throw std::exception();
			}
		}
		catch (std::exception e) {
			std::cout << "Error in Element at SetShapeFunction" << std::endl;
		}
	}
}