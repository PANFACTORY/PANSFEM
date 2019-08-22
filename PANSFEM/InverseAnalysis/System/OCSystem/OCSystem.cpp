//*****************************************************************************
//Title		:PANSFEM/InversedAnalysis/System/OCSystem/OCSystem.cpp
//Author	:Tanabe Yuta
//Date		:2019/08/10
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "OCSystem.h"


PANSFEM::OCSystem::OCSystem() : OptimizedSystem(){}


PANSFEM::OCSystem::~OCSystem(){}


PANSFEM::OCSystem::OCSystem(int _DOX, int _DOU, std::vector<int> _plist) : OptimizedSystem(_DOX, _DOU, _plist){}


void PANSFEM::OCSystem::Schedule(){
	//----------順解析の設定----------
	for (auto pfield : this->pfields) {
		//.....場の初期化.....
		pfield->Initialize();

		//.....Kmapの作成.....
		pfield->MakeKmap();

		//.....バンド幅の計算.....
		pfield->GetBandwidth();
	}

	//----------設計変数の初期化----------
	const int rholen = this->poptimizedelements.size() * this->plist.size();	//設計変数ベクトルの要素数
	const int iterationmax = 100;				//最適化ループの最大反復数
	const double valueconvergence = 1.0e-5;		//目的関数の収束判定値
	const double lambdaconvergence = 1.0e-9;	//Lagrange乗数λの収束判定値
	const double mvlmt = 0.15;					//ムーブリミット
	const double iota = 0.75;					//ダンピング係数

	//----------最適化の反復計算----------
	double previousvalue = 0.0;		//1ステップ前の目的関数の値
	for (int itr = 0; itr < iterationmax; itr++) {
		std::cout << "Iteration:" << itr << std::endl;

		//----------順解析----------
		std::cout << "\t" << "Solution";
		for (auto pfield : this->pfields) {
			//.....全体―節点方程式を解く.....
			pfield->SolveEquation();
		}
		std::cout << " is done." << std::endl;
		this->Export("Data/Output/a");


		//----------目的関数，制約条件の計算----------
		double currentvalue = this->pobjectives[0]->value();
		std::cout << "\t" << "Objective function value:" << currentvalue << std::endl;
		double constraintvalue = this->pconstraints[0]->value();
		std::cout << "\t" << "Constraint function value:" << constraintvalue << std::endl;

		//----------収束判定----------
		if (fabs((currentvalue - previousvalue) / (currentvalue + previousvalue)) < valueconvergence) {
			std::cout << "----------System is optimized----------" << std::endl;
			break;
		}

		//----------設計感度を計算----------
		Eigen::VectorXd objectivesensitivity = this->pobjectives[0]->sensitivitis();
		Eigen::VectorXd constraintsensitivity = this->pconstraints[0]->sensitivitis();

		//----------現在の設計変数ベクトルを生成----------
		Eigen::VectorXd rho = Eigen::VectorXd(rholen);
		int pos = 0;
		for (auto pelement : this->poptimizedelements) {
			for (auto pi : this->plist) {
				rho(pos) = pelement->parameters[pi];
				pos++;
			}
		}

		//----------Lagrange乗数を二分探索----------
		double lambda0 = 1.0e-10, lambda1 = 1.0e10;
		while ((lambda1 - lambda0) / (lambda1 + lambda0) > lambdaconvergence) {
			double lambda = 0.5 * (lambda1 + lambda0);
			Eigen::VectorXd B = pow((-objectivesensitivity.array() / constraintsensitivity.array()).array() / lambda, iota).array()*rho.array();
			Eigen::VectorXd rhonew = (((B.array().min(rho.array() + mvlmt)).array().min(Eigen::VectorXd::Ones(rholen).array())).array().max(rho.array() - mvlmt)).array().max(Eigen::VectorXd::Constant(rholen, 1.0e-10).array());
			int pos2 = 0;
			for (auto pelement : this->poptimizedelements) {
				for (auto pi : this->plist) {
					pelement->parameters[pi] = rhonew(pos2);
					pos2++;
				}
			}
			if (this->pconstraints[0]->value() > 0.0) {
				lambda0 = lambda;
			}
			else {
				lambda1 = lambda;
			}
		}
		std::cout << "\t" << "Lagrange value：" << lambda0 << std::endl;

		//----------目的関数の値を更新----------
		previousvalue = currentvalue;
	}
}


