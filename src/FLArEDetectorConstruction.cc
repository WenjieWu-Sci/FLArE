#include "FLArEDetectorConstruction.hh"
#include "DetectorConstructionMaterial.hh"
#include "FLArEDetectorConstructionMessenger.hh"
#include "LArBoxSD.hh"

#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4NistManager.hh>
#include <G4VisAttributes.hh>
#include <G4Box.hh>
#include <G4ReplicatedSlice.hh>
#include <G4PVReplica.hh>
#include <G4AssemblyVolume.hh>
#include <G4RotationMatrix.hh>
#include <G4ThreeVector.hh>
#include <G4PhysicalConstants.hh>
#include <G4SystemOfUnits.hh>
#include <G4OpticalSurface.hh>
#include <G4LogicalBorderSurface.hh>
#include <G4LogicalSkinSurface.hh>
#include <G4SDManager.hh>
#include <G4RunManager.hh>
#include <G4GeometryManager.hh>
#include <G4PhysicalVolumeStore.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4SolidStore.hh>
#include "G4GDMLParser.hh"

using namespace std;

FLArEDetectorConstruction::FLArEDetectorConstruction()
  : G4VUserDetectorConstruction(), fDetMaterialName("LAr")
{
  DefineMaterial();
  messenger = new FLArEDetectorConstructionMessenger(this);
  m_saveGdml = false;
}

FLArEDetectorConstruction::~FLArEDetectorConstruction() 
{
  delete messenger;
}

void FLArEDetectorConstruction::DefineMaterial() {
  //-----------------------------
  // construction of materials
  //-----------------------------
  
  LArBoxMaterials = DetectorConstructionMaterial::GetInstance();
}

