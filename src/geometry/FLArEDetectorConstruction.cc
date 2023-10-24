#include "geometry/FLArEDetectorConstruction.hh"
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

FLArEDetectorConstruction::FLArEDetectorConstruction()
{
  // load materials
  fMaterials = DetectorConstructionMaterial::GetInstance();

  // choose target material
  G4String fDetMaterialName = GeometricalParameters::Get()->GetTPCMaterialOption();
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
  BuildFLArEHadCal();
  BuildFLArEMuonCatcher();

  fFLArETPCAssembly = new G4AssemblyVolume();
  G4RotationMatrix* noRot = new G4RotationMatrix();

  // TPC
  G4ThreeVector tpcCenter(0.,0.,0.);
  if (fDetGeomOption == GeometricalParameters::tpcConfigOption::Single) {
    fFLArETPCAssembly->AddPlacedVolume(fFLArETPCLog, tpcCenter, noRot);
  } else if (fDetGeomOption == GeometricalParameters::tpcConfigOption::ThreeBySeven) {
    fFLArETPCAssembly->AddPlacedVolume(lArBoxLog, tpcCenter, noRot);
  }
  fFLArETPCAssembly->AddPlacedVolume(cryoInsulationLog, tpcCenter, noRot);

  // HadCal
  G4ThreeVector hadCalCenter(0.,0.,fLArSizeZ/2.+fThicknessInsulation+fHadCalLength/2.);
  fFLArETPCAssembly->AddPlacedAssembly(HadCalAssembly,hadCalCenter,noRot);

  // MuonCatcher
  G4ThreeVector MuonCatcherCenter(0.,0.,fLArSizeZ/2.+fThicknessInsulation+fHadCalLength+fMuonCatcherLength/2.);
  fFLArETPCAssembly->AddPlacedAssembly(MuonCatcherAssembly,MuonCatcherCenter,noRot);

  // visualization
  G4VisAttributes* absorVis = new G4VisAttributes(G4Colour(234./255, 173./255, 26./255, 0.8));
  absorVis->SetVisibility(true);
  HadAbsorLayersLogical->SetVisAttributes(absorVis);
  MuonFinderAbsorLayersLogical->SetVisAttributes(absorVis);

  G4VisAttributes* nullVis = new G4VisAttributes(G4Colour(167./255, 168./255, 189./255));
  nullVis->SetVisibility(false);
  hadCatcherLogical->SetVisAttributes(nullVis);
  HadCalXCellLogical->SetVisAttributes(nullVis);
  HadCalYCellLogical->SetVisAttributes(nullVis);
  muonFinderLogical->SetVisAttributes(nullVis);
  MuonFinderXCellLogical->SetVisAttributes(nullVis);
  MuonFinderYCellLogical->SetVisAttributes(nullVis);
}

FLArEDetectorConstruction::~FLArEDetectorConstruction()
{
  delete fFLArETPCAssembly;
  delete lArBoxLog;
  delete fFLArETPCLog;
  delete cryoInsulationLog;
  delete hadCatcherLogical;
  delete HadAbsorLayersLogical;
  delete HadCalXCellLogical;
  delete HadCalYCellLogical;
  delete HadCalAssembly;
  delete muonFinderLogical;
  delete MuonFinderAbsorLayersLogical;
  delete MuonFinderXCellLogical;
  delete MuonFinderYCellLogical;
  delete MuonCatcherAssembly;
}

void FLArEDetectorConstruction::BuildFLArETPC() 
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

  //-----------------------------------
  // insulation
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

