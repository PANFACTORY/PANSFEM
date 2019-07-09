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
#include "DirectAnalysis/ShapeFunction/Parametric/Quadrangle2/Quadrangle2.h"


#include "DirectAnalysis/Equation/Phenomenon/Structure/PlaneStrain/PlaneStrain.h"
#include "DirectAnalysis/Equation/Phenomenon/HeatTransfer/Static2D/HeatTransferStatic2D.h"
#include "DirectAnalysis/Equation/Phenomenon/Structure/PlaneStrainWithHeat/PlaneStrainWithHeat.h"
#include "DirectAnalysis/Equation/Phenomenon/HeatTransfer/Dynamic2D/HeatTransferDynamic2D.h"


#include "DirectAnalysis/Integration/Gauss/Triangle/GaussTriangle.h"
#include "DirectAnalysis/Integration/Gauss/Square/GaussSquare.h"
#include "DirectAnalysis/Integration/Gauss/Square2/GaussSquare2.h"


//**********逆解析のサンプル用**********
#include "InverseAnalysis/System/OptimizedSystem.h"
#include "InverseAnalysis/Equation/Structure/OptimizedPlaneStrain/OptimizedPlaneStrain.h"
#include "InverseAnalysis/Function/Structure/Compliance/Compliance.h"
#include "InverseAnalysis/Function/Structure/Weight/Weight.h"


using namespace PANSFEM;


