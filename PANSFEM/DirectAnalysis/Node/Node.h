//*****************************************************************************
//Title		:PANSFEM/Core/Node/Node.h
//Author	:Tanabe Yuta
//Date		:2019/06/08
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#pragma once
#include <vector>
#include <map>
#include <Eigen/dense>


namespace PANSFEM {
	class Node
	{
	public:
		Node();
		virtual ~Node();
		Node(std::vector<double> _x, std::vector<int> _ulist);


		const int DOU;					//ß“_‚Å’è‹`‚³‚ê‚½]‘®•Ï”‚ÌŸŒ³


		Eigen::VectorXd x;				//“Æ—§•Ï”
		Eigen::VectorXd u;				//]‘®•Ï”
		std::vector<bool> is_ufixed;	//]‘®•Ï”‚ªŒÅ’è‚³‚ê‚Ä‚¢‚é‚©
		std::map<int, int> us_to_un;	//Œn]‘®•Ï””Ô†¨ß“_]‘®•Ï””Ô†
	};
}
