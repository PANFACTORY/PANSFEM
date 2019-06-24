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
	//----------直梁（四角形要素）モデル----------
	/*StaticSystem model1 = StaticSystem(2, 2);
	model1.ImportNode("Data/Input/QuadrangleBeam/Node.csv");
	model1.ImportElement<Quadrangle, Quadrangle, Quadrangle>("Data/Input/QuadrangleBeam/Element.csv");
	model1.ImportField<LinearField, PlaneStrain>({ 0, 1 }, "Data/Input/QuadrangleBeam/Equation_Structure.csv");
	model1.ImportDirichlet("Data/Input/QuadrangleBeam/Dirichlet.csv");
	model1.ImportNeumann("Data/Input/QuadrangleBeam/Neumann.csv");
	model1.Schedule();
	//model1.Show();
	model1.Export("Data/Output/model1");
	*/

	//----------曲がり梁モデル----------
	StaticSystem model2 = StaticSystem(2, 2);
	model2.ImportNode("Data/Input/CurveBeam/Node.csv");
	model2.ImportElement<Quadrangle, Quadrangle, Quadrangle>("Data/Input/CurveBeam/Element.csv");
	model2.ImportField<LinearField, PlaneStrain>({ 0, 1 }, "Data/Input/CurveBeam/Equation_Structure.csv");
	model2.ImportDirichlet("Data/Input/CurveBeam/Dirichlet.csv");
	model2.ImportNeumann("Data/Input/CurveBeam/Neumann.csv");
	model2.Schedule();
	//model2.Show();
	model2.Export("Data/Output/model2");
	
	return 0;
}