//*****************************************************************************
//Title		:PANSFEM/DirectAnalysis/Field/EigenvalueField/EigenvalueField.cpp
//Author	:Tanabe Yuta
//Date		:2019/07/16
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "EigenvalueField.h"


//**********CLAPACKおよびOpenBLASの設定**********
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
	//----------全体―節点方程式のアセンブリング----------
	char JOBZ = 'V';	//固有ベクトルも求める場合（固有値のみの場合：JOBZ = 'N'）(input)
	char RANGE = 'I';	//IL番目からIU番目の固有値を求める場合(input)
	char UPLO = 'U';	//上三角要素を使う場合（下三角要素：UPLO = 'L'）(input)
	integer N = (integer)this->KDEGREE;			//方程式の数
	integer NA = (integer)this->BANDWIDTHL;		//行列Aの上対角 or 下対角要素の数(input)
	integer NB = (integer)this->BANDWIDTHL;		//行列Bの上対角 or 下対角要素の数(input)
	std::vector<double> K = std::vector<double>(N * NA, 0.0);		//左辺係数行列（帯行列）
	std::vector<double> F = std::vector<double>(N * NB, 0.0);		//右辺係数行列（帯行列）
	std::vector<double> Q = std::vector<double>(N * N, 0.0);		//3重対角化に用いた直交変換行列（JOBZ = 'V'の場合のみ）(output)
	double VL, VU;		//求めたい固有値の下限と上限(input)
	integer IL, IU;		//求めたい固有値のインデックス（昇順）(input)
	double ABSTOL;		//固有値を求める際の許容絶対誤差(input)
						//（ABSTOL = 2 * _LAMCH('S') とすると最も精度がよい）
	integer M;			//求められた固有値の数(output)
	std::vector<double> W = std::vector<double>(N);					//固有値（昇順）(output)
	std::vector<double> Z = std::vector<double>(N * N);				//固有ベクトル(output)
	std::vector<double> WORK = std::vector<double>(7 * N);			//ワークスペース
	std::vector<integer> IWORK = std::vector<integer>(5 * N);		//ワークスペース
	std::vector<integer> IFAIL = std::vector<integer>(N);			//固有値を求めた際の収束に関する情報(output)
	integer INFO;		//LAPACKのルーチンのエラー情報(output)

	//integer, parameter::LDAA = size(A, 1) !行列ABの1次元目の大きさ（ = KA + 1）
	//integer, parameter::LDBB = size(B, 1) !行列BBの1次元目の大きさ（ = KB + 1）
	//integer, parameter::LDQ = size(Q, 1)  !配列Zの1次元目の大きさ（ = N）
	//integer, parameter::LDZ = size(Z, 1)  !配列Zの1次元目の大きさ（ = N）

	//.....要素―節点方程式のアセンブリング.....
	for (auto pequation : this->pequations) {
		int Kei = 0;												//要素―節点方程式の行インデックス
		for (auto pnodei : pequation->pelement->pnodes) {
			int Ki = this->Kmap[pnodei].first;						//全体―節点方程式の行インデックス
			for (auto usi : this->uf_to_us) {
				if (pnodei->us_to_un.find(usi) != pnodei->us_to_un.end()) {

					//***Dirichlet条件が課されていないとき***
					if (!pnodei->is_ufixed[pnodei->us_to_un[usi]]) {
						//.....係数行列.....
						int Kej = 0;								//要素―節点方程式の列インデックス
						for (auto pnodej : pequation->pelement->pnodes) {
							int Kj = this->Kmap[pnodej].first;		//全体―節点方程式の列インデックス
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

					//***Dirichlet条件が課されているとき***					
					else {
						int Kej = 0;								//要素―節点方程式の列インデックス
						for (auto pnodej : pequation->pelement->pnodes) {
							int Kj = this->Kmap[pnodej].first;		//全体―節点方程式の列インデックス
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

	//----------連立方程式を解く----------
	integer LDK = (integer)NB;			//係数行列の寸法
	std::vector<integer> IPIV(N);		//ピボット位置記録用領域
	integer LDF = (integer)N;			//係数ベクトルの寸法
	integer INFO;						//エラー通知用
	dgbsv_(&N, &KL, &KU, &NRHS, K.data(), &LDK, IPIV.data(), F.data(), &LDF, &INFO);

	//----------解の代入----------
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