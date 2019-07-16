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
	class Element
	{
	public:
		Element();
		virtual ~Element();
		Element(std::vector<Node*> _pnodes, std::vector<int> _ulist);


		const int NON;									//�v�f���\������ߓ_��


		std::vector<Node*> pnodes;						//�v�f���\������ߓ_���w���|�C���^
		std::map<int, int> us_to_uel;					//�n�]���ϐ��ԍ����v�f�]���ϐ��ԍ�
		std::map<int, double> parameters;				//�n�p�����[�^�ԍ����v�f�p�����[�^�̒l
				
		
		void SetParameter(std::vector<double> _parameters, std::vector<int> _plist);	//�v�f�p�����[�^���w��
		template<class N0, class N1, class ...Ns>
		void SetShapeFunction();						//�`��֐����w��
		template<class N0>
		void SetShapeFunction();


		Eigen::MatrixXd Trial(std::vector<int> _ulist, std::vector<double> _xi);		//���s�֐��̌`��֐�
		Eigen::MatrixXd dTrialdx(std::vector<int> _ulist, std::vector<double> _xi);		//���s�֐��̌`��֐���x�ɂ���K����
		Eigen::MatrixXd Test(std::vector<int> _ulist, std::vector<double> _xi);			//�����֐��̌`��֐�
		Eigen::MatrixXd dTestdx(std::vector<int> _ulist, std::vector<double> _xi);		//�����֐��̌`��֐���x�ɂ���K����
		Eigen::MatrixXd Jacobian(std::vector<double> _xi);								//Jacobi�s��
		Eigen::VectorXd u(std::vector<int> _ulist);		//�v�f���]���ϐ���Ԃ�

		
	protected:
		std::vector<ShapeFunction*> pshapefunctions;	//���}�p�̌`��֐��̒�`
		ShapeFunction* pmapping;						//�ʑ��p�̌`��֐��̒�`
	};

	 
	template<class N0, class N1, class ...Ns>
	inline void Element::SetShapeFunction()	{
		this->pshapefunctions.push_back(new N0());
		this->SetShapeFunction<N1, Ns...>();
	}


	template<class N0>
	inline void Element::SetShapeFunction(){
		try {
			this->pmapping = new N0();
			if (this->pshapefunctions.size() != this->us_to_uel.size()) {
				throw std::exception();
			}
		}
		catch (std::exception e) {
			std::cout << "Error in setting ShapeFunctions" << this->pshapefunctions.size() << "\t" << this->NON << std::endl;
		}
	}
}