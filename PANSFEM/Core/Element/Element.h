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


		const int DOU;								//�v�f���Œ�`���ꂽ�]���ϐ��̎���
		const int NON;								//�v�f���\������ߓ_���i���`��֐��̐��j


		std::vector<Node*> pnodes;					//�v�f���\������ߓ_���w���|�C���^
		std::map<int, int> us_to_uel;				//�n�]���ϐ��ԍ����v�f�]���ϐ��ԍ�
		
		
		template<class N0, class N1, class ...Ns>
		void SetTrial();							//���s�֐��̌`��֐����w��
		template<class N0>
		void SetTrial();
		template<class N0, class N1, class ...Ns>
		void SetTest();								//�����֐��̌`��֐����w��
		template<class N0>
		void SetTest();


	protected:
		std::vector<ShapeFunction*> TrialDefines;	//���s�֐��̌`��֐��̒�`
		std::vector<ShapeFunction*> TestDefines;	//�����֐��̌`��֐��̒�`
	};

	 
	template<class N0, class N1, class ...Ns>
	inline void Element::SetTrial()	{
		this->TrialDefines.push_back(new N0());
		this->SetTrial<N1, Ns...>();
	}


	template<class N0>
	inline void Element::SetTrial(){
		this->TrialDefines.push_back(new N0());
		//----------�`��֐��̐��̌���----------
		try {
			if (this->DOU != this->TrialDefines.size()) {
				throw std::exception();
			}
		}
		catch (std::exception e) {
			std::cout << "Error in Element at SetTrial" << std::endl;
		}
	}
	
	
	template<class N0, class N1, class ...Ns>
	inline void Element::SetTest() {
		this->TestDefines.push_back(new N0());
		this->SetTest<N1, Ns...>();
	}


	template<class N0>
	inline void Element::SetTest() {
		this->TestDefines.push_back(new N0());
		//----------�`��֐��̐��̌���----------
		try {
			if (this->DOU != this->TestDefines.size()) {
				throw std::exception();
			}
		}
		catch (std::exception e) {
			std::cout << "Error in Element at SetTest" << std::endl;
		}
	}
}