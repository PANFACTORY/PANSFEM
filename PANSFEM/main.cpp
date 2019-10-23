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
#include "DirectAnalysis/Field/EigenvalueField/EigenvalueField.h"
#include "DirectAnalysis/Field/NonLinearField/NonLinearField.h"


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
#include "DirectAnalysis/Equation/Phenomenon/Structure/PlaneStrainVibration/PlaneStrainVibration.h"
#include "DirectAnalysis/Equation/StructuralElement/ElasticPlasticTruss2D/ElasticPlasticTruss2D.h"


#include "DirectAnalysis/Integration/Gauss/Triangle/GaussTriangle.h"
#include "DirectAnalysis/Integration/Gauss/Square/GaussSquare.h"
#include "DirectAnalysis/Integration/Gauss/Square2/GaussSquare2.h"
#include "DirectAnalysis/Integration/Gauss/Cubic/GaussCubic.h"
#include "DirectAnalysis/Integration/Gauss/Cubic2/GaussCubic2.h"


//**********逆解析のサンプル用**********
#include "InverseAnalysis/System/OCSystem/OCSystem.h"
#include "InverseAnalysis/Equation/Structure/OptimizedPlaneStrain/OptimizedPlaneStrain.h"
#include "InverseAnalysis/Function/Structure/Compliance/Compliance.h"
#include "InverseAnalysis/Function/Structure/Weight/Weight.h"
#include "InverseAnalysis/Equation/Structure/OptimizedAlternative2D/OptimizedAlternative2D.h"
#include "InverseAnalysis/Function/Structure/Compliance/ComplianceAlternative.h"
#include "InverseAnalysis/Function/Structure/Weight/WeightAlternative.h"
#include "InverseAnalysis/Equation/Structure/OptimizedAlternative2D/OptimizedAlternative3Phase2D.h"
#include "InverseAnalysis/Function/Structure/Compliance/ComplianceAlternative3Phase.h"
#include "InverseAnalysis/Function/Structure/Weight/WeightAlternative3Phase.h"
#include "InverseAnalysis/System/CONLINSystem/CONLINSystem.h"


#include "InverseAnalysis/Equation/Structure/OptimizedIsotropicElastic/OptimizedIsotropicElastic.h"
#include "InverseAnalysis/Function/Structure/Compliance/ComplianceIsotropicElastic.h"
#include "InverseAnalysis/Function/Structure/Weight/WeightIsotropicElastic.h"


#include "InverseAnalysis/Equation/Structure/OptimizedAlternative3D/OptimizedAlternative3D.h"
#include "InverseAnalysis/Function/Structure/Compliance/ComplianceAlternative3D.h"
#include "InverseAnalysis/Function/Structure/Weight/WeightAlternative3D.h"


#include "InverseAnalysis/Equation/Structure/Optimized3PhaseMaterials3D/Optimized3PhaseMaterials3D.h"
#include "InverseAnalysis/Function/Structure/Compliance/3PhaseMaterials3D/Compliance3PhaseMaterials3D.h"
#include "InverseAnalysis/Function/Structure/Weight/3PhaseMaterials3D/Weight3PhaseMaterials3D.h"


#include "InverseAnalysis/Equation/Structure/Optimized2PhaseMaterials3D/Optimized2PhaseMaterials3D.h"
#include "InverseAnalysis/Function/Structure/Compliance/2PhaseMaterials3D/Compliance2PhaseMaterials3D.h"
#include "InverseAnalysis/Function/Structure/Weight/2PhaseMaterials3D/Weight2PhaseMaterials3D.h"


using namespace PANSFEM;


