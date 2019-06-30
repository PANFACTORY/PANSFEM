//*****************************************************************************
//Title		:PANSFEM/main.cpp
//Author	:Tanabe Yuta
//Date		:2019/06/08
//Copyright	:(C)2019 TanabeYuta
//*****************************************************************************


#include <vector>


//**********順解析のサンプル用**********
#include "DirectAnalysis/System/StaticSystem/StaticSystem.h"
#include "DirectAnalysis/System/DynamicSystem/DynamicSystem.h"


#include "DirectAnalysis/Field/LinearField/LinearField.h"


#include "DirectAnalysis/ShapeFunction/Parametric/Triangle/Triangle.h"
#include "DirectAnalysis/ShapeFunction/Parametric/Quadrangle/Quadrangle.h"


#include "DirectAnalysis/Equation/Phenomenon/Structure/PlaneStrain/PlaneStrain.h"
#include "DirectAnalysis/Equation/Phenomenon/HeatTransfer/Static2D/HeatTransferStatic2D.h"
#include "DirectAnalysis/Equation/Phenomenon/Structure/PlaneStrainWithHeat/PlaneStrainWithHeat.h"
#include "DirectAnalysis/Equation/Phenomenon/HeatTransfer/Dynamic2D/HeatTransferDynamic2D.h"


#include "DirectAnalysis/Integration/Gauss/Square/GaussSquare.h"
#include "DirectAnalysis/Integration/Gauss/Triangle/GaussTriangle.h"


//**********逆解析のサンプル用**********
#include "InverseAnalysis/System/OptimizedSystem.h"
#include "InverseAnalysis/Function/Function.h"


using namespace PANSFEM;


int main() {
	//----------直梁（三角形要素）モデル----------
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

	//----------直梁（四角形要素）モデル----------
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

	//----------曲がり梁モデル----------
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

	//----------定常熱伝導モデル----------
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

	//----------熱はり曲げモデル----------
	/*std::string model4_path = "Data/Input/HeatAndStructure/StaticHeatBeam/";
	StaticSystem model4 = StaticSystem(2, 3);
	model4.ImportNode(model4_path + "Node.csv");
	model4.ImportElement<Quadrangle, Quadrangle, Quadrangle, Quadrangle>({ 0, 1, 2 }, model4_path + "Element.csv");
	model4.ImportParameter({ 0, 1, 2, 3, 4 }, model4_path + "Parameter.csv");
	model4.ImportField<LinearField>({ 2 });		//熱伝導場
	model4.ImportEquation<HeatTransferStatic2D, GaussSquare>(0, {}, { 4 }, model4_path + "Equation.csv");
	model4.ImportField<LinearField>({ 0, 1 });	//熱応力場
	model4.ImportEquation<PlaneStrainWithHeat, GaussSquare>(1, { 2 }, { 0, 1, 2, 3 }, model4_path + "Equation.csv");
	model4.ImportDirichlet(model4_path + "Dirichlet.csv");
	model4.ImportNeumann(0, model4_path + "Neumann2.csv");
	model4.ImportNeumann(1, model4_path + "Neumann1.csv");
	model4.Schedule();
	model4.Export("Data/Output/model4");
	*/

	//----------非定常熱伝導モデル----------
	/*std::string model5_path = "Data/Input/HeatTransfer/Dynamic/";
	DynamicSystem model5 = DynamicSystem(2, 1, 1000);
	model5.ImportNode(model5_path + "Node.csv");
	model5.ImportElement<Quadrangle, Quadrangle>({ 0 }, model5_path + "Element.csv");
	model5.ImportParameter({ 0, 1, 2, 3, 4 }, model5_path + "Parameter.csv");
	model5.ImportField<LinearField>({ 0 });
	model5.ImportEquation<HeatTransferDynamic2D, GaussSquare>(0, {}, { 0, 1, 2, 3, 4 }, model5_path + "Equation.csv");
	model5.ImportDirichlet(model5_path + "Dirichlet.csv");
	model5.ImportNeumann(0, model5_path + "Neumann.csv");
	model5.Schedule();
	model5.Export("Data/Output/model5");
	*/

	//----------トポロジー最適化モデル----------
	std::string model7_path = "Data/Input/Optimize/CurveBeam/";
	OptimizedSystem model7 = OptimizedSystem(2, 2);
	model7.ImportNode(model7_path + "Node.csv");

	return 0;
}