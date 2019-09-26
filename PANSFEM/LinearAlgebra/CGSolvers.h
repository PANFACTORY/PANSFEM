//*****************************************************************************
//Title		:LinearAlgebra/CGSolvers.h
//Author	:Tanabe Yuta
//Date		:2019/09/09
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include <numeric>
#include <chrono>
#include "CSR.h"


//********************�x�N�g���a(a+beta*b)*******************
template<class T>
inline std::vector<T> add(std::vector<T> &_a, T _beta, std::vector<T> &_b) {
	std::vector<T> v(_a.size());
	auto ai = _a.begin(), bi = _b.begin();
	for (auto &vi : v) {
		vi = (*ai) + _beta * (*bi);
		++ai;
		++bi;
	}
	return v;
}


//********************�x�N�g���a(a+beta*b+ganma*c)********************
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


//********************�x�N�g���a��(a+beta(b-ganma*c))********************
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


//********************�x�N�g����(a-b)********************
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


//********************�x�N�g����(a-beta*b)*******************
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


//********************CG�@�̃��C�����[�`��********************
template<class T>
std::vector<T> CG(CSR<T> &_A, std::vector<T> &_b, int _itrmax, T _eps) {
	//----------������----------
	std::vector<T> xk(_b.size(), T());
	std::vector<T> rk = subtract(_b, _A*xk);
	std::vector<T> pk = rk;
	T bnorm = std::inner_product(_b.begin(), _b.end(), _b.begin(), T());

	//----------�����v�Z----------
	for (int k = 0; k < _itrmax; ++k) {
		std::vector<T> Apk = _A * pk;
		T alpha = std::inner_product(rk.begin(), rk.end(), rk.begin(), T()) / std::inner_product(pk.begin(), pk.end(), Apk.begin(), T());
		std::vector<T> xkp1 = add(xk, alpha, pk);
		std::vector<T> rkp1 = subtract(rk, alpha, Apk);
		T beta = std::inner_product(rkp1.begin(), rkp1.end(), rkp1.begin(), T()) / std::inner_product(rk.begin(), rk.end(), rk.begin(), T());
		std::vector<T> pkp1 = add(rkp1, beta, pk);

		//----------�l�̍X�V----------
		xk = xkp1;
		rk = rkp1;
		pk = pkp1;

		//----------��������----------
		T rnorm = std::inner_product(rk.begin(), rk.end(), rk.begin(), T());
		std::cout << "k = " << k << "\teps = " << rnorm / bnorm << std::endl;
		if (rnorm < _eps*bnorm) {
			std::cout << "\tConvergence:" << k << std::endl;
			return xk;
		}
	}

	std::cout << "\nConvergence:faild" << std::endl;
	return xk;
}


//********************BiCGSTAB�@�̃��C�����[�`��********************
template<class T>
std::vector<T> BiCGSTAB(CSR<T> &_A, std::vector<T> &_b, int _itrmax, T _eps) {
	//----------������----------
	std::vector<T> xk(_b.size(), T());
	std::vector<T> rk = subtract(_b, _A*xk);
	std::vector<T> rdash = rk;
	std::vector<T> pk = rk;
	T bnorm = std::inner_product(_b.begin(), _b.end(), _b.begin(), T());

	//----------�����v�Z----------
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

		//----------xk�Crk, pk�̍X�V----------
		xk = xkp1;
		rk = rkp1;
		pk = pkp1;

		//----------��������----------
		T rnorm = std::inner_product(rk.begin(), rk.end(), rk.begin(), T());
		std::cout << "k = " << k << "\teps = " << rnorm / bnorm << std::endl;
		if (rnorm < _eps*bnorm) {
			std::cout << "\tConvergence:" << k << std::endl;
			return xk;
		}
	}

	std::cout << "\nConvergence:faild" << std::endl;
	return xk;
}


