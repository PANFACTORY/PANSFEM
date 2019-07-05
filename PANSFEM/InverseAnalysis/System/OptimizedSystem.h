//*****************************************************************************
//Title		:PANSFEM/InversedAnalysis/System/OptimizedSystem.h
//Author	:Tanabe Yuta
//Date		:2019/06/30
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include "../../DirectAnalysis/System/System.h"
#include "../Function/Function.h"


namespace PANSFEM {
	class OptimizedSystem :
		public System
	{
	public:
		OptimizedSystem();
		virtual ~OptimizedSystem();
		OptimizedSystem(int _DOX, int _DOU, std::vector<int> _plist);


		template<class F>
		bool ImportObjective(std::vector<int> _refulist, std::vector<int> _refplist);		//目的関数を追加
		template<class F>
		bool ImportConstraint(std::vector<int> _refulist, std::vector<int> _refplist);		//制約関数を追加


		bool ImportOptimizedElement(std::string _fname);				//最適化対象の要素を設定


		template<class I>
		bool ImportElementToObjective(int _id, std::string _fname);		//目的関数に要素に合わせた積分法を追加
		template<class I>
		bool ImportElementToConstraint(int _id, std::string _fname);	//制約条件に要素に合わせた積分法を追加


		void Schedule();							//順解析，感度計算を行う順番，タイミングを管理
													//本来なら最適化手法によって変えるべき部分，具体的なイメージをつかむため今はこうする
		void Export(std::string _fname);			//最適化の結果を出力


	protected:
		std::vector<int> plist;						//最適化されるパラメータ
		std::vector<Element*> poptimizedelements;	//設計変数を持つ要素
		std::vector<Function*> pobjectives;			//目的関数を指すポインタ
		std::vector<Function*> pconstraints;		//制約関数を指すポインタ
	};


	template<class F>
	inline bool OptimizedSystem::ImportObjective(std::vector<int> _refulist, std::vector<int> _refplist){
		this->pobjectives.push_back(new F(this->plist, _refulist, _refplist));
		return true;
	}


	template<class F>
	inline bool OptimizedSystem::ImportConstraint(std::vector<int> _refulist, std::vector<int> _refplist){
		this->pconstraints.push_back(new F(this->plist, _refulist, _refplist));
		return true;
	}
	
	//↓要修正
	template<class I>
	inline bool OptimizedSystem::ImportElementToObjective(int _id, std::string _fname){
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

			//.....IDを読み飛ばす.....
			std::getline(sbuf, str, ',');

			//.....対応する要素を指すポインタを取得.....
			std::getline(sbuf, str, ',');
			Element* tmppelement = this->pelements[stoi(str)];

			//.....目的関数に要素と積分法を追加.....
			this->pobjectives[_id]->pelements.push_back(tmppelement);
			this->pobjectives[_id]->pintegrations[tmppelement] = new I();
		}

		ifs.close();

		return true;
	}


	template<class I>
	inline bool OptimizedSystem::ImportElementToConstraint(int _id, std::string _fname)	{
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

			//.....IDを読み飛ばす.....
			std::getline(sbuf, str, ',');

			//.....対応する要素を指すポインタを取得.....
			std::getline(sbuf, str, ',');
			Element* tmppelement = this->pelements[stoi(str)];

			//.....目的関数に要素と積分法を追加.....
			this->pconstraints[_id]->pelements.push_back(tmppelement);
			this->pconstraints[_id]->pintegrations[tmppelement] = new I();
		}

		ifs.close();
		
		return false;
	}
}