G4VPhysicalVolume* FLArEDetectorConstruction::Construct()
{
//  DetectorConstructionMaterial* LArBoxMaterials = 
//    DetectorConstructionMaterial::GetInstance();

  G4bool fCheckOverlap = true;

  // create an experimental hall with size 20*20*20 m
  G4double worldSizeX = 20 * m;
  G4double worldSizeY = 20 * m;
  G4double worldSizeZ = 20 * m;
  // LAr volume: +z being beam direction
  G4double lArSizeX = 1.0 * m;
  G4double lArSizeY = 1.0 * m;
  G4double lArSizeZ = 7.0 * m;
  //G4double lArSizeX = 1.5 * m;
  //G4double lArSizeY = 1.5 * m;
  //G4double lArSizeZ = 7.0 * m;

  // Experimental hall
  G4VSolid* worldBox = new G4Box("world", worldSizeX / 2, worldSizeY / 2, worldSizeZ / 2);
  worldLog = new G4LogicalVolume(worldBox, LArBoxMaterials->Material("Air"), "world");
  G4VisAttributes* visAttr = new G4VisAttributes();
  visAttr->SetVisibility(true);
  worldLog->SetVisAttributes(visAttr);
  G4VPhysicalVolume* worldPhys = new G4PVPlacement(nullptr,
                                                   {},
                                                   worldLog,
                                                   "world",
                                                   nullptr,
                                                   false, 
                                                   0);

  G4Material* detectorMaterial = 0;
  if (fDetMaterialName == "LAr") {
    detectorMaterial = LArBoxMaterials->Material("LiquidArgon");
    G4cout<<"**** Detector Material : Liquid Argon ****"<<G4endl;
  }
  if (fDetMaterialName == "LKr") { 
    detectorMaterial = LArBoxMaterials->Material("LiquidKrypton");
    G4cout<<"**** Detector Material : Liquid Krypton ****"<<G4endl;
  }

  // LAr box
  G4VSolid* lArBox = new G4Box("lArBox", lArSizeX / 2, lArSizeY / 2, lArSizeZ / 2);
  lArBoxLog = new G4LogicalVolume(lArBox, detectorMaterial, "lArBox");
  G4VisAttributes* lArBoxVis = new G4VisAttributes(G4Colour(86./255, 152./255, 195./255));
  lArBoxVis->SetVisibility(true);
  //lArBoxVis->SetForceSolid(true);
  lArBoxVis->SetForceWireframe(true);
  lArBoxVis->SetForceAuxEdgeVisible(true);
  lArBoxLog->SetVisAttributes(lArBoxVis);
  G4VPhysicalVolume* lArBoxPhys = new G4PVPlacement(nullptr, // no Rotation
                                                    G4ThreeVector(0, 0, 0), // no transporation
                                                    lArBoxLog,              // current logical volume
                                                    "lArBox",               // name
                                                    worldLog,               // mother logical volume
                                                    false,                  // pMany
                                                    0,                      // Copy No
                                                    fCheckOverlap);

  //G4cout<<*(G4Material::GetMaterialTable())<<G4endl;

  //
  // Hadron Calorimeter
  // 
  G4double GapToHadCatcher   = 15 * cm;
  G4double thicknessAbsorber = 5 * cm;
  G4double thicknessCaloX    = 1 * cm;
  G4double thicknessCaloY    = 1 * cm;
  G4int fNbOfAbsor = 16;

  G4double thicknessOneLayer = thicknessAbsorber + thicknessCaloX + thicknessCaloY;
  G4double HadCatcherLength  = fNbOfAbsor*thicknessOneLayer;

  auto hadCatcherSolid
    = new G4Box("HadCatcherBox", lArSizeX/2, lArSizeY/2, HadCatcherLength/2);
  auto hadCatcherLogical
    = new G4LogicalVolume(hadCatcherSolid, LArBoxMaterials->Material("Air"), "HadCatcherLogical");
  new G4PVPlacement(nullptr,
                    G4ThreeVector(0, 0, lArSizeZ/2+HadCatcherLength/2+GapToHadCatcher),
                    hadCatcherLogical,
                    "HadCatcherPhysical",
                    worldLog,
                    false,
                    0,
                    fCheckOverlap);

  // Absorber
  auto HadAbsorLayersSolid
    = new G4Box("HadAbsorLayersBox", lArSizeX/2, lArSizeY/2, thicknessAbsorber/2);
  auto HadAbsorLayersLogical
    = new G4LogicalVolume(HadAbsorLayersSolid, LArBoxMaterials->Material("Iron"), "HadAbsorLayersLogical");

  // X-Plane
  auto HadCalXLayersSolid
    = new G4Box("HadCalXLayersBox", lArSizeX/2, lArSizeY/2, thicknessCaloX/2);
  auto HadCalXLayersLogical
    = new G4LogicalVolume(HadCalXLayersSolid, LArBoxMaterials->Material("Polystyrene"), "HadCalXLayersLogical");

  auto HadCalXCellSolid
    = new G4Box("HadCalXCellBox", thicknessCaloX/2, lArSizeY/2, thicknessCaloX/2);
  HadCalXCellLogical
    = new G4LogicalVolume(HadCalXCellSolid, LArBoxMaterials->Material("Polystyrene"), "HadCalXCellLogical");
  new G4PVReplica("HadCalXCellPhysical", HadCalXCellLogical,
                  HadCalXLayersLogical, kXAxis, lArSizeX/thicknessCaloX, thicknessCaloX);

  // Y-Plane
  auto HadCalYLayersSolid
    = new G4Box("HadCalYLayersBox", lArSizeX/2, lArSizeY/2, thicknessCaloY/2);
  auto HadCalYLayersLogical
    = new G4LogicalVolume(HadCalYLayersSolid, LArBoxMaterials->Material("Polystyrene"), "HadCalYLayersLogical");

  auto HadCalYCellSolid
    = new G4Box("HadCalYCellBox", lArSizeX/2, thicknessCaloY/2, thicknessCaloY/2);
  HadCalYCellLogical
    = new G4LogicalVolume(HadCalYCellSolid, LArBoxMaterials->Material("Polystyrene"), "HadCalYCellLogical");
  new G4PVReplica("HadCalYCellPhysical", HadCalYCellLogical,
                  HadCalYLayersLogical, kYAxis, lArSizeY/thicknessCaloY, thicknessCaloY);

  auto HadAbsorAssembly = new G4AssemblyVolume();
  G4RotationMatrix Ra(0, 0, 0);
  G4ThreeVector Ta(0, 0, thicknessAbsorber/2);
  HadAbsorAssembly->AddPlacedVolume(HadAbsorLayersLogical, Ta, &Ra);
  Ta.setZ(thicknessAbsorber+thicknessCaloX/2);
  HadAbsorAssembly->AddPlacedVolume(HadCalXLayersLogical, Ta, &Ra);
  Ta.setZ(thicknessAbsorber+thicknessCaloX+thicknessCaloY/2);
  HadAbsorAssembly->AddPlacedVolume(HadCalYLayersLogical, Ta, &Ra);

  for (int i= 0; i< fNbOfAbsor; ++i) {
    G4RotationMatrix Rm(0, 0, 0);
    G4ThreeVector Tm(0, 0, thicknessOneLayer*i - HadCatcherLength/2);
    HadAbsorAssembly->MakeImprint(hadCatcherLogical, Tm, &Rm);
  }

  G4VisAttributes* absorVis = new G4VisAttributes(G4Colour(234./255, 173./255, 26./255, 0.8));
  absorVis->SetVisibility(true);
  absorVis->SetForceWireframe(true);
  absorVis->SetForceAuxEdgeVisible(true);
  HadAbsorLayersLogical->SetVisAttributes(absorVis);

  G4VisAttributes* hadCalVis = new G4VisAttributes(G4Colour(34./255, 148./255, 83./255));
  hadCalVis->SetVisibility(true);
  hadCalVis->SetForceWireframe(true);
  hadCalVis->SetForceAuxEdgeVisible(true);
  //HadCalXLayersLogical->SetVisAttributes(hadCalVis);
  //HadCalYLayersLogical->SetVisAttributes(hadCalVis);
  HadCalXCellLogical->SetVisAttributes(hadCalVis);
  HadCalYCellLogical->SetVisAttributes(hadCalVis);

//  hadCatcherLogical->SetVisAttributes(lArBoxVis);

  /*
  G4VisAttributes* nullVis = new G4VisAttributes(G4Colour(167./255, 168./255, 189./255));
  nullVis->SetVisibility(false);
  hadAbsorPlaneLogical->SetVisAttributes(nullVis);
  hadCalXPlaneLogical->SetVisAttributes(nullVis);
  hadCalYPlaneLogical->SetVisAttributes(nullVis);
  */

  if (m_saveGdml) {
    G4GDMLParser fParser;
    fParser.Write("LArBoxDetGeo.gdml", worldPhys);
  }

  return worldPhys;
}

