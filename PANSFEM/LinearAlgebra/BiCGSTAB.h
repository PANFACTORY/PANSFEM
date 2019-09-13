//*****************************************************************************
//Title		:BiCGSTAB.h
//Author	:Tanabe Yuta
//Date		:2019/09/09
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include <numeric>
#include "CSR.h"


//********************ベクトル和(a+beta*b+ganma*c)********************
template<class T>
inline std::vector<T> add(std::vector<T> &_a, T _beta, std::vector<T> &_b, T _ganma, std::vector<T> &_c) {
	std::vector<T> v(_a.size());
	auto ai = _a.begin(), bi = _b.begin(), ci = _c.begin();
	for (auto &vi : v) {
		vi = (*ai) + _beta * (*bi) + _ganma * (*ci);
		++ai;
		++bi;
		++ci;
	}
	return v;
}


//********************ベクトル和差(a+beta(b-ganma*c))********************
template<class T>
inline std::vector<T> addsubstract(std::vector<T> &_a, T _beta, std::vector<T> &_b, T _ganma, std::vector<T> &_c) {
	std::vector<T> v(_a.size());
	auto ai = _a.begin(), bi = _b.begin(), ci = _c.begin();
	for (auto &vi : v) {
		vi = (*ai) + _beta * ((*bi) - _ganma * (*ci));
		++ai;
		++bi;
		++ci;
	}
	return v;
}


//********************ベクトル差(a-b)********************
template<class T>
inline std::vector<T> subtract(std::vector<T> _a, std::vector<T> _b) {
	std::vector<T> v(_b.size());
	auto ai = _a.begin(), bi = _b.begin();
	for (auto &vi : v) {
		vi = (*ai) - (*bi);
		++ai;
		++bi;
	}
	return v;
}


//********************ベクトル差(a-beta*b)*******************
template<class T>
inline std::vector<T> subtract(std::vector<T> &_a, T _beta, std::vector<T> &_b) {
	std::vector<T> v(_a.size());
	auto ai = _a.begin(), bi = _b.begin();
	for (auto &vi : v) {
		vi = (*ai) - _beta * (*bi);
		++ai;
		++bi;
	}
	return v;
}


//********************BiCGSTAB法のメインルーチン********************
template<class T>
std::vector<T> BiCGSTAB(CSR<T> &_A, std::vector<T> &_b, int _itrmax, T _eps) {
	//----------初期化----------
	std::vector<T> xk(_b.size(), T());
	std::vector<T> rk = subtract(_b, _A*xk);
	std::vector<T> rdash = rk;
	std::vector<T> pk = rk;
	T bnorm = std::inner_product(_b.begin(), _b.end(), _b.begin(), T());

	//----------反復計算----------
	for (int k = 0; k < _itrmax; ++k) {
		std::vector<T> Apk = _A * pk;
		T rdashdotrk = std::inner_product(rdash.begin(), rdash.end(), rk.begin(), T());
		T alpha = rdashdotrk / std::inner_product(rdash.begin(), rdash.end(), Apk.begin(), T());
		std::vector<T> sk = subtract(rk, alpha, Apk);
		std::vector<T> Ask = _A * sk;
		T omega = std::inner_product(Ask.begin(), Ask.end(), sk.begin(), T()) / std::inner_product(Ask.begin(), Ask.end(), Ask.begin(), T());
		std::vector<T> xkp1 = add(xk, alpha, pk, omega, sk);
		std::vector<T> rkp1 = subtract(sk, omega, Ask);
		T beta = alpha / omega * std::inner_product(rdash.begin(), rdash.end(), rkp1.begin(), T()) / rdashdotrk;
		std::vector<T> pkp1 = addsubstract(rk, beta, pk, omega, Apk);

		//----------xk，rk, pkの更新----------
		xk = xkp1;
		rk = rkp1;
		pk = pkp1;

		//----------収束判定----------
		T rnorm = std::inner_product(rk.begin(), rk.end(), rk.begin(), T());
		if (rnorm < _eps*bnorm) {
			std::cout << "\tConvergence:" << k << std::endl;
			return xk;
		}
	}

	std::cout << "\nConvergence:faild" << std::endl;
	return xk;
}