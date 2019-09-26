//*****************************************************************************
//Title		:PANSFEM/DirectAnalysis/Parameter/Parameter.h
//Author	:Tanabe Yuta
//Date		:2019/09/25
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include <vector>
#include <map>


namespace PANSFEM {
	class Parameter
	{
	public:
		Parameter();
		virtual ~Parameter();
		Parameter(std::vector<double> _parameters, std::vector<int> _plist);


		std::map<int, double> parameters;				//系パラメータ番号→要素パラメータの値
	};
}