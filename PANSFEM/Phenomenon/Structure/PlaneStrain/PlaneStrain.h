//*****************************************************************************
//Title		:PANSFEM/Phenomenon/Structure/PlaneStrain/PlaneStrain.h
//Author	:Tanabe Yuta
//Date		:2019/06/10
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include <vector>


#include "../../../Core/Equation/Equation.h"


namespace PANSFEM {
	class PlaneStrain :
		public Equation
	{
	public:
		PlaneStrain();
		virtual ~PlaneStrain();
		PlaneStrain(Element *_pelement, std::vector<int> _ulist, std::vector<double> _parameters);


		double E;	//�v�f��Young��
		double V;	//�v�f��Poisson��
		double t;	//�v�f�̌���


		void SetEquation();		//�v�f�\�ߓ_��������ݒ�
	};
}