//********************�s���SLU(0)����********************
template<class T>
CSR<T> ILU0(CSR<T> &_A) {
	PreCSR<T> q = PreCSR<T>(_A);

	for (int i = 0, iend = _A.ROWS; i < iend; i++) {
		for (int n = _A.indptr[i], nend = _A.indptr[i + 1]; n < nend; n++) {
			int j = _A.indices[n];
			T qij = _A.data[n];
			for (int k = 0, kend = _A.ROWS; k < kend; k++) {
				if (std::binary_search(_A.indices.begin() + _A.indptr[i], _A.indices.begin() + _A.indptr[i + 1], k)) {
					if (std::binary_search(_A.indices.begin() + _A.indptr[k], _A.indices.begin() + _A.indptr[k + 1], j)) {
						if ((i <= j && k < i) || (i > j && k < j)) {
							qij -= q.get(i, k)*q.get(k, j);
						}
						else {
							break;
						}
					}
				}
			}
			if (i > j) {
				qij /= q.get(j, j);
			}
			q.set(i, j, qij);
		}
	}

	return CSR<T>(q);
}


//********************ILU(0)�O����*******************
template<class T>
std::vector<T> PreILU0(CSR<T> &_A, std::vector<T> &_b) {
	//�O�i�����iLy=b�������j
	std::vector<T> v = std::vector<T>(_b);
	for (int i = 0; i < _b.size(); i++) {
		for (int k = _A.indptr[i]; k < _A.indptr[i + 1]; k++) {
			if (_A.indices[k] < i) {
				v[i] -= _A.data[k] * v[_A.indices[k]];
			}
			else {
				break;
			}
		}
	}

	//��ޑ���iUx=y�������j
	for (int i = _b.size() - 1; i >= 0; i--) {
		for (int k = _A.indptr[i + 1] - 1; k >= _A.indptr[i]; k--) {
			if (_A.indices[k] > i) {
				v[i] -= _A.data[k] * v[_A.indices[k]];
			}
			else {
				break;
			}
		}
		v[i] /= _A.get(i, i);
	}

	return v;
}


//*******************ILU(0)����O�����t��CG�@********************
template<class T>
std::vector<T> ILU0CG(CSR<T> &_A, CSR<T> &_M, std::vector<T> &_b, int _itrmax, T _eps) {
	//----------������----------
	std::vector<T> xk(_b.size(), T());
	std::vector<T> rk = subtract(_b, _A*xk);
	std::vector<T> pk = PreILU0(_M, rk);
	std::vector<T> Mrk = pk;							//�O����
	T Mrkdotrk = std::inner_product(Mrk.begin(), Mrk.end(), rk.begin(), T());
	T bnorm = std::inner_product(_b.begin(), _b.end(), _b.begin(), T());

	//----------�����v�Z----------
	for (int k = 0; k < _itrmax; ++k) {
		std::vector<T> Apk = _A * pk;

		T alpha = Mrkdotrk / std::inner_product(pk.begin(), pk.end(), Apk.begin(), T());
		std::vector<T> xkp1 = add(xk, alpha, pk);
		std::vector<T> rkp1 = subtract(rk, alpha, Apk);

		std::vector<T> Mrkp1 = PreILU0(_M, rkp1);		//�O����

		T Mrkp1dotrkp1 = std::inner_product(Mrkp1.begin(), Mrkp1.end(), rkp1.begin(), T());
		T beta = Mrkp1dotrkp1 / Mrkdotrk;
		std::vector<T> pkp1 = add(Mrkp1, beta, pk);

		//----------�l�̍X�V----------
		xk = xkp1;
		rk = rkp1;
		pk = pkp1;
		Mrk = Mrkp1;
		Mrkdotrk = Mrkp1dotrkp1;

		//----------��������----------
		T rnorm = std::inner_product(rk.begin(), rk.end(), rk.begin(), T());
		std::cout << "k = " << k << "\teps = " << rnorm / bnorm << std::endl;
		if (rnorm < _eps*bnorm) {
			std::cout << "\tConvergence:" << k << std::endl;
			return xk;
		}
	}

	std::cout << "\nConvergence:faild" << std::endl;
	return xk;
}


