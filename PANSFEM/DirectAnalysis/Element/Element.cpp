//*****************************************************************************
//Title		:PANSFEM/DirectAnalysis/Element/Element.h
//Author	:Tanabe Yuta
//Date		:2019/06/08
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include "Element.h"


PANSFEM::Element::Element() : NON(0), PARAVIEWTYPE(0){
	this->pmapping = nullptr;
}


PANSFEM::Element::~Element(){
	for (auto& pshapefunction : this->pshapefunctions) {
		delete pshapefunction;
	}
	delete this->pmapping;
}


PANSFEM::Element::Element(Parameter* _pparameter, std::vector<Node*> _pnodes, std::vector<int> _ulist, int _paraviewtype) : NON(_pnodes.size()), PARAVIEWTYPE(_paraviewtype){
	//----------パラメータを指すポインタを代入----------
	this->pparameter = _pparameter;
	
	//----------節点を指すポインタの代入----------
	this->pnodes = _pnodes;

	//----------従属変数の対応関係を整理----------
	for (int i = 0; i < _ulist.size(); i++) {
		this->us_to_uel[_ulist[i]] = i;
	}
		
	//----------自然座標系への写像関数はとりあえずnullptr----------
	this->pmapping = nullptr;
}


/*void PANSFEM::Element::SetParameter(std::vector<double> _parameters, std::vector<int> _plist){
	try {
		if (_parameters.size() != _plist.size()) {
			throw std::exception();
		}

		//----------パラメータの対応関係を整理----------
		for (int i = 0; i < _plist.size(); i++) {
			this->parameters[_plist[i]] = _parameters[i];
		}
	}
	catch (std::exception e) {
		std::cout << "Error in setting parameters" << std::endl;
	}
}*/


/*void PANSFEM::Element::SetParameter(Parameter * _pparameter) {
	this->pparameter = _pparameter;
}*/


Eigen::MatrixXd PANSFEM::Element::Trial(std::vector<int> _ueq_to_us, std::vector<double> _xi){
	int DOU = _ueq_to_us.size();			//従属変数自由度
	int DEGREE = 0;							//節点自由度の合計
	
	//----------節点自由度の計算----------
	std::vector<int> Rcol = std::vector<int>(this->NON);
	std::vector<std::vector<bool> > is_udefined = std::vector<std::vector<bool> >(this->NON, std::vector<bool>(DOU, false));
	for (int i = 0; i < this->NON; i++) {
		Rcol[i] = DEGREE;
		for (int j = 0; j < DOU; j++) {
			if (this->pnodes[i]->us_to_un.find(_ueq_to_us[j]) != this->pnodes[i]->us_to_un.end()) {
				is_udefined[i][j] = true;
				DEGREE++;
			}
		}
	}

	//----------形状関数行列を計算----------
	Eigen::MatrixXd trial = Eigen::MatrixXd(DOU, DEGREE);
	for (int j = 0; j < DOU; j++) {
		//.....必要な節点のリストを生成.....
		std::vector<Node*> tmppnodes;
		for (int i = 0; i < this->NON; i++) {
			if (is_udefined[i][j]) {
				tmppnodes.push_back(this->pnodes[i]);
			}
		}
		
		//.....形状関数を取得.....
		Eigen::VectorXd N = this->pshapefunctions[this->us_to_uel[_ueq_to_us[j]]]->Trial(tmppnodes, _xi);

		//.....右形状行列の計算.....
		Eigen::MatrixXd R = Eigen::MatrixXd::Zero(N.rows(), DEGREE);
		int Rrow = 0;
		for (int i = 0; i < this->NON; i++) {
			if (is_udefined[i][j]) {
				R(Rrow, Rcol[i]) = 1.0;
				Rrow++;
				Rcol[i]++;
			}
		}

		trial.row(j) = N.transpose() * R;
	}

	return trial;
}


Eigen::MatrixXd PANSFEM::Element::dTrialdx(std::vector<int> _ueq_to_us, std::vector<double> _xi){
	int DOX = this->pnodes[0]->x.rows();	//独立変数自由度
	int DOU = _ueq_to_us.size();			//従属変数自由度
	int DEGREE = 0;							//節点自由度の合計

	//----------節点自由度の計算----------
	std::vector<int> Rcol = std::vector<int>(this->NON);
	std::vector<std::vector<bool> > is_udefined = std::vector<std::vector<bool> >(this->NON, std::vector<bool>(DOU, false));
	for (int i = 0; i < this->NON; i++) {
		Rcol[i] = DEGREE;
		for (int j = 0; j < DOU; j++) {
			if (this->pnodes[i]->us_to_un.find(_ueq_to_us[j]) != this->pnodes[i]->us_to_un.end()) {
				is_udefined[i][j] = true;
				DEGREE++;
			}
		}
	}

	//----------形状関数行列を計算----------
	Eigen::MatrixXd dtrialdx = Eigen::MatrixXd::Zero(DOU * DOX, DEGREE);
	for (int j = 0; j < DOU; j++) {
		//.....必要な節点のリストを生成.....
		std::vector<Node*> tmppnodes;
		for (int i = 0; i < this->NON; i++) {
			if (is_udefined[i][j]) {
				tmppnodes.push_back(this->pnodes[i]);
			}
		}

		//.....形状関数を取得.....
		Eigen::MatrixXd dN = this->Jacobian(_xi).inverse() * this->pshapefunctions[this->us_to_uel[_ueq_to_us[j]]]->dTrialdx(tmppnodes, _xi);

		//.....右形状行列の計算.....
		Eigen::MatrixXd R = Eigen::MatrixXd::Zero(dN.cols(), DEGREE);
		int Rrow = 0;
		for (int i = 0; i < this->NON; i++) {
			if (is_udefined[i][j]) {
				R(Rrow, Rcol[i]) = 1.0;
				Rrow++;
				Rcol[i]++;
			}
		}

		//.....左形状行列の計算.....
		Eigen::MatrixXd L = Eigen::MatrixXd::Zero(DOU * DOX, DOX);
		for (int i = 0; i < DOX; i++) {
			L(DOU * i + j, i) = 1.0;
		}

		dtrialdx += L * dN * R;
	}

	return dtrialdx;
}


