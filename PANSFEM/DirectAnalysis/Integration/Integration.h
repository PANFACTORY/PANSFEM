//*****************************************************************************
//Title		:PANSFEM/Core/Integration/Integration.h
//Author	:Tanabe Yuta
//Date		:2019/06/08
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include <vector>
#include <Eigen/dense>


namespace PANSFEM {
	class Integration
	{
	public:
		Integration();
		virtual ~Integration();

		
		virtual Eigen::MatrixXd Integrate(std::function<Eigen::MatrixXd(std::vector<double>)>) = 0;		//�s���Ԃ��֐��̐ϕ�
		virtual Eigen::VectorXd Integrate(std::function<Eigen::VectorXd(std::vector<double>)>) = 0;		//�x�N�g����Ԃ��֐��̐ϕ�
		virtual double Integrate(std::function<double(std::vector<double>)>) = 0;						//�X�J���[��Ԃ��֐��̐ϕ�
	};
}