//*******************ILU(0)����O�����t��BiCGSTAB�@*******************
template<class T>
std::vector<T> ILU0BiCGSTAB(CSR<T> &_A, CSR<T> &_M, std::vector<T> &_b, int _itrmax, T _eps) {
	//----------������----------
	std::vector<T> xk(_b.size(), T());
	std::vector<T> rk = subtract(_b, _A*xk);
	std::vector<T> rdash = rk;
	std::vector<T> pk = rk;
	T bnorm = std::inner_product(_b.begin(), _b.end(), _b.begin(), T());

	//----------�����v�Z----------
	for (int k = 0; k < _itrmax; ++k) {
		std::vector<T> Mpk = PreILU0(_M, pk);		//�O����

		std::vector<T> AMpk = _A * Mpk;
		T rdashdotrk = std::inner_product(rdash.begin(), rdash.end(), rk.begin(), T());
		T alpha = rdashdotrk / std::inner_product(rdash.begin(), rdash.end(), AMpk.begin(), T());
		std::vector<T> sk = subtract(rk, alpha, AMpk);

		std::vector<T> Msk = PreILU0(_M, sk);		//�O����

		std::vector<T> AMsk = _A * Msk;
		T omega = std::inner_product(AMsk.begin(), AMsk.end(), sk.begin(), T()) / std::inner_product(AMsk.begin(), AMsk.end(), AMsk.begin(), T());
		std::vector<T> xkp1 = add(xk, alpha, Mpk, omega, Msk);
		std::vector<T> rkp1 = subtract(sk, omega, AMsk);
		T beta = alpha / omega * std::inner_product(rdash.begin(), rdash.end(), rkp1.begin(), T()) / rdashdotrk;
		std::vector<T> pkp1 = addsubstract(rk, beta, pk, omega, AMpk);

		//----------xk�Crk, pk�̍X�V----------
		xk = xkp1;
		rk = rkp1;
		pk = pkp1;

		//----------��������----------
		T rnorm = std::inner_product(rk.begin(), rk.end(), rk.begin(), T());
		if (rnorm < _eps*bnorm) {
			std::cout << "\tConvergence:" << k << std::endl;
			return xk;
		}
	}

	std::cout << "\nConvergence:faild" << std::endl;
	return xk;
}


//********************SOR�@********************
template<class T>
std::vector<T> SOR(CSR<T> &_A, std::vector<T> &_b, T _w, int _itrmax, T _eps) {
	std::vector<T> x = std::vector<T>(_A.ROWS, T());
	T error = T();
	for (int itr = 0; itr < _itrmax; itr++) {
		error = T();
		for (int i = 0; i < _A.ROWS; i++) {
			T Aii = T();
			T tmp = x[i];
			x[i] = _b[i];
//#pragma omp parallel for
			for (int k = _A.indptr[i]; k < _A.indptr[i + 1]; k++) {
				int j = _A.indices[k];
				if (i != j) {
					x[i] -= _A.data[k] * x[j];
				}
				else {
					Aii = _A.data[k];
				}
			}
			x[i] /= Aii;
			x[i] = tmp + _w * (x[i] - tmp);

			error += fabs((tmp - x[i]) / tmp);
		}
		if (error < _eps) {
			//std::cout << "\tConvergence:" << itr << std::endl;
			return x;
		}
	}

	//std::cout << "\t" << error << std::endl;
	return x;
}


//********************�Ίp�s��i�x�N�g���`���j�̎擾********************
template<class T>
std::vector<T> GetScaling(CSR<T>& _A) {
	std::vector<T> v(_A.ROWS);
	for (int i = 0; i < _A.ROWS; i++) {
		v[i] = _A.get(i, i);
	}
	return v;
}


//********************�Ίp�X�P�[�����O********************
template<class T>
std::vector<T> Scaling(std::vector<T>& _D, std::vector<T>& _b) {
	std::vector<T> v(_D.size());
	for (int i = 0; i < _D.size(); i++) {
		v[i] = _b[i] / _D[i];
	}
	return v;
}


