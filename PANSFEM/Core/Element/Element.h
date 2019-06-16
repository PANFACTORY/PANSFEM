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
	class ShapeFunction;

	class Element
	{
	public:
		Element();
		virtual ~Element();
		Element(std::vector<Node*> _pnodes, std::vector<int> _ulist);


		const int NON;									//�v�f���\������ߓ_���i���`��֐��̐��j


		std::vector<Node*> pnodes;						//�v�f���\������ߓ_���w���|�C���^
		std::map<int, int> us_to_uel;					//�n�]���ϐ��ԍ����v�f�]���ϐ��ԍ�
				
		
		template<class N0, class N1, class ...Ns>
		void SetShapeFunction();						//�`��֐����w��
		template<class N0>
		void SetShapeFunction();


		Eigen::MatrixXd Trial(std::vector<int> _ulist, std::vector<double> _xi);		//���s�֐��̌`��֐�
		Eigen::MatrixXd dTrialdx(std::vector<int> _ulist, std::vector<double> _xi);		//���s�֐��̌`��֐���x�ɂ���K����
		Eigen::MatrixXd Test(std::vector<int> _ulist, std::vector<double> _xi);			//�����֐��̌`��֐�
		Eigen::MatrixXd dTestdx(std::vector<int> _ulist, std::vector<double> _xi);		//�����֐��̌`��֐���x�ɂ���K����
		Eigen::MatrixXd Jacobian(std::vector<double> _xi);								//Jacobi�s��

		
	protected:
		std::vector<ShapeFunction*> pshapefunctions;	//���}�p�̌`��֐��̒�`
		ShapeFunction* pmapping;						//�ʑ��p�̌`��֐��̒�`
	};

	 
	template<class N0, class N1, class ...Ns>
	inline void Element::SetShapeFunction()	{
		this->pshapefunctions.push_back(new N0(this));
		this->SetShapeFunction<N1, Ns...>();
	}


	template<class N0>
	inline void Element::SetShapeFunction(){
		this->pmapping = new N0(this);
	}
}