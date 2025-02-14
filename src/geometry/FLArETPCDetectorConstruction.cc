#include "geometry/FLArETPCDetectorConstruction.hh"
#include "geometry/GeometricalParameters.hh"
#include "DetectorConstructionMaterial.hh"

#include "G4AssemblyVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh" 
#include "G4PVReplica.hh"
#include "G4UserLimits.hh"
#include "G4PVPlacement.hh"

FLArETPCDetectorConstruction::FLArETPCDetectorConstruction()
{
  // load materials
  fMaterials = DetectorConstructionMaterial::GetInstance();

  // choose target material
  GeometricalParameters::tpcMaterialOption fDetMaterialName = GeometricalParameters::Get()->GetTPCMaterialOption();
  detectorMaterial = 0;
  if (fDetMaterialName == GeometricalParameters::tpcMaterialOption::LiquidArgon) {
    detectorMaterial = fMaterials->Material("LiquidArgon");
    G4cout<<"**** FLArE TPC Material : Liquid Argon ****"<<G4endl;
  } else if (fDetMaterialName == GeometricalParameters::tpcMaterialOption::LiquidKrypton) {
    detectorMaterial = fMaterials->Material("LiquidKrypton");
    G4cout<<"**** FLArE TPC Material : Liquid Krypton ****"<<G4endl;
  } 
  if (!detectorMaterial) {
    G4cout << "ERROR: undefined target material!" << G4endl;  
  }
  G4cout << "Building FLArE TPC" << G4endl;

  // detector geometry configuration
  fDetGeomOption       = GeometricalParameters::Get()->GetTPCConfigOption();
  fLArSizeX            = GeometricalParameters::Get()->GetTPCSizeX();
  fLArSizeY            = GeometricalParameters::Get()->GetTPCSizeY();
  fLArSizeZ            = GeometricalParameters::Get()->GetTPCSizeZ();
  fThicknessInsulation = GeometricalParameters::Get()->GetTPCInsulationThickness();

  BuildFLArETPC();
  BuildCryostatInsulation();

  G4double halfContainerX = fLArSizeX/2. + fThicknessInsulation;
  G4double halfContainerY = fLArSizeY/2. + fThicknessInsulation;
  G4double halfContainerZ = fLArSizeZ/2. + fThicknessInsulation;
  auto containerSolid = new G4Box("FLArETPCContainerSolid", halfContainerX, halfContainerY, halfContainerZ);
  fFLArETPCLogical = new G4LogicalVolume(containerSolid, fMaterials->Material("Air"), "FLArETPCLogical");

  // TPC
  G4ThreeVector tpcCenter(0.,0.,0.);
  G4RotationMatrix* noRot = new G4RotationMatrix();
  if (fDetGeomOption == GeometricalParameters::tpcConfigOption::Single) {
    new G4PVPlacement(noRot, tpcCenter, fFLArETPCLog, "LArPhysical", fFLArETPCAssembly, false, 0, false);
  } else if (fDetGeomOption == GeometricalParameters::tpcConfigOption::ThreeBySeven) {
    new G4PVPlacement(noRot, tpcCenter, lArBoxLog, "LArPhysical", fFLArETPCAssembly, false, 0, false);
  } else {
    G4cout << "ERROR: undefined TPC configuration!" << G4endl;  
  }
  new G4PVPlacement(noRot, tpcCenter, cryoInsulationLog, "CryostatPhysical", fFLArETPCAssembly, false, 0, false);

}

FLArETPCDetectorConstruction::~FLArETPCDetectorConstruction()
{
  delete fFLArETPCAssembly;
  delete lArBoxLog;
  delete fFLArETPCLog;
  delete cryoInsulationLog;
}

