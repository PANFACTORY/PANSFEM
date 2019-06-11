//*****************************************************************************
//Title		:PANSFEM/Core/Field/Field.h
//Author	:Tanabe Yuta
//Date		:2019/06/08
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "Field.h"


//**********CLAPACK�����OpenBLAS�̐ݒ�**********
extern "C" {
#include "f2c.h"
#include "clapack.h"
}
#pragma comment( lib, "libopenblas.dll.a" )
#pragma comment( lib, "libf2c.lib" )
#pragma comment( lib, "lapack.lib" )
//***********************************************


PANSFEM::Field::Field(){}


PANSFEM::Field::~Field(){
	for (auto& pequation : this->pequations) {
		delete pequation;
	}
}


PANSFEM::Field::Field(std::vector<int> _ulist, std::vector<Equation*> _pequations){
	//----------uf_to_us��pequation��ݒ�----------
	this->uf_to_us = _ulist;
	this->pequations = _pequations;
	
	//----------��ɑ�����ߓ_��􂢏o��----------
	for (auto pequation : this->pequations) {
		for (auto pnode : pequation->pelement->pnodes) {
			if (std::find(this->pnodes.begin(), this->pnodes.end(), pnode) == this->pnodes.end()) {
				this->pnodes.push_back(pnode);
			}
		}
	}
}


void PANSFEM::Field::MakeKmap(){
	this->Kmap.clear();
	this->KDEGREE = 0;

	for (auto pnode : this->pnodes) {
		int size = 0;
		for (auto us : this->uf_to_us) {
			if (std::find(pnode->ulist.begin(), pnode->ulist.end(), us) != pnode->ulist.end()) {
				size++;
			}
		}
		this->Kmap[pnode] = std::make_pair(this->KDEGREE, size);
		this->KDEGREE += size;
	}
}


void PANSFEM::Field::GetBandwidth(){
	this->BANDWIDTHU = 0;
	this->BANDWIDTHL = 0;


}


void PANSFEM::Field::SolveEquation(){
	//----------Kmap�̍쐬----------
	this->MakeKmap();

	//----------�v�f�\�ߓ_���������v�Z----------
	for (auto& pequation : this->pequations) {
		pequation->SetEquation();
	}

	//----------�o���h���̌v�Z----------
	this->GetBandwidth();

	//----------�S�́\�ߓ_�������̃A�Z���u�����O----------
	integer N = (integer)this->KDEGREE;			//�������̐�
	integer KL = (integer)this->BANDWIDTHL;		//�эs��̉������i�Ίp�͏����j
	integer KU = (integer)this->BANDWIDTHU;		//�эs��̏㑤���i�Ίp�͏����j
	integer NB = 2 * KL + KU + 1;
	integer NRHS = 1;							//�W���x�N�g���̐�

	std::vector<double> K = std::vector<double>(NB * N, 0.0);		//�W���s��i�эs��j
	std::vector<double> F = std::vector<double>(N * NRHS, 0.0);		//�W���x�N�g��

	for (auto pequation : this->pequations) {
		
	}

	//----------���E�����̓K�p----------

	//----------�A��������������----------
	integer LDK = (integer)NB;			//�W���s��̐��@
	std::vector<integer> IPIV(N);		//�s�{�b�g�ʒu�L�^�p�̈�
	integer LDF = (integer)N;			//�W���x�N�g���̐��@
	integer INFO;						//�G���[�ʒm�p
	dgbsv_(&N, &KL, &KU, &NRHS, K.data(), &LDK, IPIV.data(), F.data(), &LDF, &INFO);

	//----------���̑��----------
	for (auto& pnode : this->pnodes) {

	}
}