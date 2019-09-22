//*****************************************************************************
//Title		:CSR.h
//Author	:Tanabe Yuta
//Date		:2019/08/27
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <thread>
#include <omp.h>
#include "PreCSR.h"


template<class T>
class PreCSR;


template<class T>
class CSR
{
public:
	CSR();
	~CSR();
	CSR(int _rows, int _cols);	//_rows：行数，_cols：列数
	CSR(PreCSR<T> _matrix);		//PreCSRからCSRを生成


	const int ROWS;				//行数
	const int COLS;				//列数


	const std::vector<T> operator*(const std::vector<T> &_vec);			//ベクトルとの積
	const CSR<T> operator*(const CSR<T> &_mat);							//CSR行列との積
	template<class F>
	friend std::ostream& operator << (std::ostream &_out, const CSR<F> &_mat);	//streamに出力


	bool set(int _row, int _col, T _data);		//値のセット
	T get(int _row, int _col) const;			//値の取得


	template<class F>
	friend CSR<F> ILU0(CSR<F>& _A);				//不完全LU(0)分解
	template<class F>
	friend std::vector<F> PreILU0(CSR<F> &_A, std::vector<F> &_b);		//不完全LU(0)分解前処理
	template<class T>
	friend std::vector<T> SOR(CSR<T> &_A, std::vector<T> &_b, T _w, int _itrmax, T _eps);	//SOR法
	template<class T>
	friend void Scaling(CSR<T>& _A, std::vector<T>& _b);				//連立方程式のスケーリング


	template<class F>
	friend class PreCSR;


private:
	std::vector<int> indptr;	//indptr[i]<=j<indptr[i+1]のdata[j],indices[j]はi行目の要素である（サイズは行数+1）
	std::vector<int> indices;	//indices[j]はdata[j]が何列目の要素かを示す（dataのサイズに等しい）
	std::vector<T> data;		//格納データ（非ゼロ要素の数に等しい）
};


template<class T>
inline CSR<T>::CSR() : ROWS(0), COLS(0) {}


template<class T>
inline CSR<T>::~CSR() {}


template<class T>
inline CSR<T>::CSR(int _rows, int _cols) : ROWS(_rows), COLS(_cols) {
	this->indptr = std::vector<int>(this->ROWS + 1, 0);
}


template<class T>
inline CSR<T>::CSR(PreCSR<T> _matrix) : ROWS(_matrix.ROWS), COLS(_matrix.COLS) {
	this->indptr = std::vector<int>(this->ROWS + 1, 0);

	for (int i = 0; i < this->ROWS; i++) {
		this->indptr[i + 1] = this->indptr[i] + _matrix.data[i].size();

		std::sort(_matrix.data[i].begin(), _matrix.data[i].end());
		for (auto matrixj : _matrix.data[i]) {
			this->indices.push_back(matrixj.first);
			this->data.push_back(matrixj.second);
		}
	}
}


template<class T>
inline const std::vector<T> CSR<T>::operator*(const std::vector<T> &_vec) {
	std::vector<T> v(this->ROWS, T());

	int iend = this->ROWS;

#pragma omp parallel for
	for (int i = 0; i < iend; ++i) {
		for (int j = this->indptr[i], jend = this->indptr[i + 1]; j < jend; ++j) {
			v[i] += this->data[j] * _vec[this->indices[j]];
		}
	}

	return v;
}


template<class T>
inline const CSR<T> CSR<T>::operator*(const CSR<T>& _mat) {
	PreCSR<T> m = PreCSR<T>(this->ROWS, _mat.COLS);

	int iend = this->ROWS;
#pragma omp parallel for
	for (int i = 0; i < iend; i++) {
		for (int j = 0, jend = _mat.COLS; j < jend; j++) {
			T dataij = T();
			bool isset = false;
			for (int k = this->indptr[i], kend = this->indptr[i + 1]; k < kend; k++) {
				auto colbegin = _mat.indices.begin() + _mat.indptr[this->indices[k]], colend = _mat.indices.begin() + _mat.indptr[this->indices[k] + 1];
				auto datapos = std::find(colbegin, colend, j);
				if (datapos != colend) {
					dataij += this->data[k] * (*(_mat.data.begin() + std::distance(_mat.indices.begin(), datapos)));
					isset = true;
				}
			}
			if (isset) {
				m.set(i, j, dataij);
			}
		}
	}

	return CSR<T>(m);
}


template<class T>
inline bool CSR<T>::set(int _row, int _col, T _data) {
	/*int j = this->indptr[_row];

	for (int jend = this->indptr[_row + 1]; j < jend; ++j) {
		if (this->indices[j] == _col) {
			this->data[j] = _data;
			return true;
		}
		else if (this->indices[j] > _col) {
			break;
		}
	}

	this->data.insert(this->data.begin() + j, _data);
	this->indices.insert(this->indices.begin() + j, _col);
	for (auto i = this->indptr.begin() + (_row + 1), iend = this->indptr.end(); i != iend; ++i) {
		*i += 1;
	}
	*/

	auto colbegin = this->indices.begin() + this->indptr[_row], colend = this->indices.begin() + this->indptr[_row + 1];
	auto colnow = std::lower_bound(colbegin, colend, _col);
	if (colnow == colend) {
		this->data.insert(this->data.begin() + std::distance(this->indices.begin(), colnow), _data);
		this->indices.insert(colnow, _col);
		for (auto i = this->indptr.begin() + (_row + 1), iend = this->indptr.end(); i != iend; ++i) {
			*i += 1;
		}
		return false;
	}
	else if (*colnow == _col) {
		*(this->data.begin() + std::distance(this->indices.begin(), colnow)) = _data;
		return true;
	}
	else {
		this->data.insert(this->data.begin() + std::distance(this->indices.begin(), colnow), _data);
		this->indices.insert(colnow, _col);
		for (auto i = this->indptr.begin() + (_row + 1), iend = this->indptr.end(); i != iend; ++i) {
			*i += 1;
		}
		return false;
	}

	return false;
}


template<class T>
inline T CSR<T>::get(int _row, int _col) const {
	/*for (int j = this->indptr[_row], jend = this->indptr[_row + 1]; j < jend; j++) {
		if (this->indices[j] == _col) {
			return this->data[j];
		}
	}*/

	auto colbegin = this->indices.begin() + this->indptr[_row], colend = this->indices.begin() + this->indptr[_row + 1];
	auto colnow = std::lower_bound(colbegin, colend, _col);
	if (colnow == colend) {
		return T();
	}
	else if (*colnow == _col) {
		return *(this->data.begin() + std::distance(this->indices.begin(), colnow));
	}
	else {
		return T();
	}
}


template<class F>
inline std::ostream & operator<<(std::ostream & _out, const CSR<F>& _mat) {
	for (int i = 0; i < _mat.ROWS; i++) {
		for (int j = 0; j < _mat.COLS; j++) {
			_out << _mat.get(i, j) << "\t";
		}
		_out << std::endl;
	}

	return _out;
}