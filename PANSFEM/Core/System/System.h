//*****************************************************************************
//Title		:PANSFEM/Core/System/System.h
//Author	:Tanabe Yuta
//Date		:2019/06/08
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


#include "../Node/Node.h"
#include "../Element/Element.h"
#include "../Field/Field.h"
#include "../Equation/Equation.h"


namespace PANSFEM {
	class System
	{
	public:
		System();
		virtual ~System();
		System(int _DOX, int _DOU);


		const int DOX;		//�n�̓Ɨ��ϐ�
		const int DOU;		//�n�̏]���ϐ�


		bool ImportNode(std::string _fname);								//�ߓ_��ǉ�
		template<class ...Ns>
		bool ImportElement(std::string _fname);								//�v�f��ǉ�
		template<class Fi, class Eq>
		bool ImportField(std::vector<int> _ulist, std::string _fname);		//���ǉ�
		bool ImportDirichlet(std::string _fname);							//Dirichlet���E���������


		virtual void Schedule() = 0;		//��̕��������������ԁC�^�C�~���O���Ǘ�
		

	protected:
		std::vector<Node*> pnodes;			//�n���\������ߓ_
		std::vector<Element*> pelements;	//�n���\������v�f
		std::vector<Field*> pfields;		//�n���\�������
	};


	template<class ...Ns>
	inline bool System::ImportElement(std::string _fname) {
		std::ifstream ifs(_fname);

		if (!ifs.is_open()) {
			std::cout << "Element file " << _fname << " open error!" << std::endl;
			return false;
		}

		//.....��s�ǂݔ�΂�.....
		std::string str0;
		std::getline(ifs, str0);

		while (!ifs.eof()) {
			//.....��s���ǂݍ���.....
			std::string buf;
			ifs >> buf;
			std::istringstream sbuf(buf);
			std::string str;

			//.....�v�fID��ǂݔ�΂�.....
			std::getline(sbuf, str, ',');

			//.....�v�f���\������ߓ_�̐����擾.....
			std::getline(sbuf, str, ',');
			int NON = stoi(str);

			//.....�ߓ_�̓Ɨ��ϐ��̒l��ǂݍ���.....
			std::vector<Node*> tmppnodes(NON);
			for (auto &pnode : tmppnodes) {
				std::getline(sbuf, str, ',');
				pnode = this->pnodes[stoi(str)];
			}

			//.....�ߓ_�̏]���ϐ��̑Ή��ԍ���ǂݍ���.....
			std::vector<int> ulist;
			while (std::getline(sbuf, str, ',')) {
				ulist.push_back(stoi(str));
			}

			//.....�ߓ_��ǉ�.....
			Element *tmppelement = new Element(tmppnodes, ulist);
			tmppelement->SetShapeFunction<Ns...>();
			this->pelements.push_back(tmppelement);
		}

		ifs.close();
		
		return true;
	}


	template<class Fi, class Eq>
	inline bool System::ImportField(std::vector<int> _ulist, std::string _fname)	{
		//----------�t�@�C�������Ƃɗv�f�\�ߓ_�������𐶐�----------
		std::vector<Equation*> tmppequations;
		std::ifstream ifs(_fname);

		if (!ifs.is_open()) {
			std::cout << "Equation file " << _fname << " open error!" << std::endl;
			return false;
		}

		//.....��s�ǂݔ�΂�.....
		std::string str0;
		std::getline(ifs, str0);

		while (!ifs.eof()) {
			//.....��s���ǂݍ���.....
			std::string buf;
			ifs >> buf;
			std::istringstream sbuf(buf);
			std::string str;

			//.....�Ή�����v�f���w���|�C���^���擾.....
			std::getline(sbuf, str, ',');
			Element* pelement = this->pelements[stoi(str)];

			//.....�v�f�\�ߓ_�������̃p�����[�^��ǂݍ���.....
			std::vector<double> parameters;
			while (std::getline(sbuf, str, ',')) {
				parameters.push_back(stod(str));
			}

			//.....�ߓ_��ǉ�.....
			tmppequations.push_back(new Eq(pelement, _ulist, parameters));
		}

		ifs.close();

		//----------���ǉ�----------
		this->pfields.push_back(new Fi(_ulist, tmppequations));

		return true;
	}
}