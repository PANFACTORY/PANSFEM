//*****************************************************************************
//Title		:PANSFEM/Core/Field/Field.h
//Author	:Tanabe Yuta
//Date		:2019/06/08
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "Field.h"


//**********CLAPACKおよびOpenBLASの設定**********
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
	//----------uf_to_usとpequationを設定----------
	this->uf_to_us = _ulist;
	this->pequations = _pequations;
	
	//----------場に属する節点を洗い出す----------
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
	//----------全体―節点方程式のアセンブリング----------
	integer N = (integer)this->KDEGREE;			//方程式の数
	integer KL = (integer)this->BANDWIDTHL;		//帯行列の下側幅（対角は除く）
	integer KU = (integer)this->BANDWIDTHU;		//帯行列の上側幅（対角は除く）
	integer NB = 2 * KL + KU + 1;
	integer NRHS = 1;							//係数ベクトルの数

	std::vector<double> K = std::vector<double>(NB * N, 0.0);		//係数行列（帯行列）
	std::vector<double> F = std::vector<double>(N * NRHS, 0.0);		//係数ベクトル

	for (auto pequation : this->pequations) {
		int Kei = 0;							//要素―節点方程式の行インデックス
		for (auto pnodei : pequation->pelement->pnodes) {
			for (int i = 0; i < this->Kmap[pnodei].second; i++) {
				int Kej = 0;					//要素―節点方程式の列インデックス
				int Ki = this->Kmap[pnodei].first + i;				//全体―節点方程式の行インデックス
				//.....係数行列.....
				for (auto pnodej : pequation->pelement->pnodes) {
					for (int j = 0; j < this->Kmap[pnodej].second; j++) {
						int Kj = this->Kmap[pnodej].first + j;		//全体―節点方程式の列インデックス
						K[Kj*NB + (KL + KU + Ki - Kj)] += pequation->Ke(Kei, Kej);
						Kej++;
					}
				}

				//.....係数ベクトル.....
				F[Ki] += pequation->Fe[Kei];
				Kei++;
			}
		}
	}
	
	//----------境界条件の適用----------
	for (auto pdirichlet : this->pdirichlets) {

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
		for (auto i : this->uf_to_us) {
			auto iu = std::find(pnode->ulist.begin(), pnode->ulist.end(), i);
			if (iu != pnode->ulist.end()) {
				pnode->u(iu - pnode->ulist.begin()) = F[iresult];
				iresult++;
			}
		}
	}
}