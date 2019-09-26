//*****************************************************************************
//Title		:PANSFEM/DirectAnalysis/Field/NonLinearField/NonLinearField.cpp
//Author	:Tanabe Yuta
//Date		:2019/06/12
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "NonLinearField.h"


//**********CLAPACKおよびOpenBLASの設定**********
extern "C" {
#include "f2c.h"
#include "clapack.h"
}
#pragma comment( lib, "libopenblas.dll.a" )
#pragma comment( lib, "libf2c.lib" )
#pragma comment( lib, "lapack.lib" )
//***********************************************


PANSFEM::NonLinearField::NonLinearField() : LinearField() {}


PANSFEM::NonLinearField::~NonLinearField() {}


PANSFEM::NonLinearField::NonLinearField(std::vector<int> _ulist) : LinearField(_ulist) {}


void PANSFEM::NonLinearField::SolveEquation() {
	//----------荷重増分のループ----------
	for (int incf = 0; incf < 1; incf++) {
		//----------荷重増分の設定----------
		//----------荷重更新----------
		//----------残差計算----------
		//----------Newton-Raphson法のループ----------
		for(int i = 0; i < 100; i++){
			//----------要素―節点方程式の計算----------
			for (auto& pequation : this->pequations) {
				pequation->SetEquation();
			}

			//----------全体―節点方程式のアセンブリング----------
			integer N = (integer)this->KDEGREE;			//方程式の数
			integer KL = (integer)this->BANDWIDTHL;		//帯行列の下側幅（対角は除く）
			integer KU = (integer)this->BANDWIDTHU;		//帯行列の上側幅（対角は除く）
			integer NB = 2 * KL + KU + 1;
			integer NRHS = 1;							//係数ベクトルの数

			std::vector<double> K = std::vector<double>(NB * N, 0.0);		//係数行列（帯行列）
			std::vector<double> F = std::vector<double>(N, 0.0);			//係数ベクトル

			//.....Neumann境界条件の設定.....
			for (auto pnode : this->pnodes) {
				int Ki = this->Kmap[pnode].first;																	//全体―節点方程式の行インデックス
				for (auto usi : this->uf_to_us) {
					if (pnode->us_to_un.find(usi) != pnode->us_to_un.end()) {										//節点で定義されているか
						if (!pnode->is_ufixed[pnode->us_to_un[usi]] && pnode->is_qfixed[pnode->us_to_un[usi]]) {	//節点でDirichlet境界条件が与えられておらずNeumann境界条件が与えられているか
							F[Ki] += pnode->q[usi];
							Ki++;
						}
					}
				}
			}

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
												Kj++;
											}
											Kej++;
										}
									}
								}
								//.....係数ベクトル.....
								F[Ki] += pequation->Fe(Kei, 0);
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





			//----------収束判定----------
			std::cout << F[3] << std::endl;
			




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
							pnode->u[pnode->us_to_un[us]] += F[this->Kmap[pnode].first + ui];
							ui++;
						}
					}
				}
			}
		}
	}
}