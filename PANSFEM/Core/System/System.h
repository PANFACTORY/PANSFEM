//*****************************************************************************
//Title		:PANSFEM/Core/System/System.h
//Author	:Tanabe Yuta
//Date		:2019/06/08
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


#include "../Node/Node.h"
#include "../Element/Element.h"
#include "../Field/Field.h"
#include "../Equation/Equation.h"


namespace PANSFEM {
	class System
	{
	public:
		System();
		virtual ~System();
		System(int _DOX, int _DOU);


		const int DOX;		//系の独立変数
		const int DOU;		//系の従属変数


		bool ImportNode(std::string _fname);								//節点を追加
		template<class ...Ns>
		bool ImportElement(std::string _fname);								//要素を追加
		template<class Fi, class Eq>
		bool ImportField(std::vector<int> _ulist, std::string _fname);		//場を追加
		bool ImportDirichlet(std::string _fname);							//Dirichlet境界条件を入力


		virtual void Schedule() = 0;		//場の方程式を解く順番，タイミングを管理
		

	protected:
		std::vector<Node*> pnodes;			//系を構成する節点
		std::vector<Element*> pelements;	//系を構成する要素
		std::vector<Field*> pfields;		//系を構成する場
	};


	template<class ...Ns>
	inline bool System::ImportElement(std::string _fname) {
		std::ifstream ifs(_fname);

		if (!ifs.is_open()) {
			std::cout << "Element file " << _fname << " open error!" << std::endl;
			return false;
		}

		//.....一行読み飛ばす.....
		std::string str0;
		std::getline(ifs, str0);

		while (!ifs.eof()) {
			//.....一行分読み込む.....
			std::string buf;
			ifs >> buf;
			std::istringstream sbuf(buf);
			std::string str;

			//.....要素IDを読み飛ばす.....
			std::getline(sbuf, str, ',');

			//.....要素を構成する節点の数を取得.....
			std::getline(sbuf, str, ',');
			int NON = stoi(str);

			//.....節点の独立変数の値を読み込む.....
			std::vector<Node*> tmppnodes(NON);
			for (auto &pnode : tmppnodes) {
				std::getline(sbuf, str, ',');
				pnode = this->pnodes[stoi(str)];
			}

			//.....節点の従属変数の対応番号を読み込む.....
			std::vector<int> ulist;
			while (std::getline(sbuf, str, ',')) {
				ulist.push_back(stoi(str));
			}

			//.....節点を追加.....
			Element *tmppelement = new Element(tmppnodes, ulist);
			tmppelement->SetShapeFunction<Ns...>();
			this->pelements.push_back(tmppelement);
		}

		ifs.close();
		
		return true;
	}


	template<class Fi, class Eq>
	inline bool System::ImportField(std::vector<int> _ulist, std::string _fname)	{
		//----------ファイルをもとに要素―節点方程式を生成----------
		std::vector<Equation*> tmppequations;
		std::ifstream ifs(_fname);

		if (!ifs.is_open()) {
			std::cout << "Equation file " << _fname << " open error!" << std::endl;
			return false;
		}

		//.....一行読み飛ばす.....
		std::string str0;
		std::getline(ifs, str0);

		while (!ifs.eof()) {
			//.....一行分読み込む.....
			std::string buf;
			ifs >> buf;
			std::istringstream sbuf(buf);
			std::string str;

			//.....対応する要素を指すポインタを取得.....
			std::getline(sbuf, str, ',');
			Element* pelement = this->pelements[stoi(str)];

			//.....要素―節点方程式のパラメータを読み込む.....
			std::vector<double> parameters;
			while (std::getline(sbuf, str, ',')) {
				parameters.push_back(stod(str));
			}

			//.....節点を追加.....
			tmppequations.push_back(new Eq(pelement, _ulist, parameters));
		}

		ifs.close();

		//----------場を追加----------
		this->pfields.push_back(new Fi(_ulist, tmppequations));

		return true;
	}
}