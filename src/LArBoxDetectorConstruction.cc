#include "LArBoxDetectorConstruction.hh"
#include "DetectorConstructionMaterial.hh"
#include "LArBoxSD.hh"

#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4NistManager.hh>
#include <G4VisAttributes.hh>
#include <G4Box.hh>
#include <G4PhysicalConstants.hh>
#include <G4SystemOfUnits.hh>
#include <G4OpticalSurface.hh>
#include <G4LogicalBorderSurface.hh>
#include <G4SDManager.hh>

using namespace std;

G4VPhysicalVolume* LArBoxDetectorConstruction::Construct() {
  //G4NistManager* nist = G4NistManager::Instance();
  //G4Material* air   = nist->FindOrBuildMaterial("G4_AIR");
  //G4Material* arGas = nist->FindOrBuildMaterial("G4_Ar");

  //-----------------------------
  // construction of materials
  //-----------------------------
  
  DetectorConstructionMaterial* LArBoxMaterials = 
    DetectorConstructionMaterial::GetInstance();

  G4bool fCheckOverlap = true;

  // create an experimental hall with size 100*100*100 m
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
  worldLog = new G4LogicalVolume(worldBox, LArBoxMaterials->Material("Vacuum"), "world");
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

  // LAr box
  G4VSolid* lArBox = new G4Box("lArBox", lArSizeX / 2, lArSizeY / 2, lArSizeZ / 2);
  //lArBoxLog = new G4LogicalVolume(lArBox, LArBoxMaterials->Material("LiquidScintillator"), "lArBox");
  //lArBoxLog = new G4LogicalVolume(lArBox, LArBoxMaterials->Material("LiquidArgon"), "lArBox");
  lArBoxLog = new G4LogicalVolume(lArBox, LArBoxMaterials->Material("LiquidKrypton"), "lArBox");
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

  G4cout<<*(G4Material::GetMaterialTable())<<G4endl;

  return worldPhys;
}

void LArBoxDetectorConstruction::ConstructSDandField() {
  G4SDManager* sdManager = G4SDManager::GetSDMpointer();
  sdManager->SetVerboseLevel(2);

  LArBoxSD* lArBoxSD = new LArBoxSD("lArBoxSD");
  lArBoxLog->SetSensitiveDetector(lArBoxSD);
  sdManager->AddNewDetector(lArBoxSD);
}
