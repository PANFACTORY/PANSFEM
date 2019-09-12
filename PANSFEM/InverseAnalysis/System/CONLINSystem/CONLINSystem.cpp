//*****************************************************************************
//Title		:PANSFEM/InversedAnalysis/System/CONLINSystem/CONLINSystem.cpp
//Author	:Tanabe Yuta
//Date		:2019/08/15
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "CONLINSystem.h"


PANSFEM::CONLINSystem::CONLINSystem() : OptimizedSystem(){}


PANSFEM::CONLINSystem::~CONLINSystem(){}


PANSFEM::CONLINSystem::CONLINSystem(int _DOX, int _DOU, std::vector<int> _plist) : OptimizedSystem(_DOX, _DOU, _plist){}


void PANSFEM::CONLINSystem::Schedule(){
	//----------順解析の設定----------
	for (auto pfield : this->pfields) {
		//.....場の初期化.....
		pfield->Initialize();

		//.....Kmapの作成.....
		pfield->MakeKmap();

		//.....バンド幅の計算.....
		pfield->GetBandwidth();
	}

	//----------
	//設計変数の初期値，制約条件の上限値，ラグランジュ乗数の初期値，
	//制約緩和の重みw，収束値のトレランス，ムーブリミットの設定
	const int NOS = this->poptimizedelements.size() * this->plist.size();	//設計変数ベクトルの要素数
	const int NOOBJ = this->pobjectives.size();								//目的関数の数
	const int NOCON = this->pconstraints.size();							//制約条件の数
	
	const double W = 5.0;			//重み（1.0～10.0程度の値とする）
	const double EPS = 1.0e-8;		//≒0
	const double movelimit = 0.1;	//ムーブリミット

	Eigen::VectorXd r = Eigen::VectorXd::Constant(NOOBJ + NOCON, 1.0e-2);
	r.segment(0, NOOBJ).array() = 1.0;										//Lagrange乗数
	
	//----------最適化の反復計算----------
	for (int itr = 0; itr < 50; itr++) {
		std::cout << "itr = " << itr << std::endl;

		//----------順解析----------
		for (auto pfield : this->pfields) {
			//.....全体―節点方程式を解く.....
			pfield->SolveEquation();
		}

		//----------現在の設計変数ベクトルを生成----------
		Eigen::VectorXd s = Eigen::VectorXd(NOS);
		int spos = 0;
		for (auto pelement : this->poptimizedelements) {
			for (auto pi : this->plist) {
				s(spos) = pelement->parameters[pi];
				spos++;
			}
		}

		//----------目的関数，制約条件の値，感度係数を求める----------
		Eigen::VectorXd c = Eigen::VectorXd(NOOBJ + NOCON);
		int cpos = 0;
		for (auto pobjective : this->pobjectives) {
			c(cpos) = pobjective->value();
			std::cout << "\t" << "Objective function value:" << c(cpos) << std::endl;
			cpos++;
		}
		for (auto pconstraint : this->pconstraints) {
			c(cpos) = pconstraint->value();
			std::cout << "\t" << "Constraint function value:" << c(cpos) << std::endl;
			cpos++;
		}		

		Eigen::MatrixXd dcds = Eigen::MatrixXd(NOOBJ + NOCON, NOS);
		int dcdxpos = 0;
		for (auto pobjective : this->pobjectives) {
			dcds.row(dcdxpos) = pobjective->sensitivitis();
			dcdxpos++;
		}
		for (auto pconstraint : this->pconstraints) {
			dcds.row(dcdxpos) = pconstraint->sensitivitis();
			dcdxpos++;
		}

		//----------式(1.6)のcバーj，式(5.2)のzjを計算----------
		Eigen::VectorXd z = dcds.cwiseAbs() * s;
		Eigen::VectorXd cbar = z - c;
				
		//----------式(1.3)のスケーリング----------
		Eigen::MatrixXd dcdstilda = (dcds.transpose().array().colwise() * s.array()).matrix().transpose();		//スケーリング後の感度行列
		Eigen::VectorXd stildamin = (EPS / s.array()).max(1.0 - movelimit);										//スケーリング後の設計変数の下限値
		Eigen::VectorXd stildamax = (1.0 / s.array()).min(1.0 + movelimit);										//スケーリング後の設計変数の上限値
		
		//----------CONLIN法の反復計算----------
		for (int itr2 = 0; itr2 < 10000; itr2++) {
			//----------設計変数stildaを求める----------
			Eigen::VectorXd a = dcdstilda.cwiseMax(0.0).transpose() * r;
			Eigen::VectorXd b = -dcdstilda.cwiseMin(0.0).transpose() * r;
			Eigen::VectorXd stildanew = (a.array().abs() > EPS).select((b.array() / a.array()).sqrt().max(stildamin.array()).min(stildamax.array()), (b.array().abs() < EPS).select(stildamin, stildamax));

			//----------gjの計算----------
			double delta = z.segment(NOOBJ, NOCON).dot(r.segment(NOOBJ, NOCON)) > z(0) * W ? sqrt(z.segment(NOOBJ, NOCON).dot(r.segment(NOOBJ, NOCON)) / (z(0) * W)) : 1.0;
			Eigen::VectorXd g = dcdstilda.cwiseMax(0.0) * stildanew - dcdstilda.cwiseMin(0.0) * (1.0 / stildanew.array()).matrix() - cbar - z * (1.0 - 1.0 / delta);
			
			//----------式(2.22)，(2.23)，(5.8)によるHjkの計算----------
			Eigen::MatrixXd dcdsp = dcdstilda.middleRows(1, NOCON).array().max(0.0).matrix();
			Eigen::MatrixXd dcdsm = dcdstilda.middleRows(1, NOCON).array().min(0.0).matrix(); 

			Eigen::VectorXd sbya = ((b.array() / a.array()) >= stildamin.array().pow(2) && (b.array() / a.array()) <= stildamax.array().pow(2)).select(stildanew.array() / a.array(), 0.0);
			Eigen::VectorXd bysa = ((b.array() / a.array()) >= stildamin.array().pow(2) && (b.array() / a.array()) <= stildamax.array().pow(2)).select(1.0 / (stildanew.array() * a.array()), 0.0);
			Eigen::VectorXd bys3a = ((b.array() / a.array()) >= stildamin.array().pow(2) && (b.array() / a.array()) <= stildamax.array().pow(2)).select(1.0 / (stildanew.array().pow(3.0) * a.array()), 0.0);

			Eigen::MatrixXd H = -0.5*((dcdsp * (dcdsp.transpose().array().colwise() * sbya.array()).matrix())
				+ (dcdsp * (dcdsm.transpose().array().colwise() * bysa.array()).matrix())
				+ (dcdsm * (dcdsp.transpose().array().colwise() * bysa.array()).matrix())
				+ (dcdsm * (dcdsm.transpose().array().colwise() * bys3a.array()).matrix()));
			if (z.segment(NOOBJ, NOCON).dot(r.segment(NOOBJ, NOCON)) > z(0) * W) {
				H -= 0.5*z.segment(NOOBJ, NOCON)*z.segment(NOOBJ, NOCON).transpose() / (W*z[0] * pow(delta, 3.0));
			}

			if (abs(H.determinant()) < EPS) {
				std::cout << "H is singurality" << std::endl;
				//std::cout << H << std::endl;

				sbya = (a.array().abs() > EPS).select(stildanew.array() / a.array(), 0.0);
				bysa = (a.array().abs() > EPS).select(1.0 / (stildanew.array() * a.array()), 0.0);
				bys3a = (a.array().abs() > EPS).select(1.0 / (stildanew.array().pow(3.0) * a.array()), 0.0);

				H = -0.5*((dcdsp * (dcdsp.transpose().array().colwise() * sbya.array()).matrix())
					+ (dcdsp * (dcdsm.transpose().array().colwise() * bysa.array()).matrix())
					+ (dcdsm * (dcdsp.transpose().array().colwise() * bysa.array()).matrix())
					+ (dcdsm * (dcdsm.transpose().array().colwise() * bys3a.array()).matrix()));
				if (z.segment(NOOBJ, NOCON).dot(r.segment(NOOBJ, NOCON)) > z(0) * W) {
					H -= 0.5*z.segment(NOOBJ, NOCON)*z.segment(NOOBJ, NOCON).transpose() / (W*z[0] * pow(delta, 3.0));
				}

				//std::cout << a << std::endl;
			}

			//----------rの更新----------
			Eigen::VectorXd dr = H.fullPivLu().solve(-g.segment(NOOBJ, NOCON));
			r.segment(NOOBJ, NOCON) += dr;

			r.segment(NOOBJ, NOCON) = r.segment(NOOBJ, NOCON).array().max(1.0e-8);

			//std::cout << "\t" <<r.transpose() << "\t" << g.transpose() << std::endl;
			std::cout << "H = "<< H << "\tg = " << g.segment(NOOBJ, NOCON) << "\tdr = " << dr << "\tr = " << r.segment(NOOBJ, NOCON) << std::endl;

			//----------式(3.7)の判定----------
			if (((g.segment(NOOBJ, NOCON).array().abs() < EPS && r.segment(NOOBJ, NOCON).array() > 0.0) || (g.segment(NOOBJ, NOCON).array() < 0.0 && r.segment(NOOBJ, NOCON).array().abs() < EPS)).all()) {
				//std::cout << "r = " << r.transpose() << "\tg = " << g.transpose() << std::endl;

				int pos2 = 0;
				for (auto pelement : this->poptimizedelements) {
					for (auto pi : this->plist) {
						pelement->parameters[pi] = stildanew(pos2)*s(pos2);
						pos2++;
					}
				}
				
				break;
			}
		}

		//----------目的関数の収束判定----------
	}
}