void FLArEDetectorConstruction::ConstructSDandField() {
  G4SDManager* sdManager = G4SDManager::GetSDMpointer();
  sdManager->SetVerboseLevel(2);

  LArBoxSD* lArBoxSD = new LArBoxSD("lArBoxSD");
  lArBoxLog->SetSensitiveDetector(lArBoxSD);
  sdManager->AddNewDetector(lArBoxSD);

  LArBoxSD* HadCalXSD = new LArBoxSD("HadCalXSD");
  HadCalXCellLogical->SetSensitiveDetector(HadCalXSD);
  sdManager->AddNewDetector(HadCalXSD);

  LArBoxSD* HadCalYSD = new LArBoxSD("HadCalYSD");
  HadCalYCellLogical->SetSensitiveDetector(HadCalYSD);
  sdManager->AddNewDetector(HadCalYSD);
}

void FLArEDetectorConstruction::SetDetMaterial(G4String detMaterial) {
//  if (!lArBoxLog) {
//    G4cerr<<"Detector has not yet been constructed"<<G4endl;
//    return;
//  }

  fDetMaterialName = detMaterial;
//  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

//void FLArEDetectorConstruction::UpdateGeometry() {
  //// clean up previous geometry
  //G4GeometryManager::GetInstance()->OpenGeometry();

  //G4PhysicalVolumeStore::GetInstance()->Clean();
  //G4LogicalVolumeStore::GetInstance()->Clean();
  //G4SolidStore::GetInstance()->Clean();
  //G4LogicalSkinSurface::CleanSurfaceTable();
  //G4LogicalBorderSurface::CleanSurfaceTable();
  //G4SurfaceProperty::CleanSurfacePropertyTable();

  //// define new one
  //G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
//  G4RunManager::GetRunManager()->GeometryHasBeenModified();
//}