int main() {
	//----------直梁（三角形要素）モデル----------
	std::string model0_path = "Data/Input/Structure/TriangleBeam/";
	/*StaticSystem model0 = StaticSystem(2, 2);
	model0.ImportNode(model0_path + "Node.csv");
	model0.ImportElement<Triangle, Triangle, Triangle>({ 0, 1 }, model0_path + "Element.csv");
	model0.ImportParameter({ 0, 1, 2 }, model0_path + "Parameter.csv");
	model0.ImportField<LinearField>({ 0, 1 });
	model0.ImportEquation<PlaneStrain, GaussTriangle>(0, {}, { 0, 1, 2 }, model0_path + "Equation.csv");
	model0.ImportDirichlet(model0_path + "Dirichlet.csv");
	model0.ImportNeumann(0, model0_path + "Neumann.csv");
	model0.Schedule();
	model0.Show();
	*/

	//----------直梁（四角形要素）モデル----------
	std::string model1_path = "Data/Input/Structure/QuadrangleBeam/";
	/*StaticSystem model1 = StaticSystem(2, 2);
	model1.ImportNode(model1_path + "Node.csv");
	model1.ImportElement<Quadrangle, Quadrangle, Quadrangle>({ 0, 1 }, model1_path + "Element.csv");
	model1.ImportParameter({ 0, 1, 2 }, model1_path + "Parameter.csv");
	model1.ImportField<LinearField>({ 0, 1 });
	model1.ImportEquation<PlaneStrain, GaussSquare>(0, {}, { 0, 1, 2 }, model1_path + "Equation.csv");
	model1.ImportDirichlet(model1_path + "Dirichlet.csv");
	model1.ImportNeumann(0, model1_path + "Neumann.csv");
	model1.Schedule();
	//model1.Show();
	model1.Export("Data/Output/model1");
	*/

	//----------曲がり梁モデル----------
	std::string model2_path = "Data/Input/Structure/CurveBeam/";
	/*StaticSystem model2 = StaticSystem(2, 2);
	model2.ImportNode(model2_path + "Node.csv");
	model2.ImportElement<Quadrangle, Quadrangle, Quadrangle>({ 0, 1 }, model2_path + "Element.csv");
	model2.ImportParameter({ 0, 1, 2 }, model2_path + "Parameter.csv");
	model2.ImportField<LinearField>({ 0, 1 });
	model2.ImportEquation<PlaneStrain, GaussSquare>(0, {}, { 0, 1, 2 }, model2_path + "Equation.csv");
	model2.ImportDirichlet(model2_path + "Dirichlet.csv");
	model2.ImportNeumann(0, model2_path + "Neumann.csv");
	model2.Schedule();
	//model2.Show();
	model2.Export("Data/Output/model2");
	*/

	//----------定常熱伝導モデル----------
	std::string model3_path = "Data/Input/HeatTransfer/Plate/";
	/*StaticSystem model3 = StaticSystem(2, 1);
	model3.ImportNode(model3_path + "Node.csv");
	model3.ImportElement<Quadrangle, Quadrangle>({ 0 }, model3_path + "Element.csv");
	model3.ImportParameter({ 0 }, model3_path + "Parameter.csv");
	model3.ImportField<LinearField>({ 0 });
	model3.ImportEquation<HeatTransferStatic2D, GaussSquare>(0, {}, { 0 }, model3_path + "Equation.csv");
	model3.ImportDirichlet(model3_path + "Dirichlet.csv");
	model3.ImportNeumann(0, model3_path + "Neumann.csv");
	model3.Schedule();
	model3.Export("Data/Output/model3");
	*/

	//----------熱はり曲げモデル----------
	std::string model4_path = "Data/Input/HeatAndStructure/StaticHeatBeam/";
	/*StaticSystem model4 = StaticSystem(2, 3);
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
	std::string model5_path = "Data/Input/HeatTransfer/Dynamic/";
	/*DynamicSystem model5 = DynamicSystem(2, 1, 1000);
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

	//----------トポロジー最適化モデル（曲がり梁）----------
	//	パラメータ
	//		0	:ρ		密度
	//		1	:Emax	Young率最大値
	//		2	:Emin	Young率最小値
	//		3	:V		Poisson比
	//		4	:t		厚さ
	//	従属変数
	//		0	:ux		x軸方向変位
	//		1	:uy		y軸方向変位
	//	設計パラメータ
	//		0	:ρ		密度（パラメータ0番）
	//------------------------------------------------------
	/*std::string model7_path = "Data/Input/Optimize/QuadrangleBeam/";
	OptimizedSystem model7 = OptimizedSystem(2, 2, { 0 });
	
	model7.ImportNode(model7_path + "Node.csv");
	model7.ImportElement<Quadrangle, Quadrangle, Quadrangle>({ 0, 1 }, model7_path + "Element.csv");
	model7.ImportParameter({ 0, 1, 2, 3, 4 }, model7_path + "Parameter.csv");
	model7.ImportField<LinearField>({ 0, 1 });
	model7.ImportEquation<OptimizedPlaneStrain, GaussSquare>(0, {}, { 0, 1, 2, 3, 4 }, model7_path + "Equation.csv");
	model7.ImportDirichlet(model7_path + "Dirichlet.csv");
	model7.ImportNeumann(0, model7_path + "Neumann.csv");
	
	model7.ImportOptimizedElement(model7_path + "Equation.csv");
	model7.ImportObjective<Compliance>({ 0, 1 }, { 0, 1, 2, 3, 4 });
	model7.ImportElementToObjective<GaussSquare>(0, model7_path + "Equation.csv");
	model7.ImportConstraint<Weight>({}, { 0, 4 });
	model7.ImportElementToConstraint<GaussSquare>(0, model7_path + "Equation.csv");
	
	model7.Schedule();
	model7.Export("Data/Output/model7");
	*/

	//----------直梁（四角形要素）モデル----------
	std::string model8_path = "Data/Input/Structure/QuadrangleBeam2/";
	StaticSystem model8 = StaticSystem(2, 2);
	model8.ImportNode(model8_path + "Node.csv");
	model8.ImportElement<Quadrangle2, Quadrangle2, Quadrangle2>({ 0, 1 }, model8_path + "Element.csv");
	model8.ImportParameter({ 0, 1, 2 }, model8_path + "Parameter.csv");
	model8.ImportField<LinearField>({ 0, 1 });
	model8.ImportEquation<PlaneStrain, GaussSquare2>(0, {}, { 0, 1, 2 }, model8_path + "Equation.csv");
	model8.ImportDirichlet(model8_path + "Dirichlet.csv");
	model8.ImportNeumann(0, model8_path + "Neumann.csv");
	model8.Schedule();
	model8.Show();
	//model8.Export("Data/Output/model8");
	

	return 0;
}