void FLArEDetectorConstruction::BuildFLArEHadCal()
{
  G4double thicknessAbsorber = 5 * cm;
  G4double thicknessCaloX    = 1 * cm;
  G4double thicknessCaloY    = 1 * cm;
  G4int fNbOfAbsor = 15;

  G4double thicknessOneLayer = thicknessAbsorber + thicknessCaloX + thicknessCaloY;
  fHadCalLength  = fNbOfAbsor*thicknessOneLayer;
  GeometricalParameters::Get()->SetHadCalLength(fHadCalLength);

  auto hadCatcherSolid
    = new G4Box("HadCatcherBox", fLArSizeX/2, fLArSizeY/2, fHadCalLength/2);
  hadCatcherLogical
    = new G4LogicalVolume(hadCatcherSolid, fMaterials->Material("Air"), "HadCatcherLogical");

  // Absorber
  auto HadAbsorLayersSolid
    = new G4Box("HadAbsorLayersBox", fLArSizeX/2, fLArSizeY/2, thicknessAbsorber/2);
  HadAbsorLayersLogical
    = new G4LogicalVolume(HadAbsorLayersSolid, fMaterials->Material("Iron"), "HadAbsorLayersLogical");

  // X-Plane
  auto HadCalXLayersSolid
    = new G4Box("HadCalXLayersBox", fLArSizeX/2, fLArSizeY/2, thicknessCaloX/2);
  auto HadCalXLayersLogical
    = new G4LogicalVolume(HadCalXLayersSolid, fMaterials->Material("Polystyrene"), "HadCalXLayersLogical");

  auto HadCalXCellSolid
    = new G4Box("HadCalXCellBox", thicknessCaloX/2, fLArSizeY/2, thicknessCaloX/2);
  HadCalXCellLogical
    = new G4LogicalVolume(HadCalXCellSolid, fMaterials->Material("Polystyrene"), "HadCalXCellLogical");
  new G4PVReplica("HadCalXCellPhysical", HadCalXCellLogical,
                  HadCalXLayersLogical, kXAxis, fLArSizeX/thicknessCaloX, thicknessCaloX);

  // Y-Plane
  auto HadCalYLayersSolid
    = new G4Box("HadCalYLayersBox", fLArSizeX/2, fLArSizeY/2, thicknessCaloY/2);
  auto HadCalYLayersLogical
    = new G4LogicalVolume(HadCalYLayersSolid, fMaterials->Material("Polystyrene"), "HadCalYLayersLogical");

  auto HadCalYCellSolid
    = new G4Box("HadCalYCellBox", fLArSizeX/2, thicknessCaloY/2, thicknessCaloY/2);
  HadCalYCellLogical
    = new G4LogicalVolume(HadCalYCellSolid, fMaterials->Material("Polystyrene"), "HadCalYCellLogical");
  new G4PVReplica("HadCalYCellPhysical", HadCalYCellLogical,
                  HadCalYLayersLogical, kYAxis, fLArSizeY/thicknessCaloY, thicknessCaloY);

  HadCalAssembly = new G4AssemblyVolume();
  G4RotationMatrix Ra(0, 0, 0);
  for (int i= 0; i< fNbOfAbsor; ++i) {
    G4RotationMatrix Rm(0, 0, 0);
    G4ThreeVector THadAbsorb(0, 0, (i+0.5)*thicknessAbsorber+i*thicknessCaloX+i*thicknessCaloY-fHadCalLength/2.);
    G4ThreeVector THadCalX(0, 0, (i+1)*thicknessAbsorber+(i+0.5)*thicknessCaloX+i*thicknessCaloY-fHadCalLength/2.);
    G4ThreeVector THadCalY(0, 0, (i+1)*thicknessAbsorber+(i+1)*thicknessCaloX+(i+0.5)*thicknessCaloY-fHadCalLength/2.);
    HadCalAssembly->AddPlacedVolume(HadAbsorLayersLogical, THadAbsorb, &Ra);
    HadCalAssembly->AddPlacedVolume(HadCalXLayersLogical, THadCalX, &Ra);
    HadCalAssembly->AddPlacedVolume(HadCalYLayersLogical, THadCalY, &Ra);
  }

  G4VisAttributes* hadCalVis = new G4VisAttributes(G4Colour(34./255, 148./255, 83./255, 0.8));
  hadCalVis->SetVisibility(true);
  HadCalXLayersLogical    ->SetVisAttributes(hadCalVis);
  HadCalYLayersLogical    ->SetVisAttributes(hadCalVis);
}

