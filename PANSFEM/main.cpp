//*****************************************************************************
//Title		:PANSFEM/main.cpp
//Author	:Tanabe Yuta
//Date		:2019/06/08
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include <vector>


#include "Core/System/System.h"
#include "Phenomenon/Structure/PlaneStrain/PlaneStrain.h"


using namespace PANSFEM;


int main() {
	//----------ƒ‚ƒfƒ‹‚Ì’è‹`----------
	System model1 = System(2, 2);
	model1.ImportNode("Data/Input/Node.csv");
	model1.ImportElement<ShapeFunction, ShapeFunction>("Data/Input/Element.csv");
	model1.ImportField<PlaneStrain>({ 0, 1 }, "Data/Input/Equation_Structure.csv");

	return 0;
}