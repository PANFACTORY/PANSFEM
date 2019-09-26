//***********************************************************************************************************
//Title		:PANSFEM/DirectAnalysis/Equation/StructualElement/ElasticPlasticTruss2D/ElasticPlasticTruss2D.cpp
//Author	:Tanabe Yuta
//Date		:2019/08/21
//Copyright	:(C)2019 TanabeYuta
//***********************************************************************************************************


#include "ElasticPlasticTruss2D.h"


PANSFEM::ElasticPlasticTruss2D::ElasticPlasticTruss2D() : Equation(){}


PANSFEM::ElasticPlasticTruss2D::~ElasticPlasticTruss2D(){}


PANSFEM::ElasticPlasticTruss2D::ElasticPlasticTruss2D(Element * _pelement, std::vector<int> _ulist, std::vector<int> _refulist, std::vector<int> _plist, Integration * _pintegration) : Equation(_pelement, _ulist, _refulist, _plist, _pintegration, 2, 2, 0, 4){
	try {
		if (_pelement->NON != 2) {
			throw std::exception();
		}

		this->epspn = 0.0;
		this->epsbarpn = 0.0;
	}
	catch (std::exception e) {
		std::cout << "Error in ElasticPlasticTruss2D" << std::endl;
	}
}


void PANSFEM::ElasticPlasticTruss2D::SetEquation(){
	//----------�p�����[�^�̎擾----------
	double A = this->pelement->pparameter->parameters[this->peq_to_ps[0]];			//�v�f�����f�ʐ�
	double E = this->pelement->pparameter->parameters[this->peq_to_ps[1]];			//�ޗ�Young��
	double H = this->pelement->pparameter->parameters[this->peq_to_ps[2]];			//�ޗ��Y���W��
	double tauy0 = this->pelement->pparameter->parameters[this->peq_to_ps[3]];		//�����~������

	Eigen::VectorXd n = this->pelement->pnodes[1]->x - this->pelement->pnodes[0]->x;	
	double L = n.norm();												//�v�f��������
	Eigen::VectorXd u = Eigen::VectorXd(this->ueq_to_us.size());
	for (int i = 0; i < this->ueq_to_us.size(); i++) {
		u(i) = this->pelement->pnodes[1]->u[this->ueq_to_us[i]] - this->pelement->pnodes[0]->u[this->ueq_to_us[i]];
	}
	double l = (n + u).norm();											//�v�f�̕ό`��̒���
	n /= L;																//�v�f���S�����x�N�g��
	
	//----------return mapping�@----------
	double tautrial = E * (log(l / L) - this->epspn);					//���s�e������
	double ftrialnp1 = abs(tautrial) - (tauy0 + H * this->epsbarpn);	//�~������
	double dganma = 0.0;												//�����^�Y���搔
	double dtaudeps = E;												//�ڐ��W��
	if (ftrialnp1 > 0.0) {
		dganma = ftrialnp1 / (E + H);
		dtaudeps = E * H / (E + H);
	}

	//----------���́C�ڐ��W���̎Z�o----------
	double sign = tautrial > 0.0 ? 1.0 : -1.0;							//�����֐�
	double tau = tautrial - E * dganma*sign;							//����
	double k = A * L / pow(l, 2.0)*(dtaudeps - 2.0*tau);
	double T = A * L*tau / l;											//����

	//----------�ڐ��������v�Z----------
	this->Ke = Eigen::MatrixXd(4, 4);
	Eigen::MatrixXd K = k * n*n.transpose() + T / l * Eigen::MatrixXd::Identity(2, 2);
	this->Ke.block(0, 0, 2, 2) = K;		this->Ke.block(0, 2, 2, 2) = -K;
	this->Ke.block(2, 0, 2, 2) = -K;	this->Ke.block(2, 2, 2, 2) = K;

	//----------�c���x�N�g�����v�Z�i�V�X�e���㏑�Ђƕ������t�ɂȂ邱�Ƃɒ��Ӂj----------
	this->Fe = Eigen::MatrixXd(4, 1);
	this->Fe.block(0, 0, 2, 1) = T * n;		this->Fe.block(2, 0, 2, 1) = -T * n;
}