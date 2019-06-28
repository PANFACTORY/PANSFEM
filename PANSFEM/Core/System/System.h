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
#include "../Neumann/Neumann.h"


namespace PANSFEM {
	class System
	{
	public:
		System();
		virtual ~System();
		System(int _DOX, int _DOU);


		const int DOX;		//�n�̓Ɨ��ϐ�
		const int DOU;		//�n�̏]���ϐ�


		bool ImportNode(std::string _fname);				//�ߓ_��ǉ�
		template<class ...Ns>
		bool ImportElement(std::vector<int> _ulist, std::string _fname);			//�v�f��ǉ�
		bool ImportParameter(std::vector<int> _plist, std::string _fname);			//�v�f�p�����[�^��ǉ�
		template<class F>
		bool ImportField(std::vector<int> _ulist);			//���ǉ�
		template<class E, class I>
		bool ImportEquation(int _fi, std::vector<int> _plist, std::string _fname);	//����\����������������
		bool ImportDirichlet(std::string _fname);			//Dirichlet���E���������
		bool ImportNeumann(int _fi, std::string _fname);	//Neumann���E���������


		virtual void Schedule() = 0;						//��̕��������������ԁC�^�C�~���O���Ǘ�
		virtual void Show();								//���ʂ�W���o�͂ɑ|�o��
		virtual void Export(std::string _fname) = 0;		//���ʂ�VTK�t�@�C���ɏ����o��	
				

	protected:
		std::vector<Node*> pnodes;			//�n���\������ߓ_
		std::vector<Element*> pelements;	//�n���\������v�f
		std::vector<Field*> pfields;		//�n���\�������
	};


	template<class ...Ns>
	inline bool System::ImportElement(std::vector<int> _ulist, std::string _fname) {
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

			//.....�v�f���\������ߓ_���w���|�C���^���擾.....
			std::vector<Node*> tmppnodes(NON);
			for (auto &pnode : tmppnodes) {
				std::getline(sbuf, str, ',');
				pnode = this->pnodes[stoi(str)];
			}

			//.....�ߓ_��ǉ�.....
			Element *tmppelement = new Element(tmppnodes, _ulist);
			tmppelement->SetShapeFunction<Ns...>();
			this->pelements.push_back(tmppelement);
		}

		ifs.close();
		
		return true;
	}


	template<class E, class I>
	inline bool System::ImportEquation(int _fi, std::vector<int> _plist, std::string _fname)	{
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

			//.....�v�f�\�ߓ_������ID��ǂݔ�΂�.....
			std::getline(sbuf, str, ',');

			//.....�Ή�����v�f���w���|�C���^���擾.....
			std::getline(sbuf, str, ',');
			Element* pelement = this->pelements[stoi(str)];

			//.....�v�f�\�ߓ_��������ǉ�.....
			this->pfields[_fi]->pequations.push_back(new E(pelement, this->pfields[_fi]->uf_to_us, _plist, new I()));
		}

		ifs.close();

		return true;
	}


	template<class F>
	inline bool System::ImportField(std::vector<int> _ulist)	{
		this->pfields.push_back(new F(_ulist));
		return true;
	}
}