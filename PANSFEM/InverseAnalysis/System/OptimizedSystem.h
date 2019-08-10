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


		virtual void Schedule() = 0;				//����́C���x�v�Z���s�����ԁC�^�C�~���O���Ǘ�
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
		Function* tmpfunction = new F(this->plist, _refulist, _refplist);
		tmpfunction->pelements = this->poptimizedelements;
		this->pobjectives.push_back(tmpfunction);
		return true;
	}


	template<class F>
	inline bool OptimizedSystem::ImportConstraint(std::vector<int> _refulist, std::vector<int> _refplist){
		Function* tmpfunction = new F(this->plist, _refulist, _refplist);
		tmpfunction->pelements = this->poptimizedelements;
		this->pconstraints.push_back(tmpfunction);
		return true;
	}
	
	
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

			//.....�Ή�����v�f�ɍ��킹�Đϕ��@��ǉ�.....
			std::getline(sbuf, str, ',');
			this->pobjectives[_id]->pintegrations[this->pelements[stoi(str)]] = new I();
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

			//.....�Ή�����v�f�ɍ��킹�Đϕ��@��ǉ�.....
			std::getline(sbuf, str, ',');
			this->pconstraints[_id]->pintegrations[this->pelements[stoi(str)]] = new I();
		}

		ifs.close();
		
		return false;
	}
}