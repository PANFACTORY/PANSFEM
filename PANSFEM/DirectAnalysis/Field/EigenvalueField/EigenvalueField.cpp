//*****************************************************************************
//Title		:PANSFEM/DirectAnalysis/Field/EigenvalueField/EigenvalueField.cpp
//Author	:Tanabe Yuta
//Date		:2019/07/16
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "EigenvalueField.h"


//**********CLAPACK�����OpenBLAS�̐ݒ�**********
extern "C" {
#include "f2c.h"
#include "clapack.h"
}
#pragma comment( lib, "libopenblas.dll.a" )
#pragma comment( lib, "libf2c.lib" )
#pragma comment( lib, "lapack.lib" )
//***********************************************


PANSFEM::EigenvalueField::EigenvalueField() : Field() {}


PANSFEM::EigenvalueField::~EigenvalueField(){}


PANSFEM::EigenvalueField::EigenvalueField(std::vector<int> _ulist) : Field(_ulist){}


void PANSFEM::EigenvalueField::SolveEquation(){
	/*
	//----------�S�́\�ߓ_�������̃A�Z���u�����O----------
	char JOBZ = 'V';	//�ŗL�x�N�g�������߂�ꍇ�i�ŗL�l�݂̂̏ꍇ�FJOBZ = 'N'�j(input)
	char RANGE = 'I';	//IL�Ԗڂ���IU�Ԗڂ̌ŗL�l�����߂�ꍇ(input)
	char UPLO = 'U';	//��O�p�v�f���g���ꍇ�i���O�p�v�f�FUPLO = 'L'�j(input)
	integer N = (integer)this->KDEGREE;			//�������̐�
	integer NA = (integer)this->BANDWIDTHL;		//�s��A�̏�Ίp or ���Ίp�v�f�̐�(input)
	integer NB = (integer)this->BANDWIDTHL;		//�s��B�̏�Ίp or ���Ίp�v�f�̐�(input)
	std::vector<double> K = std::vector<double>(N * NA, 0.0);		//���ӌW���s��i�эs��j
	std::vector<double> F = std::vector<double>(N * NB, 0.0);		//�E�ӌW���s��i�эs��j
	std::vector<double> Q = std::vector<double>(N * N, 0.0);		//3�d�Ίp���ɗp��������ϊ��s��iJOBZ = 'V'�̏ꍇ�̂݁j(output)
	double VL, VU;		//���߂����ŗL�l�̉����Ə��(input)
	integer IL, IU;		//���߂����ŗL�l�̃C���f�b�N�X�i�����j(input)
	double ABSTOL;		//�ŗL�l�����߂�ۂ̋��e��Ό덷(input)
						//�iABSTOL = 2 * _LAMCH('S') �Ƃ���ƍł����x���悢�j
	integer M;			//���߂�ꂽ�ŗL�l�̐�(output)
	std::vector<double> W = std::vector<double>(N);					//�ŗL�l�i�����j(output)
	std::vector<double> Z = std::vector<double>(N * N);				//�ŗL�x�N�g��(output)
	std::vector<double> WORK = std::vector<double>(7 * N);			//���[�N�X�y�[�X
	std::vector<integer> IWORK = std::vector<integer>(5 * N);		//���[�N�X�y�[�X
	std::vector<integer> IFAIL = std::vector<integer>(N);			//�ŗL�l�����߂��ۂ̎����Ɋւ�����(output)
	integer INFO;		//LAPACK�̃��[�`���̃G���[���(output)

	//integer, parameter::LDAA = size(A, 1) !�s��AB��1�����ڂ̑傫���i = KA + 1�j
	//integer, parameter::LDBB = size(B, 1) !�s��BB��1�����ڂ̑傫���i = KB + 1�j
	//integer, parameter::LDQ = size(Q, 1)  !�z��Z��1�����ڂ̑傫���i = N�j
	//integer, parameter::LDZ = size(Z, 1)  !�z��Z��1�����ڂ̑傫���i = N�j

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
										F[Kj*NB + (KL + KU + Ki - Kj)] += pequation->Fe(Kei, Kej);
										Kj++;
									}
									Kej++;
								}
							}
						}
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
	*/
}