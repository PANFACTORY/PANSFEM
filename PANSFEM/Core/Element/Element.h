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


		const int DOU;									//�v�f���Œ�`���ꂽ�]���ϐ��̎���
		const int NON;									//�v�f���\������ߓ_���i���`��֐��̐��j


		std::vector<Node*> pnodes;						//�v�f���\������ߓ_���w���|�C���^
		std::map<int, int> us_to_uel;					//�n�]���ϐ��ԍ����v�f�]���ϐ��ԍ�
				
		
		template<class N0, class N1, class ...Ns>
		void SetShapeFunction();						//�`��֐����w��
		template<class N0>
		void SetShapeFunction();


		Eigen::MatrixXd Trial(std::vector<int> _ulist, std::vector<int> _x);		//���s�֐��s���Ԃ�
		Eigen::MatrixXd dTrialdx(std::vector<int> _ulist, std::vector<int> _x);		//���s�֐���x�ɂ���K�����s���Ԃ�
		Eigen::MatrixXd Test(std::vector<int> _ulist, std::vector<int> _x);			//�����֐��s���Ԃ�
		Eigen::MatrixXd dTestdx(std::vector<int> _ulist, std::vector<int> _x);		//�����֐���x�ɂ���K�����s���Ԃ�
		

	protected:
		std::vector<ShapeFunction*> pshapefunctions;	//�`��֐��̒�`
	};

	 
	template<class N0, class N1, class ...Ns>
	inline void Element::SetShapeFunction()	{
		this->pshapefunctions.push_back(new N0(this));
		this->SetShapeFunction<N1, Ns...>();
	}


	template<class N0>
	inline void Element::SetShapeFunction(){
		this->pshapefunctions.push_back(new N0(this));
		//----------�`��֐��̐��̌���----------
		try {
			if (this->DOU != this->pshapefunctions.size()) {
				throw std::exception();
			}
		}
		catch (std::exception e) {
			std::cout << "Error in Element at SetShapeFunction" << std::endl;
		}
	}
}