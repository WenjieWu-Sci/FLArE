#include "DetectorConstruction.hh"
#include "DetectorConstructionMaterial.hh"
#include "DetectorConstructionMessenger.hh"
#include "LArBoxSD.hh"

#include "geometry/SpectrometerMagnetConstruction.hh"
#include "geometry/GeometricalParameters.hh"
#include "geometry/FASERnu2DetectorConstruction.hh"
#include "geometry/FORMOSADetectorConstruction.hh"
#include "geometry/FLArEDetectorConstruction.hh"

#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4NistManager.hh>
#include <G4VisAttributes.hh>
#include <G4Box.hh>
#include <G4ReplicatedSlice.hh>
#include <G4PVReplica.hh>
#include <G4AssemblyVolume.hh>
#include <G4SubtractionSolid.hh>
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
#include <G4UserLimits.hh>
#include <G4GlobalMagFieldMessenger.hh>
#include <G4UniformMagField.hh>
#include <G4FieldManager.hh>
#include <G4GDMLParser.hh>

using namespace std;

G4ThreadLocal G4UniformMagField* DetectorConstruction::magField = 0;
G4ThreadLocal G4FieldManager* DetectorConstruction::fieldMgr = 0;
G4ThreadLocal G4UniformMagField* DetectorConstruction::magFieldFASER2 = 0;
G4ThreadLocal G4FieldManager* DetectorConstruction::fieldMgrFASER2 = 0;

DetectorConstruction::DetectorConstruction()
  : G4VUserDetectorConstruction()
{
  DefineMaterial();
  messenger = new DetectorConstructionMessenger(this);
  m_saveGdml = false;
}

DetectorConstruction::~DetectorConstruction() 
{
  delete messenger;
}

