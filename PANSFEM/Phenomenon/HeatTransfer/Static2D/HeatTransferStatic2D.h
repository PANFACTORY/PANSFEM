//*****************************************************************************
//Title		:PANSFEM/Phenomenon/HeatTransfer/Static2D/HeatTransferStatic2D.h
//Author	:Tanabe Yuta
//Date		:2019/06/25
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include "../../../Core/Equation/Equation.h"


namespace PANSFEM {
	class HeatTransferStatic2D :
		public Equation
	{
	public:
		HeatTransferStatic2D();
		virtual ~HeatTransferStatic2D();
		HeatTransferStatic2D(Element *_pelement, std::vector<int> _ulist, std::vector<double> _parameters);


		double alpha;			//”M“`“±—¦


		void SetEquation();		//—v‘f\ß“_•û’ö®‚ğİ’è
	};
}