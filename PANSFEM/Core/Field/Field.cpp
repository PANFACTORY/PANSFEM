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

	for (auto pequation : this->pequations) {
		for (auto pnodei : pequation->pelement->pnodes) {
			for (auto pnodej : pequation->pelement->pnodes) {
				int bandwidthu = (this->Kmap[pnodej].first + this->Kmap[pnodej].second) - this->Kmap[pnodei].first - 1;
				if (this->BANDWIDTHU < bandwidthu) {
					this->BANDWIDTHU = bandwidthu;
				}
				
				int bandwidthl = (this->Kmap[pnodei].first + this->Kmap[pnodei].second) - this->Kmap[pnodej].first - 1;
				if (this->BANDWIDTHL < bandwidthl) {
					this->BANDWIDTHL = bandwidthl;
				}
			}
		}
	}
}


void PANSFEM::Field::SolveEquation(){
	//----------�S�́\�ߓ_�������̃A�Z���u�����O----------
	integer N = (integer)this->KDEGREE;			//�������̐�
	integer KL = (integer)this->BANDWIDTHL;		//�эs��̉������i�Ίp�͏����j
	integer KU = (integer)this->BANDWIDTHU;		//�эs��̏㑤���i�Ίp�͏����j
	integer NB = 2 * KL + KU + 1;
	integer NRHS = 1;							//�W���x�N�g���̐�

	std::vector<double> K = std::vector<double>(NB * N, 0.0);		//�W���s��i�эs��j
	std::vector<double> F = std::vector<double>(N * NRHS, 0.0);		//�W���x�N�g��

	for (auto pequation : this->pequations) {
		int Kei = 0;							//�v�f�\�ߓ_�������̍s�C���f�b�N�X
		for (auto pnodei : pequation->pelement->pnodes) {
			for (int i = 0; i < this->Kmap[pnodei].second; i++) {
				int Kej = 0;					//�v�f�\�ߓ_�������̗�C���f�b�N�X
				int Ki = this->Kmap[pnodei].first + i;				//�S�́\�ߓ_�������̍s�C���f�b�N�X
				//.....�W���s��.....
				for (auto pnodej : pequation->pelement->pnodes) {
					for (int j = 0; j < this->Kmap[pnodej].second; j++) {
						int Kj = this->Kmap[pnodej].first + j;		//�S�́\�ߓ_�������̗�C���f�b�N�X
						K[Kj*NB + (KL + KU + Ki - Kj)] += pequation->Ke(Kei, Kej);
						Kej++;
					}
				}

				//.....�W���x�N�g��.....
				F[Ki] += pequation->Fe[Kei];
				Kei++;
			}
		}
	}
	
	//----------���E�����̓K�p----------
	for (auto pdirichlet : this->pdirichlets) {

	}

	//----------�A��������������----------
	integer LDK = (integer)NB;			//�W���s��̐��@
	std::vector<integer> IPIV(N);		//�s�{�b�g�ʒu�L�^�p�̈�
	integer LDF = (integer)N;			//�W���x�N�g���̐��@
	integer INFO;						//�G���[�ʒm�p
	dgbsv_(&N, &KL, &KU, &NRHS, K.data(), &LDK, IPIV.data(), F.data(), &LDF, &INFO);

	//----------���̑��----------
	int iresult = 0;
	for (auto& pnode : this->pnodes) {
		for (auto i : this->uf_to_us) {
			auto iu = std::find(pnode->ulist.begin(), pnode->ulist.end(), i);
			if (iu != pnode->ulist.end()) {
				pnode->u(iu - pnode->ulist.begin()) = F[iresult];
				iresult++;
			}
		}
	}
}