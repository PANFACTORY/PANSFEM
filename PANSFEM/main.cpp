﻿//*****************************************************************************
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
#include "DirectAnalysis/ShapeFunction/Parametric/Cubic/Cubic.h"
#include "DirectAnalysis/ShapeFunction/Parametric/Cubic2/Cubic2.h"


#include "DirectAnalysis/Equation/Phenomenon/Structure/PlaneStrain/PlaneStrain.h"
#include "DirectAnalysis/Equation/Phenomenon/HeatTransfer/Static2D/HeatTransferStatic2D.h"
#include "DirectAnalysis/Equation/Phenomenon/Structure/PlaneStrainWithHeat/PlaneStrainWithHeat.h"
#include "DirectAnalysis/Equation/Phenomenon/HeatTransfer/Dynamic2D/HeatTransferDynamic2D.h"
#include "DirectAnalysis/Equation/Phenomenon/Structure/IsotropicElastic/IsotropicElastic.h"
#include "DirectAnalysis/Equation/StructuralElement/Beam2D/Beam2D.h"


#include "DirectAnalysis/Integration/Gauss/Triangle/GaussTriangle.h"
#include "DirectAnalysis/Integration/Gauss/Square/GaussSquare.h"
#include "DirectAnalysis/Integration/Gauss/Square2/GaussSquare2.h"
#include "DirectAnalysis/Integration/Gauss/Cubic/GaussCubic.h"
#include "DirectAnalysis/Integration/Gauss/Cubic2/GaussCubic2.h"


//**********逆解析のサンプル用**********
#include "InverseAnalysis/System/OptimizedSystem.h"
#include "InverseAnalysis/Equation/Structure/OptimizedPlaneStrain/OptimizedPlaneStrain.h"
#include "InverseAnalysis/Function/Structure/Compliance/Compliance.h"
#include "InverseAnalysis/Function/Structure/Weight/Weight.h"


using namespace PANSFEM;


