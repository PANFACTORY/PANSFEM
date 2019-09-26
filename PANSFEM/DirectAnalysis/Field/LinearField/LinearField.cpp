//*****************************************************************************
//Title		:PANSFEM/DirectAnalysis/Field/LinearField/LinearField.cpp
//Author	:Tanabe Yuta
//Date		:2019/06/12
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include <chrono>


#include "LinearField.h"
#include "../../../LinearAlgebra/CSR.h"
#include "../../../LinearAlgebra/PreCSR.h"
#include "../../../LinearAlgebra/CGSolvers.h"


//**********CLAPACK�����OpenBLAS�̐ݒ�**********
extern "C" {
#include "f2c.h"
#include "clapack.h"
}
#pragma comment( lib, "libopenblas.dll.a" )
#pragma comment( lib, "libf2c.lib" )
#pragma comment( lib, "lapack.lib" )
//***********************************************


PANSFEM::LinearField::LinearField() : Field() {}


PANSFEM::LinearField::~LinearField(){}


PANSFEM::LinearField::LinearField(std::vector<int> _ulist) : Field(_ulist){}


void PANSFEM::LinearField::SolveEquation() {
	//----------�v�f�\�ߓ_�������̌v�Z----------
	/*for (auto& pequation : this->pequations) {
		pequation->SetEquation();
	}*/
#pragma omp parallel for
	for (int i = 0; i < this->pequations.size(); i++) {
		this->pequations[i]->SetEquation();
	}

	//----------�S�́\�ߓ_�������̃A�Z���u�����O----------
	PreCSR<double> K = PreCSR<double>(this->KDEGREE, this->KDEGREE);	//�W���s��
	std::vector<double> F = std::vector<double>(this->KDEGREE, 0.0);	//�W���x�N�g��

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
										K.set(Ki, Kj, K.get(Ki, Kj) + pequation->Ke(Kei, Kej));
										Kj++;
									}
									Kej++;
								}
							}
						}
						//.....�W���x�N�g��.....
						F[Ki] += pequation->Fe(Kei, 0);
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
										F[Kj] -= pequation->Ke(Kej, Kei) * pnodei->u(pnodei->us_to_un[usi]);
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
	CSR<double> K2 = CSR<double>(K);

	//Scaling(K2, F);

	std::chrono::system_clock::time_point start, end;
	start = std::chrono::system_clock::now();

	CSR<double> M = ILU0(K2);
	//std::vector<double> res = CG(K2, F, this->KDEGREE, 1.0e-10);
	//std::vector<double> res = SORCG(K2, F, this->KDEGREE, 1.0e-10, 1.7);
	//std::vector<double> res = ScalingCG(K2, F, this->KDEGREE, 1.0e-10);
	std::vector<double> res = ILU0CG(K2, M, F, this->KDEGREE, 1.0e-10);
	//std::vector<double> res = BiCGSTAB(K2, F, this->KDEGREE, 1.0e-10);
	//std::vector<double> res = ILU0BiCGSTAB(K2, M, F, this->KDEGREE, 1.0e-10);

	end = std::chrono::system_clock::now();
	std::cout << static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0) << std::endl;

	//----------���̑��----------
	int iresult = 0;
	for (auto& pnode : this->pnodes) {
		int ui = 0;
		for (auto us : this->uf_to_us) {
			if (pnode->us_to_un.find(us) != pnode->us_to_un.end()) {
				if (!pnode->is_ufixed[pnode->us_to_un[us]]) {
					pnode->u[pnode->us_to_un[us]] = res[this->Kmap[pnode].first + ui];
					ui++;
				}
			}
		}
	}
}


/*void PANSFEM::LinearField::SolveEquation(){
	//----------�v�f�\�ߓ_�������̌v�Z----------
	for (auto& pequation : this->pequations) {
		pequation->SetEquation();
	}

	//----------�S�́\�ߓ_�������̃A�Z���u�����O----------
	integer N = (integer)this->KDEGREE;			//�������̐�
	integer KL = (integer)this->BANDWIDTHL;		//�эs��̉������i�Ίp�͏����j
	integer KU = (integer)this->BANDWIDTHU;		//�эs��̏㑤���i�Ίp�͏����j
	integer NB = 2 * KL + KU + 1;
	integer NRHS = 1;							//�W���x�N�g���̐�

	std::vector<double> K = std::vector<double>(NB * N, 0.0);		//�W���s��i�эs��j
	std::vector<double> F = std::vector<double>(N, 0.0);			//�W���x�N�g��

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
						F[Ki] += pequation->Fe(Kei, 0);
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
										F[Kj] -= pequation->Ke(Kej, Kei) * pnodei->u(pnodei->us_to_un[usi]);
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
}*/