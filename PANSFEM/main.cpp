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
	model1.ImportNode("Data/Input/Structure/QuadrangleBeam/Node.csv");
	model1.ImportElement<Quadrangle, Quadrangle, Quadrangle>("Data/Input/Structure/QuadrangleBeam/Element.csv");
	model1.ImportField<LinearField, PlaneStrain>({ 0, 1 }, "Data/Input/Structure/QuadrangleBeam/Equation_Structure.csv");
	model1.ImportDirichlet("Data/Input/Structure/QuadrangleBeam/Dirichlet.csv");
	model1.ImportNeumann("Data/Input/Structure/QuadrangleBeam/Neumann.csv");
	model1.Schedule();
	//model1.Show();
	model1.Export("Data/Output/model1");
	*/

	//----------曲がり梁モデル----------
	StaticSystem model2 = StaticSystem(2, 2);
	model2.ImportNode("Data/Input/Structure/CurveBeam/Node.csv");
	model2.ImportElement<Quadrangle, Quadrangle, Quadrangle>("Data/Input/Structure/CurveBeam/Element.csv");
	model2.ImportField<LinearField, PlaneStrain>({ 0, 1 }, "Data/Input/Structure/CurveBeam/Equation_Structure.csv");
	model2.ImportDirichlet("Data/Input/Structure/CurveBeam/Dirichlet.csv");
	model2.ImportNeumann("Data/Input/Structure/CurveBeam/Neumann.csv");
	model2.Schedule();
	//model2.Show();
	model2.Export("Data/Output/model2");


	//----------熱伝導モデル----------
	StaticSystem model3 = StaticSystem(2, 1);
	
	return 0;
}