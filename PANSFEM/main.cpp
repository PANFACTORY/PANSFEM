//*****************************************************************************
//Title		:PANSFEM/main.cpp
//Author	:Tanabe Yuta
//Date		:2019/06/08
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include <vector>


#include "Core/System/StaticSystem/StaticSystem.h"
#include "Core/ShapeFunction/Simplex/Triangle.h"
#include "Core/Field/LinearField/LinearField.h"
#include "Phenomenon/Structure/PlaneStrain/PlaneStrain.h"


using namespace PANSFEM;


int main() {
	//----------ê¸å`ê√ìIÉÇÉfÉã----------
	StaticSystem model1 = StaticSystem(2, 2);
	model1.ImportNode("Data/Input/Node.csv");
	model1.ImportElement<Triangle, Triangle, Triangle>("Data/Input/Element.csv");
	model1.ImportField<LinearField, PlaneStrain>({ 0, 1 }, "Data/Input/Equation_Structure.csv");
	model1.ImportDirichlet("Data/Input/Dirichlet.csv");
	model1.Schedule();

	return 0;
}