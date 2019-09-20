//*****************************************************************************
//Title		:PreCSR.h
//Author	:Tanabe Yuta
//Date		:2019/09/13
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include <iostream>
#include <vector>
#include <algorithm>


#include "CSR.h"


template<class T>
class CSR;


template<class T>
class PreCSR
{
public:
	PreCSR();
	~PreCSR();
	PreCSR(int _rows, int _cols);
	PreCSR(CSR<T> _matrix);						//CSRからPreCSRを生成


	const int ROWS;			//行数
	const int COLS;			//列数


	const std::vector<T> operator*(const std::vector<T> &_vec);		//ベクトルとの積
	template<class F>
	friend std::ostream& operator << (std::ostream &_out, const PreCSR<F> &_mat);	//streamに出力


	bool set(int _row, int _col, T _data);		//値のセット
	T get(int _row, int _col) const;			//値の取得


	template<class F>
	friend class CSR;


private:
	std::vector<std::vector<std::pair<int, T> > > data;
};


template<class T>
inline PreCSR<T>::PreCSR() : ROWS(0), COLS(0) {}


template<class T>
inline PreCSR<T>::~PreCSR() {}


template<class T>
inline PreCSR<T>::PreCSR(int _rows, int _cols) : ROWS(_rows), COLS(_cols) {
	this->data = std::vector<std::vector<std::pair<int, T> > >(_rows);
}


template<class T>
inline PreCSR<T>::PreCSR(CSR<T> _matrix) : ROWS(_matrix.ROWS), COLS(_matrix.COLS) {
	this->data = std::vector<std::vector<std::pair<int, T> > >(_matrix.ROWS);
	for (int i = 0; i < _matrix.ROWS; i++) {
		for (int k = _matrix.indptr[i]; k < _matrix.indptr[i + 1]; k++) {
			this->data[i].push_back(std::pair<int, T>(_matrix.indices[k], _matrix.data[k]));
		}
	}
}


template<class T>
inline const std::vector<T> PreCSR<T>::operator*(const std::vector<T>& _vec) {
	std::vector<T> v(this->ROWS, T());

	//#pragma omp parallel for 
	for (int i = 0; i < this->ROWS; i++) {
		for (auto dataj : this->data[i]) {
			v[i] += dataj.second * _vec[dataj.first];
		}
	}

	return v;
}


template<class T>
inline bool PreCSR<T>::set(int _row, int _col, T _data) {
	for (auto& dataj : this->data[_row]) {
		if (dataj.first == _col) {
			dataj.second = _data;
			return true;
		}
	}

	this->data[_row].push_back(std::pair<int, T>(_col, _data));
	return false;
}


template<class T>
inline T PreCSR<T>::get(int _row, int _col) const {
	for (const auto& dataj : this->data[_row]) {
		if (dataj.first == _col) {
			return dataj.second;
		}
	}

	return T();
}


template<class F>
inline std::ostream & operator<<(std::ostream & _out, const PreCSR<F>& _mat) {
	for (int i = 0; i < _mat.ROWS; i++) {
		for (int j = 0; j < _mat.COLS; j++) {
			_out << _mat.get(i, j) << "\t";
		}
		_out << std::endl;
	}

	return _out;
}