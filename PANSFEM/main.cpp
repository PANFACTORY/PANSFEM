//*****************************************************************************
//Title		:PANSFEM/main.cpp
//Author	:Tanabe Yuta
//Date		:2019/06/08
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include <vector>


#include "Core/System/StaticSystem/StaticSystem.h"
#include "Core/ShapeFunction/Parametric/Quadrangle/Quadrangle.h"
#include "Core/Field/LinearField/LinearField.h"
#include "Phenomenon/Structure/PlaneStrain/PlaneStrain.h"


using namespace PANSFEM;


int main() {
	//----------ê¸å`ê√ìIÉÇÉfÉã----------
	StaticSystem model1 = StaticSystem(2, 2);
	model1.ImportNode("Data/Input/QuadrangleBeam/Node.csv");
	model1.ImportElement<Quadrangle, Quadrangle, Quadrangle>("Data/Input/QuadrangleBeam/Element.csv");
	model1.ImportField<LinearField, PlaneStrain>({ 0, 1 }, "Data/Input/QuadrangleBeam/Equation_Structure.csv");
	model1.ImportDirichlet("Data/Input/QuadrangleBeam/Dirichlet.csv");
	//model1.ImportNeumann("Data/Input/Neumann.csv");
	model1.Schedule();
	model1.Show();

	return 0;
}