void DetectorConstruction::DefineMaterial() {
  //-----------------------------
  // construction of materials
  //-----------------------------
  
  LArBoxMaterials = DetectorConstructionMaterial::GetInstance();
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{

  G4bool fCheckOverlap = true;

  // create an experimental hall with size 20*20*90 m
  G4double worldSizeX = 20 * m;
  G4double worldSizeY = 20 * m;
  G4double worldSizeZ = 90 * m;

  // Experimental hall
  G4VSolid* worldBox = new G4Box("world", worldSizeX / 2, worldSizeY / 2, worldSizeZ / 2);
  worldLog = new G4LogicalVolume(worldBox, LArBoxMaterials->Material("Air"), "world");
  G4VPhysicalVolume* worldPhys = new G4PVPlacement(nullptr,
                                                   {},
                                                   worldLog,
                                                   "world",
                                                   nullptr,
                                                   false, 
                                                   0);

  //-----------------------------------
  // FLArE TPC volume, HadCal, and MuonCatcher

  FLArEDetectorConstruction *FLArEAssembler = new FLArEDetectorConstruction();
  G4AssemblyVolume* FLArEAssembly = FLArEAssembler->GetFLArEAssembly();
  TPCModuleLogical            = FLArEAssembler->GetFLArETPCVolume();
  hadCatcherLogical           = FLArEAssembler->GetHadCalVolume();
  HadCalXCellLogical          = FLArEAssembler->GetHadCalXVolume();
  HadCalYCellLogical          = FLArEAssembler->GetHadCalYVolume();
  HadAbsorLayersLogical       = FLArEAssembler->GetHadCalAbsorbVolume();
  muonFinderLogical           = FLArEAssembler->GetMuonCatcherVolume();
  MuonFinderXCellLogical      = FLArEAssembler->GetMuonCatcherXVolume();
  MuonFinderYCellLogical      = FLArEAssembler->GetMuonCatcherYVolume();
  MuonFinderAbsorLayersLogical= FLArEAssembler->GetMuonCatcherAbsorbVolume();

  // positioning
  G4ThreeVector FLArEPos(0.,0.,0.); 
  FLArEAssembly->MakeImprint(worldLog, FLArEPos, nullptr, 0, fCheckOverlap);
  
  double lArSizeZ         = FLArEAssembler->GetLengthTPC();
  double GapToHadCatcher  = FLArEAssembler->GetTPCInsulationThickness();
  double HadCatcherLength = FLArEAssembler->GetLengthHadCal();
  double MuonFinderLength = FLArEAssembler->GetLengthMuonCatcher();

  //-----------------------------------
  // FORMOSA
  G4double detectorGapLength = 1.2*m;

  FORMOSADetectorConstruction *FORMOSAAssembler = new FORMOSADetectorConstruction();
  G4AssemblyVolume* FORMOSAAssembly = FORMOSAAssembler->GetFORMOSAAssembly();
  FORMOSAScintillatorBarLogical = FORMOSAAssembler->GetScintillatorBar();

  // positioning
  G4double lengthFORMOSA = GeometricalParameters::Get()->GetFORMOSATotalSizeZ();
  G4double FORMOSAPosZ = (lArSizeZ/2. + GapToHadCatcher + HadCatcherLength + MuonFinderLength) +
                          detectorGapLength + lengthFORMOSA/2.;
  G4ThreeVector FORMOSAPos(0.,0.,FORMOSAPosZ); 
  FORMOSAAssembly->MakeImprint(worldLog, FORMOSAPos, nullptr, 0, fCheckOverlap);
                         
  //-----------------------------------
  // FASERnu2 Emulsion Detector
  FASERnu2DetectorConstruction *FASERnu2Assembler = new FASERnu2DetectorConstruction();
  FASERnu2EmulsionLogical = FASERnu2Assembler->GetEmulsionFilm();
  FASERnu2VetoInterfaceLogical = FASERnu2Assembler->GetVetoInterfaceDetector();
  G4AssemblyVolume* FASERnu2Assembly = FASERnu2Assembler->GetFASERnu2Assembly();
  
  // positioning
  G4double lengthFASERnu2 = GeometricalParameters::Get()->GetFASERnu2TotalSizeZ();
  G4double FASERnu2PosZ = FORMOSAPosZ + lengthFORMOSA/2. + detectorGapLength + lengthFASERnu2/2;
  G4ThreeVector FASERnu2Pos(0.,0.,FASERnu2PosZ); 
  FASERnu2Assembly->MakeImprint(worldLog, FASERnu2Pos, nullptr, 0, fCheckOverlap);
  
  //-----------------------------------
  // FASER2 Magnet + Tracking stations

  SpectrometerMagnetConstruction *magnetAssembler = new SpectrometerMagnetConstruction();
  FASER2MagnetLogical = magnetAssembler->GetMagneticVolume(); //need to assign B field
  TrackingVerScinBarLogical = magnetAssembler->GetVerTrackingScinBar(); //need to assign SD
  TrackingHorScinBarLogical = magnetAssembler->GetHorTrackingScinBar(); //need to assign SD
  G4AssemblyVolume* magnetAssembly = magnetAssembler->GetSpectrometerMagnetAssembly();

  // positioning
  G4double lengthDecayTunnelFASER2 = 10*m;
  G4double lengthVetoStationFASER2 = 20.*cm; //guesses (including gaps)
  G4double lengthSpectrometerMagnetAssembly = GeometricalParameters::Get()->GetMagnetTotalSizeZ() 
                                            + 2*GeometricalParameters::Get()->GetTrackingStationTotalSizeZ();
  G4double magnetPosZ = FASERnu2PosZ + lengthFASERnu2/2. + detectorGapLength + 
                        lengthVetoStationFASER2 + lengthDecayTunnelFASER2 +
			lengthSpectrometerMagnetAssembly/2.;  
  GeometricalParameters::Get()->SetMagnetZPosition(magnetPosZ); // save for momentum analysis

  G4ThreeVector magPos(0.,0.,magnetPosZ); 
  magnetAssembly->MakeImprint(worldLog, magPos, nullptr, 0, fCheckOverlap);
  
  //-------------------------------------------------------------------

  // visualization
  G4VisAttributes* nullVis = new G4VisAttributes(G4Colour(167./255, 168./255, 189./255));
  nullVis->SetVisibility(false);
  worldLog->SetVisAttributes(nullVis);

  if (m_saveGdml) {
    G4GDMLParser fParser;
    fParser.Write("LArBoxDetGeo.gdml", worldPhys);
  }

  return worldPhys;
}

void DetectorConstruction::ConstructSDandField() {
  G4SDManager* sdManager = G4SDManager::GetSDMpointer();
  sdManager->SetVerboseLevel(2);

  LArBoxSD* TPCModuleSD = new LArBoxSD("lArBoxSD");
  TPCModuleLogical->SetSensitiveDetector(TPCModuleSD);
  sdManager->AddNewDetector(TPCModuleSD);

  LArBoxSD* HadCalXSD = new LArBoxSD("HadCalXSD");
  HadCalXCellLogical->SetSensitiveDetector(HadCalXSD);
  sdManager->AddNewDetector(HadCalXSD);

  LArBoxSD* HadCalYSD = new LArBoxSD("HadCalYSD");
  HadCalYCellLogical->SetSensitiveDetector(HadCalYSD);
  sdManager->AddNewDetector(HadCalYSD);

  LArBoxSD* MuonFinderXSD = new LArBoxSD("MuonFinderXSD");
  MuonFinderXCellLogical->SetSensitiveDetector(MuonFinderXSD);
  sdManager->AddNewDetector(MuonFinderXSD);

  LArBoxSD* MuonFinderYSD = new LArBoxSD("MuonFinderYSD");
  MuonFinderYCellLogical->SetSensitiveDetector(MuonFinderYSD);
  sdManager->AddNewDetector(MuonFinderYSD);

  LArBoxSD* HadAbsorbSD = new LArBoxSD("HadAbsorbSD");
  HadAbsorLayersLogical->SetSensitiveDetector(HadAbsorbSD);
  sdManager->AddNewDetector(HadAbsorbSD);

  LArBoxSD* MuonFinderAbsorbSD = new LArBoxSD("MuonFinderAbsorbSD");
  MuonFinderAbsorLayersLogical->SetSensitiveDetector(MuonFinderAbsorbSD);
  sdManager->AddNewDetector(MuonFinderAbsorbSD);

  LArBoxSD* ScintillatorBarSD = new LArBoxSD("FORMOSAScinBarSD");
  FORMOSAScintillatorBarLogical->SetSensitiveDetector(ScintillatorBarSD);
  sdManager->AddNewDetector(ScintillatorBarSD);

  LArBoxSD* EmulsionFilmSD = new LArBoxSD("FASERnu2EmulsionSD");
  FASERnu2EmulsionLogical->SetSensitiveDetector(EmulsionFilmSD);
  sdManager->AddNewDetector(EmulsionFilmSD);
  
  LArBoxSD* VetoInterfaceSD = new LArBoxSD("FASERnu2VetoInterfaceSD");
  FASERnu2VetoInterfaceLogical->SetSensitiveDetector(VetoInterfaceSD);
  sdManager->AddNewDetector(VetoInterfaceSD);

  LArBoxSD* TrkHorScinSD = new LArBoxSD("TrkHorScinSD");
  TrackingHorScinBarLogical->SetSensitiveDetector(TrkHorScinSD);
  sdManager->AddNewDetector(TrkHorScinSD);

  LArBoxSD* TrkVerScinSD = new LArBoxSD("TrkVerScinSD");
  TrackingVerScinBarLogical->SetSensitiveDetector(TrkVerScinSD);
  sdManager->AddNewDetector(TrkVerScinSD);

  // HadCatcher + MuonFinder  magnetic field
  G4ThreeVector fieldValue = G4ThreeVector(0,fFieldValue, 0);
  magField = new G4UniformMagField(fieldValue);
  fieldMgr = new G4FieldManager();
  fieldMgr->SetDetectorField(magField);
  fieldMgr->CreateChordFinder(magField);
  hadCatcherLogical->SetFieldManager(fieldMgr, true);
  muonFinderLogical->SetFieldManager(fieldMgr, true);

  // FASER2 magnetic field
  G4ThreeVector fieldValueFASER2 = GeometricalParameters::Get()->GetSpectrometerMagnetField();
  magFieldFASER2 = new G4UniformMagField(fieldValueFASER2);
  fieldMgrFASER2 = new G4FieldManager();
  fieldMgrFASER2->SetDetectorField(magFieldFASER2);
  fieldMgrFASER2->CreateChordFinder(magFieldFASER2);
  FASER2MagnetLogical->SetFieldManager(fieldMgrFASER2, true);

}

//void DetectorConstruction::UpdateGeometry() {
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