Eigen::MatrixXd PANSFEM::Element::Test(std::vector<int> _ueq_to_us, std::vector<double> _xi){
	int DOU = _ueq_to_us.size();			//従属変数自由度
	int DEGREE = 0;							//節点自由度の合計

	//----------節点自由度の計算----------
	std::vector<int> Rcol = std::vector<int>(this->NON);
	std::vector<std::vector<bool> > is_udefined = std::vector<std::vector<bool> >(this->NON, std::vector<bool>(DOU, false));
	for (int i = 0; i < this->NON; i++) {
		Rcol[i] = DEGREE;
		for (int j = 0; j < DOU; j++) {
			if (this->pnodes[i]->us_to_un.find(_ueq_to_us[j]) != this->pnodes[i]->us_to_un.end()) {
				is_udefined[i][j] = true;
				DEGREE++;
			}
		}
	}

	//----------形状関数行列を計算----------
	Eigen::MatrixXd test = Eigen::MatrixXd(DOU, DEGREE);
	for (int j = 0; j < DOU; j++) {
		//.....必要な節点のリストを生成.....
		std::vector<Node*> tmppnodes;
		for (int i = 0; i < this->NON; i++) {
			if (is_udefined[i][j]) {
				tmppnodes.push_back(this->pnodes[i]);
			}
		}

		//.....形状関数を取得.....
		Eigen::VectorXd N = this->pshapefunctions[this->us_to_uel[_ueq_to_us[j]]]->Test(tmppnodes, _xi);

		//.....右形状行列の計算.....
		Eigen::MatrixXd R = Eigen::MatrixXd::Zero(N.rows(), DEGREE);
		int Rrow = 0;
		for (int i = 0; i < this->NON; i++) {
			if (is_udefined[i][j]) {
				R(Rrow, Rcol[i]) = 1.0;
				Rrow++;
				Rcol[i]++;
			}
		}

		test.row(j) = N.transpose() * R;
	}

	return test;
}


Eigen::MatrixXd PANSFEM::Element::dTestdx(std::vector<int> _ueq_to_us, std::vector<double> _xi){
	int DOX = this->pnodes[0]->x.rows();	//独立変数自由度
	int DOU = _ueq_to_us.size();			//従属変数自由度
	int DEGREE = 0;							//節点自由度の合計

	//----------節点自由度の計算----------
	std::vector<int> Rcol = std::vector<int>(this->NON);
	std::vector<std::vector<bool> > is_udefined = std::vector<std::vector<bool> >(this->NON, std::vector<bool>(DOU, false));
	for (int i = 0; i < this->NON; i++) {
		Rcol[i] = DEGREE;
		for (int j = 0; j < DOU; j++) {
			if (this->pnodes[i]->us_to_un.find(_ueq_to_us[j]) != this->pnodes[i]->us_to_un.end()) {
				is_udefined[i][j] = true;
				DEGREE++;
			}
		}
	}

	//----------形状関数行列を計算----------
	Eigen::MatrixXd dtestdx = Eigen::MatrixXd::Zero(DOU * DOX, DEGREE);
	for (int j = 0; j < DOU; j++) {
		//.....必要な節点のリストを生成.....
		std::vector<Node*> tmppnodes;
		for (int i = 0; i < this->NON; i++) {
			if (is_udefined[i][j]) {
				tmppnodes.push_back(this->pnodes[i]);
			}
		}

		//.....形状関数を取得.....
		Eigen::MatrixXd dN = this->Jacobian(_xi).inverse() * this->pshapefunctions[this->us_to_uel[_ueq_to_us[j]]]->dTestdx(tmppnodes, _xi);

		//.....右形状行列の計算.....
		Eigen::MatrixXd R = Eigen::MatrixXd::Zero(dN.cols(), DEGREE);
		int Rrow = 0;
		for (int i = 0; i < this->NON; i++) {
			if (is_udefined[i][j]) {
				R(Rrow, Rcol[i]) = 1.0;
				Rrow++;
				Rcol[i]++;
			}
		}

		//.....左形状行列の計算.....
		Eigen::MatrixXd L = Eigen::MatrixXd::Zero(DOU * DOX, DOX);
		for (int i = 0; i < DOX; i++) {
			L(DOU * i + j, i) = 1.0;
		}

		dtestdx += L * dN * R;
	}

	return dtestdx;
}


Eigen::MatrixXd PANSFEM::Element::Jacobian(std::vector<double> _xi){
	Eigen::MatrixXd xs = Eigen::MatrixXd(this->NON, this->pnodes[0]->x.rows());
	for (int i = 0; i < this->NON; i++) {
		xs.row(i) = this->pnodes[i]->x.transpose();
	}
	
	return this->pmapping->dTrialdx(this->pnodes, _xi) * xs;
}


Eigen::VectorXd PANSFEM::Element::u(std::vector<int> _ueq_to_us){
	std::vector<double> us;
	for (auto pnode : this->pnodes) {
		for (auto ui : _ueq_to_us) {
			if (pnode->us_to_un.find(ui) != pnode->us_to_un.end()) {
				us.push_back(pnode->u[pnode->us_to_un[ui]]);
			}
		}
	}
	return Eigen::Map<Eigen::VectorXd>(&us[0], us.size());
}
