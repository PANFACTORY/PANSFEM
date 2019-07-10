//*****************************************************************************
//Title		:PANSFEM/InversedAnalysis/System/OptimizedSystem.cpp
//Author	:Tanabe Yuta
//Date		:2019/06/30
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "OptimizedSystem.h"


PANSFEM::OptimizedSystem::OptimizedSystem() : System(){}


PANSFEM::OptimizedSystem::~OptimizedSystem(){}


PANSFEM::OptimizedSystem::OptimizedSystem(int _DOX, int _DOU, std::vector<int> _plist) : System(_DOX, _DOU){
	this->plist = _plist;
}


bool PANSFEM::OptimizedSystem::ImportOptimizedElement(std::string _fname){
	std::ifstream ifs(_fname);

	if (!ifs.is_open()) {
		std::cout << "Element file " << _fname << " open error!" << std::endl;
		return false;
	}

	//.....一行読み飛ばす.....
	std::string str0;
	std::getline(ifs, str0);

	while (!ifs.eof()) {
		//.....一行分読み込む.....
		std::string buf;
		ifs >> buf;
		std::istringstream sbuf(buf);
		std::string str;

		//.....IDを読み飛ばす.....
		std::getline(sbuf, str, ',');

		//.....対応する要素を指すポインタを最適化対象要素のリストに追加.....
		std::getline(sbuf, str, ',');
		this->poptimizedelements.push_back(this->pelements[stoi(str)]);
	}

	ifs.close();

	return true;
}


void PANSFEM::OptimizedSystem::Schedule(){
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
	const double valueconvergence = 1.0e-6;		//目的関数の収束判定値
	const double lambdaconvergence = 1.0e-3;	//Lagrange乗数λの収束判定値
	const double mvlmt = 0.15;					//ムーブリミット
	const double iota = 0.75;					//ダンピング係数

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
		double lambda0 = 0.0, lambda1 = 1.0e4;
		while ((lambda1 - lambda0)/(lambda1 + lambda0) > lambdaconvergence) {
			double lambda = 0.5 * (lambda1 + lambda0);
			Eigen::VectorXd B = pow((-objectivesensitivity.array() / constraintsensitivity.array()).array() / lambda, iota).array()*rho.array();
			Eigen::VectorXd rhonew = (((B.array().min(rho.array() * (1.0 + mvlmt))).array().min(Eigen::VectorXd::Ones(rholen).array())).array().max(rho.array() * (1.0 - mvlmt))).array().max(Eigen::VectorXd::Constant(rholen, 1.0e-10).array());
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


void PANSFEM::OptimizedSystem::Export(std::string _fname){
	std::ofstream fout(_fname + ".vtk");

	//----------Headerの出力----------
	fout << "# vtk DataFile Version 4.1\n";
	fout << "vtk output\n";
	fout << "ASCII\n";
	fout << "DATASET UNSTRUCTURED_GRID\n";

	//----------節点の出力----------
	fout << "\nPOINTS\t" << this->pnodes.size() << "\tfloat\n";
	for (auto pnode : this->pnodes) {
		int i = 0;
		for (; i < this->DOX; i++) {
			fout << pnode->x(i) << "\t";
		}
		for (; i < 3; i++) {
			fout << "0.0" << "\t";
		}
		fout << std::endl;
	}

	//----------要素の追加----------
	int datanum = 0;
	for (auto pelement : this->pelements) {
		datanum += pelement->NON + 1;
	}

	std::map<Node*, int> nodelist;
	int nodenum = 0;
	for (auto pnode : this->pnodes) {
		nodelist[pnode] = nodenum;
		nodenum++;
	}

	fout << "\nCELLS " << this->pelements.size() << "\t" << datanum << "\n";
	for (auto pelement : this->pelements) {
		fout << pelement->NON << "\t";
		for (auto pnode : pelement->pnodes) {
			fout << nodelist[pnode] << "\t";
		}
		fout << std::endl;
	}

	//----------要素タイプの設定----------
	//要修正
	fout << "\nCELL_TYPES\t" << this->pelements.size() << "\n";
	for (auto pelement : this->pelements) {
		fout << "9\n";
	}

	//----------節点の値を追加----------
	fout << "\nCELL_DATA\t" << this->pelements.size() << "\n";
	fout << "SCALARS Rho float\n";
	fout << "LOOKUP_TABLE default\n";

	for (auto pelement : this->pelements) {
		fout << pelement->parameters[this->plist[0]] << std::endl;
	}

	fout.close();
}