void FLArETPCDetectorConstruction::BuildFLArETPC() 
{
  auto lArBox = new G4Box("lArBox", fLArSizeX/2., fLArSizeY/2., fLArSizeZ/2.);

  if (fDetGeomOption == GeometricalParameters::tpcConfigOption::Single) {
    G4cout << "TPC module configuration: single" << G4endl;
    fFLArETPCLog = new G4LogicalVolume(lArBox, detectorMaterial, "TPCModuleLogical");
    auto lArBoxVis = new G4VisAttributes(G4Colour(86./255, 152./255, 195./255));
    lArBoxVis->SetVisibility(true);
    lArBoxVis->SetForceWireframe(true);
    lArBoxVis->SetForceAuxEdgeVisible(true);
    fFLArETPCLog->SetVisAttributes(lArBoxVis);
    fFLArETPCLog->SetUserLimits(new G4UserLimits(0.5*mm));
  } else if (fDetGeomOption == GeometricalParameters::tpcConfigOption::ThreeBySeven) {
    G4cout << "TPC module configuration: 3x7" << G4endl;
    lArBoxLog = new G4LogicalVolume(lArBox, detectorMaterial, "TPCModuleLogical");
    auto lArBoxVis = new G4VisAttributes(G4Colour(86./255, 152./255, 195./255));
    lArBoxVis->SetVisibility(false);
    lArBoxLog->SetVisAttributes(lArBoxVis);

    G4double TPCLayerWidth   = fLArSizeX;
    G4double TPCLayerHeight  = fLArSizeY;
    G4double TPCLayerLength  = fLArSizeZ / 7.0;
    G4double TPCModuleWidth  = TPCLayerWidth / 3.0;
    G4double TPCModuleHeight = TPCLayerHeight;
    G4double TPCModuleLength = TPCLayerLength;
    auto TPCLayerSolid
      = new G4Box("TPCLayerBox", TPCLayerWidth/2, TPCLayerHeight/2, TPCLayerLength/2);
    auto TPCLayerLogical
      = new G4LogicalVolume(TPCLayerSolid, detectorMaterial, "TPCLayerLogical");
    auto TPCModuleSolid
      = new G4Box("TPCModuleBox", TPCModuleWidth/2, TPCModuleHeight/2, TPCModuleLength/2);
    fFLArETPCLog = new G4LogicalVolume(TPCModuleSolid, detectorMaterial, "TPCModuleLog");
    new G4PVReplica("TPCModulePhysical", fFLArETPCLog, TPCLayerLogical, kXAxis, 3, TPCModuleWidth);
    new G4PVReplica("TPC", TPCLayerLogical, lArBoxLog, kZAxis, 7, TPCLayerLength);
    G4VisAttributes* TPCModuleVis = new G4VisAttributes(G4Colour(86./255, 152./255, 195./255));
    TPCModuleVis->SetVisibility(true);
    TPCModuleVis->SetForceWireframe(true);
    TPCModuleVis->SetForceAuxEdgeVisible(true);
    TPCLayerLogical->SetVisAttributes(lArBoxVis);
    fFLArETPCLog->SetVisAttributes(TPCModuleVis);
    fFLArETPCLog->SetUserLimits(new G4UserLimits(0.5*mm));
  }
}

void FLArETPCDetectorConstruction::BuildCryostatInsulation()
{
  //-----------------------------------
  // insulation
  auto lArBox = new G4Box("lArBox", fLArSizeX/2., fLArSizeY/2., fLArSizeZ/2.);
  auto CryoInsulationBlockSolid = new G4Box("CryoInsulationBlock", 
                                            fLArSizeX/2.+fThicknessInsulation, 
                                            fLArSizeY/2.+fThicknessInsulation,
                                            fLArSizeZ/2.+fThicknessInsulation);
  auto CryoInsulationSolid = new G4SubtractionSolid("CryoInsulation",
                                                    CryoInsulationBlockSolid,
                                                    lArBox,
                                                    0, //no rotation
                                                    G4ThreeVector(0,0,0)); // no translation
  cryoInsulationLog = new G4LogicalVolume(CryoInsulationSolid, 
                                          fMaterials->Material("R_PUF"), 
                                          "CryoInsulationLogical"); 
  auto CryoInsulationVis = new G4VisAttributes(G4Colour(86./255, 152./255, 195./255));
  CryoInsulationVis->SetVisibility(true);
  CryoInsulationVis->SetForceWireframe(true);
  CryoInsulationVis->SetForceAuxEdgeVisible(true);
  cryoInsulationLog->SetVisAttributes(CryoInsulationVis);
}
