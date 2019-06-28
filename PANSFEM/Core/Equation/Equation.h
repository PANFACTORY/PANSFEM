//*****************************************************************************
//Title		:PANSFEM/Core/Equation/Equation.h
//Author	:Tanabe Yuta
//Date		:2019/06/09
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include <vector>
#include <map>


#include "../Element/Element.h"
#include "../Integration/Integration.h"


namespace PANSFEM {
	class Equation
	{
	public:
		Equation();
		virtual ~Equation();
		Equation(Element *_pelement, std::vector<int> _ulist, std::vector<int> _plist, Integration* _pintegration, int _DOX, int _DOU, int _NOP);


		const int DOX;		//�v�f�\�ߓ_���������L�q����Ɨ��ϐ����R�x�F�p�����Ɏw��
		const int DOU;		//�v�f�\�ߓ_���������L�q����]���ϐ����R�x�F�p�����Ɏw��
		const int NOP;		//�v�f�\�ߓ_���������L�q����p�����[�^�̐��F�p�����Ɏw��


		Element *pelement;	//�v�f�\�ߓ_�������ɑΉ�����v�f���w���|�C���^
		std::vector<int> ueq_to_us;			//�v�f�\�ߓ_�������]���ϐ��ԍ����n�]���ϐ��ԍ�
		std::vector<int> peq_to_ps;			//�v�f�\�ߓ_�������p�����[�^�ԍ����n�p�����[�^�ԍ�
		Integration* pintegration;			//�v�f�\�ߓ_�������𐶐�����Ƃ��̐ϕ��@
				

		Eigen::MatrixXd Ke;	//�v�f�\�ߓ_�������̌W���s��
		Eigen::VectorXd Fe;	//�v�f�\�ߓ_�������̌W���x�N�g��


		void SetEquation();	//�v�f�\�ߓ_��������ݒ�
		virtual Eigen::MatrixXd GetKe(std::vector<double> _xi) = 0;
		virtual Eigen::VectorXd GetFe(std::vector<double> _xi) = 0;
	};
}