//*****************************************************************************
//Title		:PANSFEM/Core/Field/Field.h
//Author	:Tanabe Yuta
//Date		:2019/06/08
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include <algorithm>
#include <numeric>


#include "Field.h"



PANSFEM::Field::Field(){}


PANSFEM::Field::~Field(){
	for (auto& pequation : this->pequations) {
		delete pequation;
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

	//----------•‚ªL‚¢‡‚ÉÀ•WŽ²”Ô†‚ð•À‚×‚é----------
	int DOX = this->pnodes[0]->x.rows();
	std::vector<double> xmax = std::vector<double>(DOX, 0.0);
	std::vector<double> xmin = std::vector<double>(DOX, 0.0);
	for (auto pnode : this->pnodes) {
		for (int i = 0; i < DOX; i++) {
			if (xmax[i] < pnode->x(i)) {
				xmax[i] = pnode->x(i);
			}
			if(xmin[i] > pnode->x(i)) {
				xmin[i] = pnode->x(i);
			}
		}
	}
	std::vector<double> xrange = std::vector<double>(DOX);
	for (int i = 0; i < DOX; i++) {
		xrange[i] = xmax[i] - xmin[i];
	}
	std::vector<size_t> index(DOX);
	std::iota(index.begin(), index.end(), 0);
	std::sort(index.begin(), index.end(), [&xrange](size_t i1, size_t i2) {
		return xrange[i1] > xrange[i2];
	});
	/*
	std::vector<Node*> sortedpnodes = this->pnodes;
	std::sort(sortedpnodes.begin(), sortedpnodes.end(),	[](const Node* pnode0, const Node* pnode1) {
		if (fabs(pnode0->x[2] - pnode1->x[2]) < 1.0e-8) {
			return pnode0->x[1] > pnode1->x[1];
		}
		return pnode0->x[2] > pnode1->x[2];
	});
	*/	
	std::vector<Node*> sortedpnodes = this->pnodes;
	std::sort(sortedpnodes.begin(), sortedpnodes.end(), [&DOX, &index](const Node* pnode0, const Node* pnode1) {
		for (auto i : index) {
			if (fabs(pnode0->x[i] - pnode1->x[i]) > 1.0e-8) {
				return pnode0->x[i] < pnode1->x[i];
			}
		}
		return pnode0->x[index[DOX-1]] < pnode1->x[index[DOX-1]];
	});

	for (auto pnode : sortedpnodes) {
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