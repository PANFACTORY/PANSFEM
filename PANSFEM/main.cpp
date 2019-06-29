//*****************************************************************************
//Title		:PANSFEM/main.cpp
//Author	:Tanabe Yuta
//Date		:2019/06/08
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include <vector>


#include "Core/System/StaticSystem/StaticSystem.h"


#include "Core/Field/LinearField/LinearField.h"


#include "Core/ShapeFunction/Parametric/Triangle/Triangle.h"
#include "Core/ShapeFunction/Parametric/Quadrangle/Quadrangle.h"


#include "Phenomenon/Structure/PlaneStrain/PlaneStrain.h"
#include "Phenomenon/HeatTransfer/Static2D/HeatTransferStatic2D.h"
#include "Phenomenon/Structure/PlaneStrainWithHeat/PlaneStrainWithHeat.h"


#include "Core/Integration/Gauss/Square/GaussSquare.h"
#include "Core/Integration/Gauss/Triangle/GaussTriangle.h"


using namespace PANSFEM;


int main() {
	//----------íºó¿ÅiéOäpå`óvëfÅjÉÇÉfÉã----------
	/*StaticSystem model0 = StaticSystem(2, 2);
	model0.ImportNode("Data/Input/Structure/TriangleBeam/Node.csv");
	model0.ImportElement<Triangle, Triangle, Triangle>({ 0, 1 }, "Data/Input/Structure/TriangleBeam/Element.csv");
	model0.ImportParameter({ 0, 1, 2 }, "Data/Input/Structure/TriangleBeam/Parameter.csv");
	model0.ImportField<LinearField>({ 0, 1 });
	model0.ImportEquation<PlaneStrain, GaussTriangle>(0, {}, { 0, 1, 2 }, "Data/Input/Structure/TriangleBeam/Equation.csv");
	model0.ImportDirichlet("Data/Input/Structure/TriangleBeam/Dirichlet.csv");
	model0.ImportNeumann(0, "Data/Input/Structure/TriangleBeam/Neumann.csv");
	model0.Schedule();
	model0.Show();
	*/

	//----------íºó¿Åiéläpå`óvëfÅjÉÇÉfÉã----------
	/*StaticSystem model1 = StaticSystem(2, 2);
	model1.ImportNode("Data/Input/Structure/QuadrangleBeam/Node.csv");
	model1.ImportElement<Quadrangle, Quadrangle, Quadrangle>({ 0, 1 }, "Data/Input/Structure/QuadrangleBeam/Element.csv");
	model1.ImportParameter({ 0, 1, 2 }, "Data/Input/Structure/QuadrangleBeam/Parameter.csv");
	model1.ImportField<LinearField>({ 0, 1 });
	model1.ImportEquation<PlaneStrain, GaussSquare>(0, {}, { 0, 1, 2 }, "Data/Input/Structure/QuadrangleBeam/Equation.csv");
	model1.ImportDirichlet("Data/Input/Structure/QuadrangleBeam/Dirichlet.csv");
	model1.ImportNeumann(0, "Data/Input/Structure/QuadrangleBeam/Neumann.csv");
	model1.Schedule();
	//model1.Show();
	model1.Export("Data/Output/model1");
	*/

	//----------ã»Ç™ÇËó¿ÉÇÉfÉã----------
	/*StaticSystem model2 = StaticSystem(2, 2);
	model2.ImportNode("Data/Input/Structure/CurveBeam/Node.csv");
	model2.ImportElement<Quadrangle, Quadrangle, Quadrangle>({ 0, 1 }, "Data/Input/Structure/CurveBeam/Element.csv");
	model2.ImportParameter({ 0, 1, 2 }, "Data/Input/Structure/CurveBeam/Parameter.csv");
	model2.ImportField<LinearField>({ 0, 1 });
	model2.ImportEquation<PlaneStrain, GaussSquare>(0, {}, { 0, 1, 2 }, "Data/Input/Structure/CurveBeam/Equation.csv");
	model2.ImportDirichlet("Data/Input/Structure/CurveBeam/Dirichlet.csv");
	model2.ImportNeumann(0, "Data/Input/Structure/CurveBeam/Neumann.csv");
	model2.Schedule();
	//model2.Show();
	model2.Export("Data/Output/model2");
	*/

	//----------îMì`ì±ÉÇÉfÉã----------
	/*StaticSystem model3 = StaticSystem(2, 1);
	model3.ImportNode("Data/Input/HeatTransfer/Plate/Node.csv");
	model3.ImportElement<Quadrangle, Quadrangle>({ 0 }, "Data/Input/HeatTransfer/Plate/Element.csv");
	model3.ImportParameter({ 0 }, "Data/Input/HeatTransfer/Plate/Parameter.csv");
	model3.ImportField<LinearField>({ 0 });
	model3.ImportEquation<HeatTransferStatic2D, GaussSquare>(0, {}, { 0 }, "Data/Input/HeatTransfer/Plate/Equation.csv");
	model3.ImportDirichlet("Data/Input/HeatTransfer/Plate/Dirichlet.csv");
	model3.ImportNeumann(0, "Data/Input/HeatTransfer/Plate/Neumann.csv");
	model3.Schedule();
	model3.Export("Data/Output/model3");
	*/

	//----------îMÇÕÇËã»Ç∞ÉÇÉfÉã----------
	std::string model4_path = "Data/Input/HeatAndStructure/StaticHeatBeam/";
	StaticSystem model4 = StaticSystem(2, 3);
	model4.ImportNode(model4_path + "Node.csv");
	model4.ImportElement<Quadrangle, Quadrangle, Quadrangle, Quadrangle>({ 0, 1, 2 }, model4_path + "Element.csv");
	model4.ImportParameter({ 0, 1, 2, 3, 4 }, model4_path + "Parameter.csv");
	model4.ImportField<LinearField>({ 2 });		//îMì`ì±èÍ
	model4.ImportEquation<HeatTransferStatic2D, GaussSquare>(0, {}, { 4 }, model4_path + "Equation.csv");
	model4.ImportField<LinearField>({ 0, 1 });	//îMâûóÕäwèÍ
	model4.ImportEquation<PlaneStrainWithHeat, GaussSquare>(1, { 2 }, { 0, 1, 2, 3 }, model4_path + "Equation.csv");
	model4.ImportDirichlet(model4_path + "Dirichlet.csv");
	model4.ImportNeumann(0, model4_path + "Neumann2.csv");
	model4.ImportNeumann(1, model4_path + "Neumann1.csv");
	model4.Schedule();
	model4.Export("Data/Output/model4");
	
	return 0;
}