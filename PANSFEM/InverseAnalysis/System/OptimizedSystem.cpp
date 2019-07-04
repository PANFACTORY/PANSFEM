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

	//----------最適化の反復計算----------
	double previousvalue = 0.0;		//1ステップ前の目的関数の値
	for (int itr = 0; itr < 10; itr++) {
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
		
		//----------目的関数の計算----------
		double currentvalue = this->pobjectives[0]->value();
		std::cout << "\t" << "Objective function value:" << currentvalue << std::endl;

		//----------収束判定----------
		if (fabs(currentvalue - previousvalue) < 1.0e-5) {
			//std::cout << "----------System is optimized----------" << std::endl;
			//break;
		}
		
		//----------設計感度を計算----------
		Eigen::VectorXd objectivesensitivity = this->pobjectives[0]->sensitivitis();
		Eigen::VectorXd constraintsensitivity = this->pconstraints[0]->sensitivitis();
				
		//----------Lagrange乗数を二分探索----------
		double lambda0 = 0.0, lambda1 = 1.0e4;
		while ((lambda1 - lambda0)/(lambda1 + lambda0) > 1.0e-3) {
			double lambda = 0.5 * (lambda1 + lambda0);
			Eigen::VectorXd B = pow((-objectivesensitivity.array() / constraintsensitivity.array()).array() / lambda, 0.75);
			
			
		}

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
	fout << "\nPOINT_DATA\t" << this->pnodes.size() << "\n";
	fout << "SCALARS p" << " float\n";
	fout << "LOOKUP_TABLE default\n";

	for (auto pelement : this->pelements) {
		fout << pelement->parameters[this->plist[0]] << std::endl;
	}

	fout.close();
}