void FLArEDetectorConstruction::BuildFLArEMuonCatcher() {
  G4double thicknessAbsorber = 16 * cm;
  G4double thicknessCaloX    = 1 * cm;
  G4double thicknessCaloY    = 1 * cm;
  G4int    fNbOfAbsor = 2;
  G4double thicknessOneLayer = thicknessAbsorber + thicknessCaloX + thicknessCaloY;
  fMuonCatcherLength  = fNbOfAbsor*thicknessOneLayer;

  auto muonFinderSolid
    = new G4Box("MuonFinderBox", fLArSizeX/2, fLArSizeY/2, fMuonCatcherLength/2);
  muonFinderLogical
    = new G4LogicalVolume(muonFinderSolid, fMaterials->Material("Air"), "MuonFinderLogical");

  // Absorber
  auto MuonFinderAbsorLayersSolid
    = new G4Box("MuonFinderAbsorLayersBox", fLArSizeX/2, fLArSizeY/2, thicknessAbsorber/2);
  MuonFinderAbsorLayersLogical
    = new G4LogicalVolume(MuonFinderAbsorLayersSolid, fMaterials->Material("Iron"), "MuonFinderAbsorLayersLogical");

  // X-Plane
  auto MuonFinderXLayersSolid
    = new G4Box("MuonFinderXLayersBox", fLArSizeX/2, fLArSizeY/2, thicknessCaloX/2);
  auto MuonFinderXLayersLogical
    = new G4LogicalVolume(MuonFinderXLayersSolid, fMaterials->Material("Polystyrene"), "MuonFinderXLayersLogical");

  auto MuonFinderXCellSolid
    = new G4Box("MuonFinderXCellBox", thicknessCaloX/2, fLArSizeY/2, thicknessCaloX/2);
  MuonFinderXCellLogical
    = new G4LogicalVolume(MuonFinderXCellSolid, fMaterials->Material("Polystyrene"), "MuonFinderXCellLogical");
  new G4PVReplica("MuonFinderXCellPhysical", MuonFinderXCellLogical,
                  MuonFinderXLayersLogical, kXAxis, fLArSizeX/thicknessCaloX, thicknessCaloX);

  // Y-Plane
  auto MuonFinderYLayersSolid
    = new G4Box("MuonFinderYLayersBox", fLArSizeX/2, fLArSizeY/2, thicknessCaloY/2);
  auto MuonFinderYLayersLogical
    = new G4LogicalVolume(MuonFinderYLayersSolid, fMaterials->Material("Polystyrene"), "MuonFinderYLayersLogical");

  auto MuonFinderYCellSolid
    = new G4Box("MuonFinderYCellBox", fLArSizeX/2, thicknessCaloY/2, thicknessCaloY/2);
  MuonFinderYCellLogical
    = new G4LogicalVolume(MuonFinderYCellSolid, fMaterials->Material("Polystyrene"), "MuonFinderYCellLogical");
  new G4PVReplica("MuonFinderYCellPhysical", MuonFinderYCellLogical,
                  MuonFinderYLayersLogical, kYAxis, fLArSizeY/thicknessCaloY, thicknessCaloY);

  MuonCatcherAssembly = new G4AssemblyVolume();
  G4RotationMatrix Ra(0, 0, 0);
  for (int i= 0; i< fNbOfAbsor; ++i) {
    G4RotationMatrix Rm(0, 0, 0);
    G4ThreeVector TMuonAbsorb(0, 0, (i+0.5)*thicknessAbsorber+i*thicknessCaloX+i*thicknessCaloY-fMuonCatcherLength/2.);
    G4ThreeVector TMuonCatcherX(0, 0, (i+1)*thicknessAbsorber+(i+0.5)*thicknessCaloX+i*thicknessCaloY-fMuonCatcherLength/2.);
    G4ThreeVector TMuonCatcherY(0, 0, (i+1)*thicknessAbsorber+(i+1)*thicknessCaloX+(i+0.5)*thicknessCaloY-fMuonCatcherLength/2.);
    MuonCatcherAssembly->AddPlacedVolume(MuonFinderAbsorLayersLogical, TMuonAbsorb, &Ra);
    MuonCatcherAssembly->AddPlacedVolume(MuonFinderXLayersLogical, TMuonCatcherX, &Ra);
    MuonCatcherAssembly->AddPlacedVolume(MuonFinderYLayersLogical, TMuonCatcherY, &Ra);
  }

  G4VisAttributes* hadCalVis = new G4VisAttributes(G4Colour(34./255, 148./255, 83./255, 0.8));
  hadCalVis->SetVisibility(true);
  MuonFinderXLayersLogical->SetVisAttributes(hadCalVis);
  MuonFinderYLayersLogical->SetVisAttributes(hadCalVis);
}
