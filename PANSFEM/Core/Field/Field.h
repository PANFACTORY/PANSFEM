//*****************************************************************************
//Title		:PANSFEM/Core/Field/Field.h
//Author	:Tanabe Yuta
//Date		:2019/06/08
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include <vector>
#include <map>


#include "../Equation/Equation.h"


namespace PANSFEM {
	class Field
	{
	public:
		Field();
		virtual ~Field();
		Field(std::vector<int> _ulist, std::vector<Equation*> _pequations);


		int KDEGREE;		//��̑S�́\�ߓ_�������̎���
		int BANDWIDTHU;		//��o���h��
		int BANDWIDTHL;		//���o���h��


		std::vector<int> uf_to_us;				//��̏]���ϐ��ԍ����n�̏]���ϐ��ԍ�
		std::map<Node*, std::pair<int, int> > Kmap;		//�ߓ_���w���|�C���^���S�́\�ߓ_�������ʒu�C��


		void MakeKmap();						//Kmap�̍쐬
		void GetBandwidth();					//�o���h�����v�Z
		void SolveEquation();					//�S�́\�ߓ_�������������i�A���������j
		//void SolveEigen();						//�S�́\�ߓ_�������������i�ŗL�l���j


	protected:
		std::vector<Equation*> pequations;		//����L�q����v�f�\�ߓ_���������w���|�C���^
		std::vector<Node*> pnodes;				//��ɑ�����ߓ_���w���|�C���^
	};
}