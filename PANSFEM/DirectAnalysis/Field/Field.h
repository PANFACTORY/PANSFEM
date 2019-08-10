//*****************************************************************************
//Title		:PANSFEM/Core/Field/Field.h
//Author	:Tanabe Yuta
//Date		:2019/06/08
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include <vector>
#include <map>
#include <fstream>


#include "../Equation/Equation.h"
#include "../Neumann/Neumann.h"


namespace PANSFEM {
	class Field
	{
	public:
		Field();
		virtual ~Field();
		Field(std::vector<int> _ulist);


		int KDEGREE;		//��̑S�́\�ߓ_�������̎���
		int BANDWIDTHU;		//��o���h��
		int BANDWIDTHL;		//���o���h��


		std::vector<int> uf_to_us;				//��̏]���ϐ��ԍ����n�̏]���ϐ��ԍ�
		std::map<Node*, std::pair<int, int> > Kmap;		//�ߓ_���w���|�C���^���S�́\�ߓ_�������ʒu�C��


		void Initialize();						//��ɑ�����v�f�A�ߓ_���w���|�C���^���W�߂�
		void MakeKmap();						//Kmap�̍쐬
		void GetBandwidth();					//�o���h�����v�Z
		virtual void SolveEquation() = 0;		//�S�́\�ߓ_�������������i���`�A���������j
		

		std::vector<Equation*> pequations;		//����L�q����v�f�\�ߓ_���������w���|�C���^
		std::vector<Node*> pnodes;				//��ɑ�����ߓ_���w���|�C���^
		std::vector<Neumann*> pneumanns;		//����L�q����Neumann���E����
	};
}