//********************�Ίp�X�P�[�����O�O�����t��CG�@********************
template<class T>
std::vector<T> ScalingCG(CSR<T> &_A, std::vector<T> &_b, int _itrmax, T _eps) {
	//----------������----------
	std::vector<T> D = GetScaling(_A);					//�O�����p�Ίp�s��
	std::vector<T> xk(_b.size(), T());
	std::vector<T> rk = subtract(_b, _A*xk);
	std::vector<T> pk = Scaling(D, rk);				//�O����
	T bnorm = std::inner_product(_b.begin(), _b.end(), _b.begin(), T());

	std::vector<T> Mrk = Scaling(D, rk);			//�O����
	T Mrkdotrk = std::inner_product(Mrk.begin(), Mrk.end(), rk.begin(), T());

	//----------�����v�Z----------
	for (int k = 0; k < _itrmax; ++k) {
		std::vector<T> Apk = _A * pk;
		T alpha = Mrkdotrk / std::inner_product(pk.begin(), pk.end(), Apk.begin(), T());
		std::vector<T> xkp1 = add(xk, alpha, pk);
		std::vector<T> rkp1 = subtract(rk, alpha, Apk);
		std::vector<T> Mrkp1 = Scaling(D, rkp1);	//�O����
		T Mrkp1dotrkp1 = std::inner_product(Mrkp1.begin(), Mrkp1.end(), rkp1.begin(), T());
		T beta = Mrkp1dotrkp1 / Mrkdotrk;
		std::vector<T> pkp1 = add(Mrkp1, beta, pk);

		//----------�l�̍X�V----------
		xk = xkp1;
		rk = rkp1;
		pk = pkp1;
		Mrk = Mrkp1;
		Mrkdotrk = Mrkp1dotrkp1;

		//----------��������----------
		T rnorm = std::inner_product(rk.begin(), rk.end(), rk.begin(), T());
		std::cout << "k = " << k << "\teps = " << rnorm / bnorm << std::endl;
		if (rnorm < _eps*bnorm) {
			std::cout << "\tConvergence:" << k << std::endl;
			return xk;
		}
	}

	std::cout << "\nConvergence:faild" << std::endl;
	return xk;
}


//********************SOR�@�O�����t��CG�@********************
template<class T>
std::vector<T> SORCG(CSR<T> &_A, std::vector<T> &_b, int _itrmax, T _eps, T _omega) {
	//----------������----------
	std::vector<T> xk(_b.size(), T());
	std::vector<T> rk = subtract(_b, _A*xk);
	std::vector<T> pk = SOR(_A, rk, _omega, 50, 1.0e-10);				//�O����
	T bnorm = std::inner_product(_b.begin(), _b.end(), _b.begin(), T());

	std::vector<T> Mrk = SOR(_A, rk, _omega, 50, 1.0e-10);				//�O����
	T Mrkdotrk = std::inner_product(Mrk.begin(), Mrk.end(), rk.begin(), T());

	//----------�����v�Z----------
	for (int k = 0; k < _itrmax; ++k) {
		std::vector<T> Apk = _A * pk;
		T alpha = Mrkdotrk / std::inner_product(pk.begin(), pk.end(), Apk.begin(), T());
		std::vector<T> xkp1 = add(xk, alpha, pk);
		std::vector<T> rkp1 = subtract(rk, alpha, Apk);
		std::vector<T> Mrkp1 = SOR(_A, rkp1, _omega, 50, 1.0e-10);		//�O����
		T Mrkp1dotrkp1 = std::inner_product(Mrkp1.begin(), Mrkp1.end(), rkp1.begin(), T());
		T beta = Mrkp1dotrkp1 / Mrkdotrk;
		std::vector<T> pkp1 = add(Mrkp1, beta, pk);

		//----------�l�̍X�V----------
		xk = xkp1;
		rk = rkp1;
		pk = pkp1;
		Mrk = Mrkp1;
		Mrkdotrk = Mrkp1dotrkp1;

		//----------��������----------
		T rnorm = std::inner_product(rk.begin(), rk.end(), rk.begin(), T());
		std::cout << "k = " << k << "\teps = " << rnorm / bnorm << std::endl;
		if (rnorm < _eps*bnorm) {
			std::cout << "\tConvergence:" << k << std::endl;
			return xk;
		}
	}

	std::cout << "\nConvergence:faild" << std::endl;
	return xk;
}