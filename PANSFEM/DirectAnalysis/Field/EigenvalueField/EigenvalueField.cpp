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
	//----------�v�f�\�ߓ_�������̌v�Z----------
	for (auto& pequation : this->pequations) {
		pequation->SetEquation();
	}

	//----------�v�f�\�ߓ_�������̃A�Z���u�����O----------
	integer N = (integer)this->KDEGREE;							//�������̐�
	integer KA = (integer)this->BANDWIDTHL;						//�s��A�̏�Ίp or ���Ίp�v�f�̐�(input)
	integer KB = (integer)this->BANDWIDTHL;						//�s��B�̏�Ίp or ���Ίp�v�f�̐�(input)
	std::vector<double> K = std::vector<double>(N * (KA + 1), 0.0);	//���ӌW���s��i�эs��j
	std::vector<double> F = std::vector<double>(N * (KB + 1), 0.0);	//�E�ӌW���s��i�эs��j
		
	for (auto pequation : this->pequations) {
		int Kei = 0;											//�v�f�\�ߓ_�������̍s�C���f�b�N�X
		for (auto pnodei : pequation->pelement->pnodes) {
			int Ki = this->Kmap[pnodei].first;					//�S�́\�ߓ_�������̍s�C���f�b�N�X
			for (auto usi : this->uf_to_us) {
				if (pnodei->us_to_un.find(usi) != pnodei->us_to_un.end()) {

					//***Dirichlet�������ۂ���Ă��Ȃ��Ƃ�***
					if (!pnodei->is_ufixed[pnodei->us_to_un[usi]]) {
						//.....�W���s��.....
						int Kej = 0;							//�v�f�\�ߓ_�������̗�C���f�b�N�X
						for (auto pnodej : pequation->pelement->pnodes) {
							int Kj = this->Kmap[pnodej].first;	//�S�́\�ߓ_�������̗�C���f�b�N�X
							for (auto usj : this->uf_to_us) {
								if (pnodej->us_to_un.find(usj) != pnodej->us_to_un.end()) {
									if (!pnodej->is_ufixed[pnodej->us_to_un[usj]]) {
										if (Ki <= Kj) {
											K[(KA + 1)*(Kj + 1) + Ki - Kj - 1] += pequation->Ke(Kei, Kej);
											F[(KB + 1)*(Kj + 1) + Ki - Kj - 1] += pequation->Fe(Kei, Kej);
										}
										Kj++;
									}
									Kej++;
								}
							}
						}
						Ki++;
					}
					Kei++;
				}
			}
		}
	}

	//----------�ŗL�l��������----------
	char JOBZ = 'V';											//�ŗL�x�N�g�������߂�ꍇ�i�ŗL�l�݂̂̏ꍇ�FJOBZ = 'N'�j(input)
	char RANGE = 'I';											//IL�Ԗڂ���IU�Ԗڂ̌ŗL�l�����߂�ꍇ(input)
	char UPLO = 'U';											//��O�p�v�f���g���ꍇ�i���O�p�v�f�FUPLO = 'L'�j(input)
	std::vector<double> Q = std::vector<double>(N * N, 0.0);	//3�d�Ίp���ɗp��������ϊ��s��iJOBZ = 'V'�̏ꍇ�̂݁j(output)
	double VL, VU;												//���߂����ŗL�l�̉����Ə��(input)
	integer IL, IU;												//���߂����ŗL�l�̃C���f�b�N�X�i�����j(input)
	double ABSTOL;												//�ŗL�l�����߂�ۂ̋��e��Ό덷(input)
																//�iABSTOL = 2 * _LAMCH('S') �Ƃ���ƍł����x���悢�j
	integer M;													//���߂�ꂽ�ŗL�l�̐�(output)
	std::vector<double> W = std::vector<double>(N);				//�ŗL�l�i�����j(output)
	std::vector<double> Z = std::vector<double>(N * N);			//�ŗL�x�N�g��(output)
	std::vector<double> WORK = std::vector<double>(7 * N);		//���[�N�X�y�[�X
	std::vector<integer> IWORK = std::vector<integer>(5 * N);	//���[�N�X�y�[�X
	std::vector<integer> IFAIL = std::vector<integer>(N);		//�ŗL�l�����߂��ۂ̎����Ɋւ�����(output)
	integer INFO;												//LAPACK�̃��[�`���̃G���[���(output)
	integer LDK = KA + 1;										//�s��AB��1�����ڂ̑傫���i = KA + 1�j
	integer LDF = KB + 1;										//�s��BB��1�����ڂ̑傫���i = KB + 1�j
	integer LDQ = N;											//�z��Q��1�����ڂ̑傫���i = N�j
	integer LDZ = N;											//�z��Z��1�����ڂ̑傫���i = N�j

	IL = 1;
	IU = 10;
	char DLAMCH = 'S';
	ABSTOL = dlamch_(&DLAMCH);
	dsbgvx_(&JOBZ, &RANGE, &UPLO, &N, &KA, &KB, K.data(), &LDK, F.data(), &LDF, Q.data(), &LDQ, &VL, &VU, &IL, &IU, &ABSTOL, &M, W.data(), Z.data(), &LDZ, WORK.data(), IWORK.data(), IFAIL.data(), &INFO);

	std::cout << INFO << "\t" << M << std::endl;

	for (int i = 0; i < 10; i++) {
		std::cout << W[i] << std::endl;
	}

	//----------���̑��----------
	for (auto& pnode : this->pnodes) {
		int ui = 0;
		for (auto us : this->uf_to_us) {
			if (pnode->us_to_un.find(us) != pnode->us_to_un.end()) {
				if (!pnode->is_ufixed[pnode->us_to_un[us]]) {
					pnode->u[pnode->us_to_un[us]] = Z[this->Kmap[pnode].first + ui + 2*N];
					ui++;
				}
			}
		}
	}
}