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
#include "../Neumann/Neumann.h"


namespace PANSFEM {
	class System
	{
	public:
		System();
		virtual ~System();
		System(int _DOX, int _DOU);


		const int DOX;		//系の独立変数
		const int DOU;		//系の従属変数


		bool ImportNode(std::string _fname);				//節点を追加
		template<class ...Ns>
		bool ImportElement(std::string _fname);				//要素を追加
		template<class F>
		bool ImportField(std::vector<int> _ulist);			//場を追加
		template<class E, class I>
		bool ImportEquation(int _fi, std::string _fname);	//場を構成する方程式を入力
		bool ImportDirichlet(std::string _fname);			//Dirichlet境界条件を入力
		bool ImportNeumann(int _fi, std::string _fname);	//Neumann境界条件を入力


		virtual void Schedule() = 0;						//場の方程式を解く順番，タイミングを管理
		virtual void Show();								//結果を標準出力に掃出す
		virtual void Export(std::string _fname) = 0;		//結果をVTKファイルに書き出す	
				

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


	template<class E, class I>
	inline bool System::ImportEquation(int _fi, std::string _fname)	{
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
			this->pfields[_fi]->pequations.push_back(new E(pelement, this->pfields[_fi]->uf_to_us, parameters, new I()));
		}

		ifs.close();

		return true;
	}


	template<class F>
	inline bool System::ImportField(std::vector<int> _ulist)	{
		this->pfields.push_back(new F(_ulist));
		return true;
	}
}