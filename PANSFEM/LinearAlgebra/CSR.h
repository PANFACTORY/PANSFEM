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



template<class T>
class CSR
{
public:
	CSR();
	~CSR();
	CSR(int _rows, int _cols);	//_rows�F�s���C_cols�F��


	const int ROWS;				//�s��
	const int COLS;				//��


	const std::vector<T> operator*(const std::vector<T> &_vec);					//�x�N�g���Ƃ̐�
	template<class F>
	friend std::ostream& operator << (std::ostream &_out, const CSR<F> &_mat);	//stream�ɏo��


	bool set(int _row, int _col, T _data);		//�l�̃Z�b�g
	T get(int _row, int _col) const;			//�l�̎擾


private:
	std::vector<int> indptr;	//indptr[i]<=j<indptr[i+1]��data[j],indices[j]��i�s�ڂ̗v�f�ł���i�T�C�Y�͍s��+1�j
	std::vector<int> indices;	//indices[j]��data[j]������ڂ̗v�f���������idata�̃T�C�Y�ɓ������j
	std::vector<T> data;		//�i�[�f�[�^�i��[���v�f�̐��ɓ������j
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
inline bool CSR<T>::set(int _row, int _col, T _data) {
	int j = this->indptr[_row];

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

	return false;
}


template<class T>
inline T CSR<T>::get(int _row, int _col) const {
	for (int j = this->indptr[_row], jend = this->indptr[_row + 1]; j < jend; j++) {
		if (this->indices[j] == _col) {
			return this->data[j];
		}
	}
	return T();
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