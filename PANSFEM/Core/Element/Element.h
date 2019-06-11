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


		const int DOU;								//要素内で定義された従属変数の次元
		const int NON;								//要素を構成する節点数（＝形状関数の数）


		std::vector<Node*> pnodes;					//要素を構成する節点を指すポインタ
		std::map<int, int> us_to_uel;				//系従属変数番号→要素従属変数番号
		
		
		template<class N0, class N1, class ...Ns>
		void SetTrial();							//試行関数の形状関数を指定
		template<class N0>
		void SetTrial();
		template<class N0, class N1, class ...Ns>
		void SetTest();								//試験関数の形状関数を指定
		template<class N0>
		void SetTest();


	protected:
		std::vector<ShapeFunction*> TrialDefines;	//試行関数の形状関数の定義
		std::vector<ShapeFunction*> TestDefines;	//試験関数の形状関数の定義
	};

	 
	template<class N0, class N1, class ...Ns>
	inline void Element::SetTrial()	{
		this->TrialDefines.push_back(new N0());
		this->SetTrial<N1, Ns...>();
	}


	template<class N0>
	inline void Element::SetTrial(){
		this->TrialDefines.push_back(new N0());
		//----------形状関数の数の検証----------
		try {
			if (this->DOU != this->TrialDefines.size()) {
				throw std::exception();
			}
		}
		catch (std::exception e) {
			std::cout << "Error in Element at SetTrial" << std::endl;
		}
	}
	
	
	template<class N0, class N1, class ...Ns>
	inline void Element::SetTest() {
		this->TestDefines.push_back(new N0());
		this->SetTest<N1, Ns...>();
	}


	template<class N0>
	inline void Element::SetTest() {
		this->TestDefines.push_back(new N0());
		//----------形状関数の数の検証----------
		try {
			if (this->DOU != this->TestDefines.size()) {
				throw std::exception();
			}
		}
		catch (std::exception e) {
			std::cout << "Error in Element at SetTest" << std::endl;
		}
	}
}