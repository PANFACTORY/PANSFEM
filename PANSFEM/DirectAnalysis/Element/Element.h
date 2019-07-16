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
	class Element
	{
	public:
		Element();
		virtual ~Element();
		Element(std::vector<Node*> _pnodes, std::vector<int> _ulist);


		const int NON;									//要素を構成する節点数


		std::vector<Node*> pnodes;						//要素を構成する節点を指すポインタ
		std::map<int, int> us_to_uel;					//系従属変数番号→要素従属変数番号
		std::map<int, double> parameters;				//系パラメータ番号→要素パラメータの値
				
		
		void SetParameter(std::vector<double> _parameters, std::vector<int> _plist);	//要素パラメータを指定
		template<class N0, class N1, class ...Ns>
		void SetShapeFunction();						//形状関数を指定
		template<class N0>
		void SetShapeFunction();


		Eigen::MatrixXd Trial(std::vector<int> _ulist, std::vector<double> _xi);		//試行関数の形状関数
		Eigen::MatrixXd dTrialdx(std::vector<int> _ulist, std::vector<double> _xi);		//試行関数の形状関数のxによる一階微分
		Eigen::MatrixXd Test(std::vector<int> _ulist, std::vector<double> _xi);			//試験関数の形状関数
		Eigen::MatrixXd dTestdx(std::vector<int> _ulist, std::vector<double> _xi);		//試験関数の形状関数のxによる一階微分
		Eigen::MatrixXd Jacobian(std::vector<double> _xi);								//Jacobi行列
		Eigen::VectorXd u(std::vector<int> _ulist);		//要素内従属変数を返す

		
	protected:
		std::vector<ShapeFunction*> pshapefunctions;	//内挿用の形状関数の定義
		ShapeFunction* pmapping;						//写像用の形状関数の定義
	};

	 
	template<class N0, class N1, class ...Ns>
	inline void Element::SetShapeFunction()	{
		this->pshapefunctions.push_back(new N0());
		this->SetShapeFunction<N1, Ns...>();
	}


	template<class N0>
	inline void Element::SetShapeFunction(){
		try {
			this->pmapping = new N0();
			if (this->pshapefunctions.size() != this->us_to_uel.size()) {
				throw std::exception();
			}
		}
		catch (std::exception e) {
			std::cout << "Error in setting ShapeFunctions" << this->pshapefunctions.size() << "\t" << this->NON << std::endl;
		}
	}
}