/*void PANSFEM::OptimizedSystem::Schedule() {
	//----------順解析の設定----------
	for (auto pfield : this->pfields) {
		//.....場の初期化.....
		pfield->Initialize();

		//.....Kmapの作成.....
		pfield->MakeKmap();

		//.....バンド幅の計算.....
		pfield->GetBandwidth();
	}

	//----------設計変数の初期化----------
	const int rholen = this->poptimizedelements.size() * this->plist.size();	//設計変数ベクトルの要素数
	const int iterationmax = 50;				//最適化ループの最大反復数
	const double valueconvergence = 1.0e-5;		//目的関数の収束判定値
	const double lambdaconvergence = 1.0e-9;	//Lagrange乗数λの収束判定値
	const double mvlmt = 0.15;					//ムーブリミット
	const double iota = 0.75;					//ダンピング係数

	const double beta = 50.0;					//Heaviside Filterのパラメータ
	const double R = 3.6;						//Heaviside Filterのフィルタ半径

	std::map<Element*, std::vector<Element*>> relation;
	for (auto pelementi : this->poptimizedelements) {
		relation[pelementi] = std::vector<Element*>(0);

		//要素iの重心を計算
		Eigen::VectorXd centeri = Eigen::VectorXd::Zero(this->DOX);
		for (auto pnode : pelementi->pnodes) {
			centeri += pnode->x;
		}
		centeri /= (double)pelementi->pnodes.size();

		for (auto pelementj : this->poptimizedelements) {
			//要素jの重心を計算
			Eigen::VectorXd centerj = Eigen::VectorXd::Zero(this->DOX);
			for (auto pnode : pelementj->pnodes) {
				centerj += pnode->x;
			}
			centerj /= (double)pelementj->pnodes.size();

			double rij = (centerj - centeri).norm();
			if (rij < R) {
				relation[pelementi].push_back(pelementj);
			}
		}
	}

	std::map<Element*, Eigen::VectorXd> center;
	for (auto pelement : this->poptimizedelements) {
		center[pelement] = Eigen::VectorXd::Zero(this->DOX);
		for (auto pnode : pelement->pnodes) {
			center[pelement] += pnode->x;
		}
		center[pelement] /= (double)pelement->pnodes.size();
	}

	//----------最適化の反復計算----------
	double previousvalue = 0.0;		//1ステップ前の目的関数の値
	for (int itr = 0; itr < iterationmax; itr++) {
		std::cout << "Iteration:" << itr << std::endl;

		//----------順解析----------
		std::cout << "\t" << "Solution";
		for (auto pfield : this->pfields) {
			//.....要素―節点方程式を計算.....
			for (auto& pequation : pfield->pequations) {
				pequation->SetEquation();
			}

			//.....全体―節点方程式を解く.....
			pfield->SolveEquation();
		}
		std::cout << " is done." << std::endl;
		this->Export("Data/Output/a");

		//----------目的関数，制約条件の計算----------
		double currentvalue = this->pobjectives[0]->value();
		std::cout << "\t" << "Objective function value:" << currentvalue << std::endl;
		double constraintvalue = this->pconstraints[0]->value();
		std::cout << "\t" << "Constraint function value:" << constraintvalue << std::endl;

		//----------収束判定----------
		if (fabs((currentvalue - previousvalue) / (currentvalue + previousvalue)) < valueconvergence && constraintvalue <= 0.0) {
			std::cout << "----------System is optimized----------" << std::endl;
			break;
		}

		//----------設計感度を計算----------
		Eigen::VectorXd objectivesensitivity = this->pobjectives[0]->sensitivitis();
		Eigen::VectorXd constraintsensitivity = this->pconstraints[0]->sensitivitis();

		//----------現在の設計変数ベクトルを生成----------
		Eigen::VectorXd rho = Eigen::VectorXd(rholen);
		int pos = 0;
		for (auto pelement : this->poptimizedelements) {
			for (auto pi : this->plist) {
				rho(pos) = pelement->parameters[pi];
				pos++;
			}
		}

		//----------Lagrange乗数を二分探索----------
		double lambda0 = 1.0e-10, lambda1 = 1.0e10;
		while ((lambda1 - lambda0) / (lambda1 + lambda0) > lambdaconvergence) {
			double lambda = 0.5 * (lambda1 + lambda0);
			Eigen::VectorXd B = pow((-objectivesensitivity.array() / constraintsensitivity.array()).array() / lambda, iota).array()*rho.array();
			Eigen::VectorXd rhonew = (((B.array().min(rho.array() + mvlmt)).array().min(Eigen::VectorXd::Ones(rholen).array())).array().max(rho.array() - mvlmt)).array().max(Eigen::VectorXd::Constant(rholen, 1.0e-10).array());

			//----------Heaviside Filter----------
			Eigen::VectorXd rhof = Eigen::VectorXd::Zero(rholen);
			int pos3 = 0;
			for (auto pelementi : this->poptimizedelements) {
				std::vector<double> sumw = std::vector<double>(this->plist.size(), 0.0);
				std::vector<double> sumws = std::vector<double>(this->plist.size(), 0.0);

				for (auto pelementj : relation[pelementi]) {
					double w = R - (center[pelementj] - center[pelementi]).norm();
					auto iter = std::find(this->poptimizedelements.begin(), this->poptimizedelements.end(), pelementj);
					int index = std::distance(this->poptimizedelements.begin(), iter);

					for (int k = 0; k < this->plist.size(); k++) {
						sumw[k] += w;
						sumws[k] += w*rhonew[this->plist.size()*index+k];
					}
				}

				for (int k = 0; k < this->plist.size(); k++) {
					rhof[pos3] = sumws[k] / sumw[k];
					pos3++;
				}
			}
			//Eigen::VectorXd rhomod = 1.0 - (-rhof.array()*beta).exp() + rhof.array()*exp(-beta);
			Eigen::VectorXd rhomod = 1.0 / (1.0 + (-(rhof.array() - 0.5)*beta).exp());

			int pos2 = 0;
			for (auto pelement : this->poptimizedelements) {
				for (auto pi : this->plist) {
					pelement->parameters[pi] = rhomod(pos2);
					pos2++;
				}
			}
			if (this->pconstraints[0]->value() > 0.0) {
				lambda0 = lambda;
			}
			else {
				lambda1 = lambda;
			}
			pos2 = 0;
			for (auto pelement : this->poptimizedelements) {
				for (auto pi : this->plist) {
					pelement->parameters[pi] = rhonew(pos2);
					pos2++;
				}
			}
		}
		std::cout << "\t" << "Lagrange value：" << lambda0 << std::endl;

		//----------目的関数の値を更新----------
		previousvalue = currentvalue;
	}
}*/