int main() {
	std::cout << "--------------------This is PANSFEM.--------------------" << std::endl;

	//----------直梁（三角形要素）モデル----------
	std::string model0_path = "Data/Input/Structure/TriangleBeam/";
	/*StaticSystem model0 = StaticSystem(2, 2);
	model0.ImportNode(model0_path + "Node.csv");
	model0.ImportParameter({ 0, 1, 2 }, model0_path + "Parameter.csv");
	model0.ImportElement<Triangle, Triangle, Triangle>({ 0, 1 }, 5, model0_path + "Element.csv");
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
	model1.ImportParameter({ 0, 1, 2 }, model1_path + "Parameter.csv");
	model1.ImportElement<Quadrangle, Quadrangle, Quadrangle>({ 0, 1 }, 9, model1_path + "Element.csv");
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
	model4.ImportParameter({ 0, 1, 2, 3, 4 }, model4_path + "Parameter.csv");
	model4.ImportElement<Quadrangle, Quadrangle, Quadrangle, Quadrangle>({ 0, 1, 2 }, 9, model4_path + "Element.csv");
	model4.ImportField<LinearField>({ 2 });		//熱伝導場
	model4.ImportEquation<HeatTransferStatic2D, GaussSquare>(0, {}, { 4 }, model4_path + "Equation.csv");
	model4.ImportField<LinearField>({ 0, 1 });	//熱応力場
	model4.ImportEquation<PlaneStrainWithHeat, GaussSquare>(1, { 2 }, { 0, 1, 2, 3 }, model4_path + "Equation.csv");
	model4.ImportDirichlet(model4_path + "Dirichlet.csv");
	model4.ImportNeumann(model4_path + "Neumann2.csv");
	model4.ImportNeumann(model4_path + "Neumann1.csv");
	model4.Schedule();
	model4.Export("Data/Output/model4");
	*/

	//----------非定常熱伝導モデル----------
	std::string model5_path = "Data/Input/HeatTransfer/Dynamic/";
	/*DynamicSystem model5 = DynamicSystem(2, 1, 1000, "Data/Output/model5/model5");
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

	//----------トポロジー最適化モデル（梁）----------
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
	std::string model6_path = "Data/Input/Optimize/QuadrangleBeam/";
	/*OCSystem model6 = OCSystem(2, 2, { 0 });
	
	model6.ImportNode(model6_path + "Node.csv");
	model6.ImportParameter({ 0, 1, 2, 3, 4 }, model6_path + "Parameter.csv");
	model6.ImportElement<Quadrangle2, Quadrangle2, Quadrangle2>({ 0, 1 }, 23, model6_path + "Element.csv");
	model6.ImportField<LinearField>({ 0, 1 });
	model6.ImportEquation<OptimizedPlaneStrain, GaussSquare2>(0, {}, { 0, 1, 2, 3, 4 }, model6_path + "Equation.csv");
	model6.ImportDirichlet(model6_path + "Dirichlet.csv");
	model6.ImportNeumann(model6_path + "Neumann.csv");
	
	model6.ImportOptimizedParameter(model6_path + "Equation.csv");
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

	//CSR format
	//	N = 167680
	//	data = 27010462
	//	☞325MB
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

	//----------トポロジー最適化モデル（2D橋）----------
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
	/*StaticSystem model12 = StaticSystem(2, 3);
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
	*/

	//----------固有値解析（平面ひずみモデル）----------
	std::string model13_path = "Data/Input/Vibration/Beam/";
	/*StaticSystem model13 = StaticSystem(2, 3);
	model13.ImportNode(model13_path + "Node.csv");
	model13.ImportElement<Quadrangle2, Quadrangle2, Quadrangle2>({ 0, 1 }, 23, model13_path + "Element.csv");
	model13.ImportParameter({ 0, 1, 2, 3 }, model13_path + "Parameter.csv");
	model13.ImportField<EigenvalueField>({ 0, 1 });
	model13.ImportEquation<PlaneStrainVibration, GaussSquare2>(0, {}, { 0, 1, 2, 3 }, model13_path + "Equation.csv");
	model13.ImportDirichlet(model13_path + "Dirichlet.csv");
	model13.Schedule();
	//model13.Show();
	model13.Export("Data/Output/model13");
	*/

	//----------トポロジー最適化モデル（代替要素梁）----------
	//	パラメータ
	//		0	:s		設計変数
	//		1	:E		材料Young率
	//		2	:l		Lattice梁の対角半長さ
	//		3	:rho	材料密度
	//		4	:dmax	Lattice梁最大幅
	//		5	:dmin	Lattice梁最小幅
	//		6	:t		Lattice梁厚さ
	//	従属変数
	//		0	:ux		x軸方向変位
	//		1	:uy		y軸方向変位
	//	設計パラメータ
	//		0	:s		密度（パラメータ0番）
	//------------------------------------------------------
	std::string model14_path = "Data/Input/Optimize/AlternativeBeam/";
	/*OptimizedSystem model14 = OptimizedSystem(2, 2, { 0 });
	model14.ImportNode(model14_path + "Node.csv");
	model14.ImportElement<Quadrangle2, Quadrangle2, Quadrangle2>({ 0, 1 }, 23, model14_path + "Element.csv");
	model14.ImportParameter({ 0, 1, 2, 3, 4, 5, 6 }, model14_path + "Parameter.csv");
	model14.ImportField<LinearField>({ 0, 1 });
	model14.ImportEquation<OptimizedAlternative2D, GaussSquare2>(0, {}, { 0, 1, 2, 4, 5, 6 }, model14_path + "Equation.csv");
	model14.ImportDirichlet(model14_path + "Dirichlet.csv");
	model14.ImportNeumann(0, model14_path + "Neumann.csv");

	model14.ImportOptimizedElement(model14_path + "Equation.csv");
	model14.ImportObjective<ComplianceAlternative>({ 0, 1 }, { 0, 1, 2, 4, 5, 6 });
	model14.ImportElementToObjective<GaussSquare2>(0, model14_path + "Equation.csv");
	model14.ImportConstraint<WeightAlternative>({}, { 0, 2, 3, 4, 5, 6 });
	model14.ImportElementToConstraint<GaussSquare2>(0, model14_path + "Equation.csv");

	model14.Schedule();
	//model14.Show();
	model14.Export("Data/Output/model14");
	*/

	//----------トポロジー最適化モデル（代替要素梁三相モデル）----------
	//	パラメータ
	//		0	:s0		設計変数0
	//		1	:s1		設計変数1
	//		2	:E		材料Young率
	//		3	:l		Lattice梁の対角半長さ
	//		4	:rho	材料密度
	//		5	:dmax	Lattice梁最大幅
	//		6	:dmin	Lattice梁最小幅
	//		7	:d0 	Lattice梁0幅
	//		8	:t		Lattice梁厚さ
	//	従属変数
	//		0	:ux		x軸方向変位
	//		1	:uy		y軸方向変位
	//	設計パラメータ
	//		0	:s0		正規化梁幅0（パラメータ0番）
	//		1	:s1		正規化梁幅1（パラメータ1番）
	//------------------------------------------------------
	/*std::string model15_path = "Data/Input/Optimize/AlternativeBeam/";
	OCSystem model15 = OCSystem(2, 2, { 0, 1 });
	model15.ImportNode(model15_path + "Node.csv");
	model15.ImportElement<Quadrangle2, Quadrangle2, Quadrangle2>({ 0, 1 }, 23, model15_path + "Element.csv");
	model15.ImportParameter({ 0, 1, 2, 3, 4, 5, 6, 7, 8 }, model15_path + "Parameter.csv");
	model15.ImportField<LinearField>({ 0, 1 });
	model15.ImportEquation<OptimizedAlternative3Phase2D, GaussSquare2>(0, {}, { 0, 1, 2, 3, 5, 6, 7, 8 }, model15_path + "Equation.csv");
	model15.ImportDirichlet(model15_path + "Dirichlet.csv");
	model15.ImportNeumann(0, model15_path + "Neumann.csv");

	model15.ImportOptimizedElement(model15_path + "Equation.csv");
	model15.ImportObjective<ComplianceAlternative3Phase>({ 0, 1 }, { 0, 1, 2, 3, 5, 6, 7, 8 });
	model15.ImportElementToObjective<GaussSquare2>(0, model15_path + "Equation.csv");
	model15.ImportConstraint<WeightAlternative3Phase>({}, { 0, 1, 3, 4, 5, 6, 7, 8 });
	model15.ImportElementToConstraint<GaussSquare2>(0, model15_path + "Equation.csv");

	model15.Schedule();
	//model15.Show();
	model15.Export("Data/Output/model15");
	*/

	//----------固有値解析（平面ひずみモデル）----------
	std::string model16_path = "Data/Input/Optimize/Vibration/";
	/*StaticSystem model16 = StaticSystem(2, 2);
	model16.ImportNode(model16_path + "Node.csv");
	model16.ImportElement<Quadrangle2, Quadrangle2, Quadrangle2>({ 0, 1 }, 23, model16_path + "Element.csv");
	model16.ImportParameter({ 0, 1, 2, 3 }, model16_path + "Parameter.csv");
	model16.ImportField<EigenvalueField>({ 0, 1 });
	model16.ImportEquation<PlaneStrainVibration, GaussSquare2>(0, {}, { 0, 1, 2, 3 }, model16_path + "Equation.csv");
	model16.ImportDirichlet(model16_path + "Dirichlet.csv");
	model16.Schedule();
	//model16.Show();
	model16.Export("Data/Output/model16");
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
	std::string model17_path = "Data/Input/Optimize/Bridge/";
	/*CONLINSystem model17 = CONLINSystem(2, 2, { 0 });
	model17.ImportNode(model17_path + "Node2.csv");
	model17.ImportElement<Quadrangle2, Quadrangle2, Quadrangle2>({ 0, 1 }, 9, model17_path + "Element2.csv");
	model17.ImportParameter({ 0, 1, 2, 3, 4 }, model17_path + "Parameter.csv");
	model17.ImportField<LinearField>({ 0, 1 });
	model17.ImportEquation<OptimizedPlaneStrain, GaussSquare2>(0, {}, { 0, 1, 2, 3, 4 }, model17_path + "Equation.csv");
	model17.ImportDirichlet(model17_path + "Dirichlet.csv");
	model17.ImportNeumann(0, model17_path + "Neumann.csv");

	model17.ImportOptimizedElement(model17_path + "Equation.csv");
	model17.ImportObjective<Compliance>({ 0, 1 }, { 0, 1, 2, 3, 4 });
	model17.ImportElementToObjective<GaussSquare2>(0, model17_path + "Equation.csv");
	model17.ImportConstraint<Weight>({}, { 0, 4 });
	model17.ImportElementToConstraint<GaussSquare2>(0, model17_path + "Equation.csv");

	model17.Schedule();
	model17.Export("Data/Output/model17");
	*/

	//----------弾塑性トラス（二次元弾塑性トラスモデル）----------
	//	パラメータ
	//		0	:A		要素初期断面積
	//		1	:E		材料Young率
	//		2	:H		材料塑性係数
	//		3	:tauy0	初期降伏応力
	//------------------------------------------------------------
	std::string model18_path = "Data/Input/StructuralElement/ElasticPlasticTruss/";
	/*StaticSystem model18 = StaticSystem(2, 2);
	model18.ImportNode(model18_path + "Node.csv");
	model18.ImportParameter({ 0, 1, 2, 3 }, model18_path + "Parameter.csv");
	model18.ImportElement<Triangle, Triangle, Triangle>({ 0, 1 }, 3, model18_path + "Element.csv");
	model18.ImportField<NonLinearField>({ 0, 1 });
	model18.ImportEquation<ElasticPlasticTruss2D, GaussTriangle>(0, {}, { 0, 1, 2, 3 }, model18_path + "Equation.csv");
	model18.ImportDirichlet(model18_path + "Dirichlet.csv");
	model18.ImportNeumann(model18_path + "Neumann.csv");
	model18.Schedule();
	//model18.Show();
	model18.Export("Data/Output/model18");
	*/

	//----------トポロジー最適化モデル（3D梁）----------
	//	パラメータ
	//		0	:ρ		密度
	//		1	:Emax	Young率最大値
	//		2	:Emin	Young率最小値
	//		3	:V		Poisson比
	//	従属変数
	//		0	:ux		x軸方向変位
	//		1	:uy		y軸方向変位
	//		2	:uz		z軸方向変位
	//	設計パラメータ
	//		0	:ρ		密度（パラメータ0番）
	//
	//------------------------------------------------------
	/*std::string model19_path = "Data/Input/Optimize/Beam3D/";
	OCSystem model19 = OCSystem(3, 3, { 0 });

	model19.ImportNode(model19_path + "Node.csv");
	model19.ImportParameter({ 0, 1, 2, 3 }, model19_path + "Parameter.csv");
	model19.ImportElement<Cubic2, Cubic2, Cubic2, Cubic2>({ 0, 1, 2 }, 25, model19_path + "Element.csv");
	model19.ImportField<LinearField>({ 0, 1, 2 });
	model19.ImportEquation<OptimizedIsotropicElastic, GaussCubic2>(0, {}, { 0, 1, 2, 3 }, model19_path + "Equation.csv");
	model19.ImportDirichlet(model19_path + "Dirichlet.csv");
	model19.ImportNeumann(model19_path + "Neumann.csv");

	model19.ImportOptimizedParameter(model19_path + "Equation.csv");
	model19.ImportObjective<ComplianceIsotropicElastic>({ 0, 1, 2 }, { 0, 1, 2, 3 });
	model19.ImportElementToObjective<GaussCubic2>(0, model19_path + "Equation.csv");
	model19.ImportConstraint<WeightIsotropicElastic>({}, { 0 });
	model19.ImportElementToConstraint<GaussCubic2>(0, model19_path + "Equation.csv");

	model19.Schedule();
	model19.Export("Data/Output/model19");
	*/

	//----------骨の三次元モデル----------
	std::string model20_path = "Data/Input/Structure/Bone/";
	/*StaticSystem model20 = StaticSystem(3, 3);
	model20.ImportNode(model20_path + "Node.csv");
	model20.ImportElement<Cubic, Cubic, Cubic, Cubic>({ 0, 1, 2 }, 12, model20_path + "Element.csv");
	model20.ImportParameter({ 0, 1 }, model20_path + "Parameter.csv");
	model20.ImportField<LinearField>({ 0, 1, 2 });
	model20.ImportEquation<IsotropicElastic, GaussCubic>(0, {}, { 0, 1 }, model20_path + "Equation.csv");
	model20.ImportDirichlet(model20_path + "Dirichlet.csv");
	model20.ImportNeumann(0, model20_path + "Neumann.csv");
	model20.Schedule();
	//model20.Show();
	model20.Export("Data/Output/model20");
	*/

	//----------トポロジー最適化モデル（代替要素3D梁）----------
	//	パラメータ
	//		0	:s		設計変数
	//		1	:dmax	最大梁直径
	//		2	:dmin	最小梁直径
	//		3	:L		Latticeセルサイズ
	//		4	:E		材料Young率	
	//		5	:ρ		材料密度
	//	従属変数
	//		0	:ux		x軸方向変位
	//		1	:uy		y軸方向変位
	//		2	:uz		z軸方向変位
	//	設計パラメータ
	//		0	:s		（パラメータ0番）
	//
	//------------------------------------------------------
	std::string model21_path = "Data/Input/Optimize/AlternativeBeam3D/";
	/*OCSystem model21 = OCSystem(3, 3, { 0 });

	model21.ImportNode(model21_path + "Node.csv");
	model21.ImportParameter({ 0, 1, 2, 3, 4, 5 }, model21_path + "Parameter.csv");
	model21.ImportElement<Cubic2, Cubic2, Cubic2, Cubic2>({ 0, 1, 2 }, 25, model21_path + "Element.csv");
	model21.ImportField<LinearField>({ 0, 1, 2 });
	model21.ImportEquation<OptimizedAlternative3D, GaussCubic2>(0, {}, { 0, 1, 2, 3, 4 }, model21_path + "Equation.csv");
	model21.ImportDirichlet(model21_path + "Dirichlet.csv");
	model21.ImportNeumann(model21_path + "Neumann.csv");

	model21.ImportOptimizedParameter(model21_path + "Equation.csv");
	model21.ImportObjective<ComplianceAlternative3D>({ 0, 1, 2 }, { 0, 1, 2, 3, 4 });
	model21.ImportElementToObjective<GaussCubic2>(0, model21_path + "Equation.csv");
	model21.ImportConstraint<WeightAlternative3D>({}, { 0, 1, 2, 3, 5 });
	model21.ImportElementToConstraint<GaussCubic2>(0, model21_path + "Equation.csv");

	model21.Schedule();
	model21.Export("Data/Output/model21");
	*/


	//----------トポロジー最適化モデル（3D梁）----------
	//	パラメータ
	//		0	:s0		設計変数1
	//		1	:s1		設計変数2
	//		2	:E0		空気Young率
	//		3	:V0		空気Poisson比
	//		4	:G0		空気横弾性係数
	//		5	:ρ0	空気密度
	//		6	:E1		LatticeYoung率
	//		7	:V1		LatticePoisson比
	//		8	:G1		Lattice横弾性係数
	//		9	:ρ1	Lattice密度
	//		10	:E2		固体Young率
	//		11	:V2		固体Poisson比
	//		12	:G2		固体横弾性係数
	//		13	:ρ2	固体密度
	//	従属変数
	//		0	:ux		x軸方向変位
	//		1	:uy		y軸方向変位
	//		2	:uz		z軸方向変位
	//	設計パラメータ
	//		0	:s0		設計変数1
	//		1	:s1		設計変数2
	//
	//------------------------------------------------------
	//std::string model22_path = "Data/Input/Optimize/AirLatticeSolid/";
	std::string model22_path = "Data/Input/Optimize/AirLatticeSolid2/";
	/*OCSystem model22 = OCSystem(3, 3, { 0, 1 });

	model22.ImportNode(model22_path + "Node.csv");
	model22.ImportParameter({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 }, model22_path + "Parameter.csv");
	model22.ImportElement<Cubic2, Cubic2, Cubic2, Cubic2>({ 0, 1, 2 }, 25, model22_path + "Element.csv");
	model22.ImportField<LinearField>({ 0, 1, 2 });
	model22.ImportEquation<Optimized3PhaseMaterials3D, GaussCubic2>(0, {}, { 0, 1, 2, 3, 4, 6, 7, 8, 10, 11, 12 }, model22_path + "Equation.csv");
	model22.ImportDirichlet(model22_path + "Dirichlet.csv");
	model22.ImportNeumann(model22_path + "Neumann.csv");

	model22.ImportOptimizedParameter(model22_path + "Equation.csv");
	model22.ImportObjective<Compliance3PhaseMaterials3D>({ 0, 1, 2 }, { 0, 1, 2, 3, 4, 6, 7, 8, 10, 11, 12 });
	model22.ImportElementToObjective<GaussCubic2>(0, model22_path + "Equation.csv");
	model22.ImportConstraint<Weight3PhaseMaterials3D>({}, { 0, 1, 5, 9, 13 });
	model22.ImportElementToConstraint<GaussCubic2>(0, model22_path + "Equation.csv");

	model22.Schedule();
	model22.Export("Data/Output/model23");
	*/

	//----------トポロジー最適化モデル（3D梁）----------
	//	パラメータ
	//		0	:s		設計変数
	//		1	:E0		空気Young率
	//		2	:V0		空気Poisson比
	//		3	:G0		空気横弾性係数
	//		4	:ρ0	空気密度
	//		5	:E1		LatticeYoung率
	//		6	:V1		LatticePoisson比
	//		7	:G1		Lattice横弾性係数
	//		8	:ρ1	Lattice密度
	//	従属変数
	//		0	:ux		x軸方向変位
	//		1	:uy		y軸方向変位
	//		2	:uz		z軸方向変位
	//	設計パラメータ
	//		0	:s		設計変数
	//
	//------------------------------------------------------
	std::string model24_path = "Data/Input/Optimize/AirSolid/LatticeSolid/";
	OCSystem model24 = OCSystem(3, 3, { 0 });

	model24.ImportNode(model24_path + "Node.csv");
	model24.ImportParameter({ 0, 1, 2, 3, 4, 5, 6, 7, 8 }, model24_path + "Parameter.csv");
	model24.ImportElement<Cubic2, Cubic2, Cubic2, Cubic2>({ 0, 1, 2 }, 25, model24_path + "Element.csv");
	model24.ImportField<LinearField>({ 0, 1, 2 });
	model24.ImportEquation<Optimized2PhaseMaterials3D, GaussCubic2>(0, {}, { 0, 1, 2, 3, 5, 6, 7 }, model24_path + "Equation.csv");
	model24.ImportDirichlet(model24_path + "Dirichlet.csv");
	model24.ImportNeumann(model24_path + "Neumann.csv");

	model24.ImportOptimizedParameter(model24_path + "Equation.csv");
	model24.ImportObjective<Compliance2PhaseMaterials3D>({ 0, 1, 2 }, { 0, 1, 2, 3, 5, 6, 7 });
	model24.ImportElementToObjective<GaussCubic2>(0, model24_path + "Equation.csv");
	model24.ImportConstraint<Weight2PhaseMaterials3D>({}, { 0, 4, 8 });
	model24.ImportElementToConstraint<GaussCubic2>(0, model24_path + "Equation.csv");

	model24.Schedule();
	model24.Export("Data/Output/AirSolid/W50L10");

	std::cout << "--------------------Finish--------------------" << std::endl;

	return 0;
}