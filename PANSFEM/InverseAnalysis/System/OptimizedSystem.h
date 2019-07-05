//*****************************************************************************
//Title		:PANSFEM/InversedAnalysis/System/OptimizedSystem.h
//Author	:Tanabe Yuta
//Date		:2019/06/30
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include "../../DirectAnalysis/System/System.h"
#include "../Function/Function.h"


namespace PANSFEM {
	class OptimizedSystem :
		public System
	{
	public:
		OptimizedSystem();
		virtual ~OptimizedSystem();
		OptimizedSystem(int _DOX, int _DOU, std::vector<int> _plist);


		template<class F>
		bool ImportObjective(std::vector<int> _refulist, std::vector<int> _refplist);		//�ړI�֐���ǉ�
		template<class F>
		bool ImportConstraint(std::vector<int> _refulist, std::vector<int> _refplist);		//����֐���ǉ�


		bool ImportOptimizedElement(std::string _fname);				//�œK���Ώۂ̗v�f��ݒ�


		template<class I>
		bool ImportElementToObjective(int _id, std::string _fname);		//�ړI�֐��ɗv�f�ɍ��킹���ϕ��@��ǉ�
		template<class I>
		bool ImportElementToConstraint(int _id, std::string _fname);	//��������ɗv�f�ɍ��킹���ϕ��@��ǉ�


		void Schedule();							//����́C���x�v�Z���s�����ԁC�^�C�~���O���Ǘ�
													//�{���Ȃ�œK����@�ɂ���ĕς���ׂ������C��̓I�ȃC���[�W�����ނ��ߍ��͂�������
		void Export(std::string _fname);			//�œK���̌��ʂ��o��


	protected:
		std::vector<int> plist;						//�œK�������p�����[�^
		std::vector<Element*> poptimizedelements;	//�݌v�ϐ������v�f
		std::vector<Function*> pobjectives;			//�ړI�֐����w���|�C���^
		std::vector<Function*> pconstraints;		//����֐����w���|�C���^
	};


	template<class F>
	inline bool OptimizedSystem::ImportObjective(std::vector<int> _refulist, std::vector<int> _refplist){
		this->pobjectives.push_back(new F(this->plist, _refulist, _refplist));
		return true;
	}


	template<class F>
	inline bool OptimizedSystem::ImportConstraint(std::vector<int> _refulist, std::vector<int> _refplist){
		this->pconstraints.push_back(new F(this->plist, _refulist, _refplist));
		return true;
	}
	
	//���v�C��
	template<class I>
	inline bool OptimizedSystem::ImportElementToObjective(int _id, std::string _fname){
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

			//.....ID��ǂݔ�΂�.....
			std::getline(sbuf, str, ',');

			//.....�Ή�����v�f���w���|�C���^���擾.....
			std::getline(sbuf, str, ',');
			Element* tmppelement = this->pelements[stoi(str)];

			//.....�ړI�֐��ɗv�f�Ɛϕ��@��ǉ�.....
			this->pobjectives[_id]->pelements.push_back(tmppelement);
			this->pobjectives[_id]->pintegrations[tmppelement] = new I();
		}

		ifs.close();

		return true;
	}


	template<class I>
	inline bool OptimizedSystem::ImportElementToConstraint(int _id, std::string _fname)	{
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

			//.....ID��ǂݔ�΂�.....
			std::getline(sbuf, str, ',');

			//.....�Ή�����v�f���w���|�C���^���擾.....
			std::getline(sbuf, str, ',');
			Element* tmppelement = this->pelements[stoi(str)];

			//.....�ړI�֐��ɗv�f�Ɛϕ��@��ǉ�.....
			this->pconstraints[_id]->pelements.push_back(tmppelement);
			this->pconstraints[_id]->pintegrations[tmppelement] = new I();
		}

		ifs.close();
		
		return false;
	}
}