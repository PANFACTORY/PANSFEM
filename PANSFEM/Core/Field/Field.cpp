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
	for (auto& pneumann : this->pneumanns) {
		delete pneumann;
	}
}


PANSFEM::Field::Field(std::vector<int> _ulist){
	this->uf_to_us = _ulist;
}


void PANSFEM::Field::Initialize(){
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
			if (pnode->us_to_un.find(us) != pnode->us_to_un.end()) {
				if (!pnode->is_ufixed[pnode->us_to_un[us]]) {
					size++;
				}
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
			if (this->Kmap[pnodei].second > 0) {

				for (auto pnodej : pequation->pelement->pnodes) {
					if (this->Kmap[pnodej].second > 0) {

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

	//.....Neumann���E�����̐ݒ�.....
	for (auto pneumann : this->pneumanns) {
		int Ki = this->Kmap[pneumann->pnode].first;					//�S�́\�ߓ_�������̍s�C���f�b�N�X
		for (auto usi : this->uf_to_us) {
			if (pneumann->pnode->us_to_un.find(usi) != pneumann->pnode->us_to_un.end()) {	//�ߓ_�Œ�`����Ă��邩
				if (!pneumann->pnode->is_ufixed[pneumann->pnode->us_to_un[usi]]) {			//�ߓ_��Dirichlet���E�������^�����Ă��Ȃ���
					if (pneumann->q.find(usi) != pneumann->q.end()) {
						F[Ki] += pneumann->q[usi];
					}
					Ki++;
				}
			}
		}
	}

	//.....�v�f�\�ߓ_�������̃A�Z���u�����O.....
	for (auto pequation : this->pequations) {
		int Kei = 0;												//�v�f�\�ߓ_�������̍s�C���f�b�N�X
		for (auto pnodei : pequation->pelement->pnodes) {
			int Ki = this->Kmap[pnodei].first;						//�S�́\�ߓ_�������̍s�C���f�b�N�X
			for (auto usi : this->uf_to_us) {
				if (pnodei->us_to_un.find(usi) != pnodei->us_to_un.end()) {

					//***Dirichlet�������ۂ���Ă��Ȃ��Ƃ�***
					if (!pnodei->is_ufixed[pnodei->us_to_un[usi]]) {
						//.....�W���s��.....
						int Kej = 0;								//�v�f�\�ߓ_�������̗�C���f�b�N�X
						for (auto pnodej : pequation->pelement->pnodes) {
							int Kj = this->Kmap[pnodej].first;		//�S�́\�ߓ_�������̗�C���f�b�N�X
							for (auto usj : this->uf_to_us) {
								if (pnodej->us_to_un.find(usj) != pnodej->us_to_un.end()) {
									if (!pnodej->is_ufixed[pnodej->us_to_un[usj]]) {
										K[Kj*NB + (KL + KU + Ki - Kj)] += pequation->Ke(Kei, Kej);
										Kj++;
									}
									Kej++;
								}
							}
						}
						//.....�W���x�N�g��.....
						F[Ki] += pequation->Fe[Kei];
						Ki++;
					}

					//***Dirichlet�������ۂ���Ă���Ƃ�***
					else {
						int Kej = 0;								//�v�f�\�ߓ_�������̗�C���f�b�N�X
						for (auto pnodej : pequation->pelement->pnodes) {
							int Kj = this->Kmap[pnodej].first;		//�S�́\�ߓ_�������̗�C���f�b�N�X
							for (auto usj : this->uf_to_us) {
								if (pnodej->us_to_un.find(usj) != pnodej->us_to_un.end()) {
									if (!pnodej->is_ufixed[pnodej->us_to_un[usj]]) {
										F[Kj] -= pequation->Ke(Kei, Kej) * pnodei->u(pnodej->us_to_un[usj]);
										Kj++;
									}
									Kej++;
								}
							}
						}
					}

					Kei++;
				}
			}
		}
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
		int ui = 0;
		for (auto us : this->uf_to_us) {
			if (pnode->us_to_un.find(us) != pnode->us_to_un.end()) {
				if (!pnode->is_ufixed[pnode->us_to_un[us]]) {
					pnode->u[pnode->us_to_un[us]] = F[this->Kmap[pnode].first + ui];
					ui++;
				}
			}
		}
	}
}