int main() {
	std::cout << "--------------------This is PANSFEM.--------------------" << std::endl;

	//----------直梁（三角形要素）モデル----------
	std::string model0_path = "Data/Input/Structure/TriangleBeam/";
	/*StaticSystem model0 = StaticSystem(2, 2);
	model0.ImportNode(model0_path + "Node.csv");
	model0.ImportElement<Triangle, Triangle, Triangle>({ 0, 1 }, 5, model0_path + "Element.csv");
	model0.ImportParameter({ 0, 1, 2 }, model0_path + "Parameter.csv");
	model0.ImportField<LinearField>({ 0, 1 });
	model0.ImportEquation<PlaneStrain, GaussTriangle>(0, {}, { 0, 1, 2 }, model0_path + "Equation.csv");
	model0.ImportDirichlet(model0_path + "Dirichlet.csv");
	model0.ImportNeumann(0, model0_path + "Neumann.csv");
	model0.Schedule();
	model0.Show();
	model0.Export("Data/Output/model0");
	*/

	//----------直梁（四角形要素）モデル----------
	std::string model1_path = "Data/Input/Structure/QuadrangleBeam/";
	/*StaticSystem model1 = StaticSystem(2, 2);
	model1.ImportNode(model1_path + "Node.csv");
	model1.ImportElement<Quadrangle, Quadrangle, Quadrangle>({ 0, 1 }, 9, model1_path + "Element.csv");
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
	model2.ImportElement<Quadrangle, Quadrangle, Quadrangle>({ 0, 1 }, 9, model2_path + "Element.csv");
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
	model3.ImportElement<Quadrangle, Quadrangle>({ 0 }, 9, model3_path + "Element.csv");
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
	model4.ImportElement<Quadrangle, Quadrangle, Quadrangle, Quadrangle>({ 0, 1, 2 }, 9, model4_path + "Element.csv");
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
	DynamicSystem model5 = DynamicSystem(2, 1, 1000, "Data/Output/model5/model5");
	model5.ImportNode(model5_path + "Node.csv");
	model5.ImportElement<Quadrangle, Quadrangle>({ 0 }, 9, model5_path + "Element.csv");
	model5.ImportParameter({ 0, 1, 2, 3, 4 }, model5_path + "Parameter.csv");
	model5.ImportField<LinearField>({ 0 });
	model5.ImportEquation<HeatTransferDynamic2D, GaussSquare>(0, {}, { 0, 1, 2, 3, 4 }, model5_path + "Equation.csv");
	model5.ImportDirichlet(model5_path + "Dirichlet.csv");
	model5.ImportNeumann(0, model5_path + "Neumann.csv");
	model5.Schedule();
	model5.Export("Data/Output/model5");
	*/

	//----------トポロジー最適化モデル（2D曲がり梁）----------
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
	//
	//	N=59004
	//	NB=5974
	//------------------------------------------------------
	std::string model6_path = "Data/Input/Optimize/CurveBeam/";
	/*OptimizedSystem model6 = OptimizedSystem(2, 2, { 0 });
	
	model6.ImportNode(model6_path + "Node.csv");
	model6.ImportElement<Quadrangle2, Quadrangle2, Quadrangle2>({ 0, 1 }, 9, model6_path + "Element.csv");
	model6.ImportParameter({ 0, 1, 2, 3, 4 }, model6_path + "Parameter.csv");
	model6.ImportField<LinearField>({ 0, 1 });
	model6.ImportEquation<OptimizedPlaneStrain, GaussSquare2>(0, {}, { 0, 1, 2, 3, 4 }, model6_path + "Equation.csv");
	model6.ImportDirichlet(model6_path + "Dirichlet.csv");
	model6.ImportNeumann(0, model6_path + "Neumann.csv");
	
	model6.ImportOptimizedElement(model6_path + "Equation.csv");
	model6.ImportObjective<Compliance>({ 0, 1 }, { 0, 1, 2, 3, 4 });
	model6.ImportElementToObjective<GaussSquare2>(0, model6_path + "Equation.csv");
	model6.ImportConstraint<Weight>({}, { 0, 4 });
	model6.ImportElementToConstraint<GaussSquare2>(0, model6_path + "Equation.csv");
	
	model6.Schedule();
	model6.Export("Data/Output/model6");
	*/

	//----------直梁（四角形要素）モデル----------
	std::string model7_path = "Data/Input/Structure/QuadrangleBeam2/";
	/*StaticSystem model7 = StaticSystem(2, 2);
	model7.ImportNode(model7_path + "Node.csv");
	model7.ImportElement<Quadrangle2, Quadrangle2, Quadrangle2>({ 0, 1 }, 23, model7_path + "Element.csv");
	model7.ImportParameter({ 0, 1, 2 }, model7_path + "Parameter.csv");
	model7.ImportField<LinearField>({ 0, 1 });
	model7.ImportEquation<PlaneStrain, GaussSquare2>(0, {}, { 0, 1, 2 }, model7_path + "Equation.csv");
	model7.ImportDirichlet(model7_path + "Dirichlet.csv");
	model7.ImportNeumann(0, model7_path + "Neumann.csv");
	model7.Schedule();
	//model7.Show();
	model7.Export("Data/Output/model7");
	*/

	//----------直梁（直方体要素）モデル----------
	std::string model8_path = "Data/Input/Structure/CubicBeam/";
	/*StaticSystem model8 = StaticSystem(3, 3);
	model8.ImportNode(model8_path + "Node.csv");
	model8.ImportElement<Cubic, Cubic, Cubic, Cubic>({ 0, 1, 2 }, 12, model8_path + "Element.csv");
	model8.ImportParameter({ 0, 1 }, model8_path + "Parameter.csv");
	model8.ImportField<LinearField>({ 0, 1, 2 });
	model8.ImportEquation<IsotropicElastic, GaussCubic>(0, {}, { 0, 1 }, model8_path + "Equation.csv");
	model8.ImportDirichlet(model8_path + "Dirichlet.csv");
	model8.ImportNeumann(0, model8_path + "Neumann.csv");
	model8.Schedule();
	//model8.Show();
	model8.Export("Data/Output/model8");
	*/

	//----------直梁（直方体2次要素）モデル----------
	//	N = 167680
	//	NB= 84991
	//  ☞114GB

	//Sorted
	//	N = 167680
	//	NB= 10438
	//	☞14GB
	std::string model9_path = "Data/Input/Structure/CubicBeam2/";
	/*StaticSystem model9 = StaticSystem(3, 3);
	model9.ImportNode(model9_path + "Node.csv");
	model9.ImportElement<Cubic2, Cubic2, Cubic2, Cubic2>({ 0, 1, 2 }, 25, model9_path + "Element.csv");
	model9.ImportParameter({ 0, 1 }, model9_path + "Parameter.csv");
	model9.ImportField<LinearField>({ 0, 1, 2 });
	model9.ImportEquation<IsotropicElastic, GaussCubic2>(0, {}, { 0, 1 }, model9_path + "Equation.csv");
	model9.ImportDirichlet(model9_path + "Dirichlet.csv");
	model9.ImportNeumann(0, model9_path + "Neumann.csv");
	model9.Schedule();
	//model9.Show();
	model9.Export("Data/Output/model9");
	*/

	//----------二次元ラーメンモデル----------
	std::string model10_path = "Data/Input/StructuralElement/Ramen/";
	/*StaticSystem model10 = StaticSystem(2, 3);
	model10.ImportNode(model10_path + "Node.csv");
	model10.ImportElement<Cubic, Cubic, Cubic, Cubic>({ 0, 1, 2 }, 3, model10_path + "Element.csv");
	model10.ImportParameter({ 0, 1, 2 }, model10_path + "Parameter.csv");
	model10.ImportField<LinearField>({ 0, 1, 2 });
	model10.ImportEquation<Beam2D, GaussCubic>(0, {}, { 0, 1, 2 }, model10_path + "Equation.csv");
	model10.ImportDirichlet(model10_path + "Dirichlet.csv");
	model10.ImportNeumann(0, model10_path + "Neumann.csv");
	model10.Schedule();
	//model10.Show();
	model10.Export("Data/Output/model10");
	*/

	//----------トポロジー最適化モデル（2D曲がり梁）----------
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
	//
	//	N=59004
	//	NB=5974
	//------------------------------------------------------
	std::string model11_path = "Data/Input/Optimize/Bridge/";
	/*OptimizedSystem model11 = OptimizedSystem(2, 2, { 0 });
	//StaticSystem model11 = StaticSystem(2, 2);
	model11.ImportNode(model11_path + "Node2.csv");
	model11.ImportElement<Quadrangle2, Quadrangle2, Quadrangle2>({ 0, 1 }, 9, model11_path + "Element2.csv");
	model11.ImportParameter({ 0, 1, 2, 3, 4 }, model11_path + "Parameter.csv");
	model11.ImportField<LinearField>({ 0, 1 });
	model11.ImportEquation<OptimizedPlaneStrain, GaussSquare2>(0, {}, { 0, 1, 2, 3, 4 }, model11_path + "Equation.csv");
	model11.ImportDirichlet(model11_path + "Dirichlet.csv");
	model11.ImportNeumann(0, model11_path + "Neumann.csv");
	
	model11.ImportOptimizedElement(model11_path + "Equation.csv");
	model11.ImportObjective<Compliance>({ 0, 1 }, { 0, 1, 2, 3, 4 });
	model11.ImportElementToObjective<GaussSquare2>(0, model11_path + "Equation.csv");
	model11.ImportConstraint<Weight>({}, { 0, 4 });
	model11.ImportElementToConstraint<GaussSquare2>(0, model11_path + "Equation.csv");
	
	model11.Schedule();
	model11.Export("Data/Output/model11");
	*/

	//----------梁モデル（ビーム要素）----------
	std::string model12_path = "Data/Input/StructuralElement/Beam/";
	StaticSystem model12 = StaticSystem(2, 3);
	model12.ImportNode(model12_path + "Node.csv");
	model12.ImportElement<Cubic, Cubic, Cubic, Cubic>({ 0, 1, 2 }, 3, model12_path + "Element.csv");
	model12.ImportParameter({ 0, 1, 2 }, model12_path + "Parameter.csv");
	model12.ImportField<LinearField>({ 0, 1, 2 });
	model12.ImportEquation<Beam2D, GaussCubic>(0, {}, { 0, 1, 2 }, model12_path + "Equation.csv");
	model12.ImportDirichlet(model12_path + "Dirichlet.csv");
	model12.ImportNeumann(0, model12_path + "Neumann.csv");
	model12.Schedule();
	model12.Show();
	model12.Export("Data/Output/model12");
	

	std::cout << "--------------------Finish--------------------" << std::endl;
	return 0;
}