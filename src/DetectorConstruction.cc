#include "DetectorConstruction.hh"
#include "DetectorConstructionMaterial.hh"
#include "DetectorConstructionMessenger.hh"
#include "LArBoxSD.hh"

#include "geometry/SpectrometerMagnetConstruction.hh"
#include "geometry/GeometricalParameters.hh"
#include "geometry/FASERnu2DetectorConstruction.hh"
#include "geometry/FORMOSADetectorConstruction.hh"
#include "geometry/FLArEDetectorConstruction.hh"
#include "geometry/SamplingCalorimeterConstruction.hh"

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
  : G4VUserDetectorConstruction(), 
    m_addFLArE(true), m_addFORMOSA(true), m_addFASERnu2(true), m_addFASER2(true),
    m_addSamplingCalorimeter(false)
{
  DefineMaterial();
  messenger = new DetectorConstructionMessenger(this);
  m_saveGdml = false;
  fCheckOverlap = false;
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
  auto worldBox = new G4Box("worldBox", 30*m/2, 30*m/2, 150*m/2);
  worldLV = new G4LogicalVolume(worldBox, LArBoxMaterials->Material("Rock"), "worldLV");
  auto worldPV = new G4PVPlacement(nullptr,
                                   {},
                                   worldLV,
                                   "worldPV",
                                   nullptr,
                                   false, 
                                   0);

  // FPF long paper: https://dx.doi.org/10.1088/1361-6471/ac865e
  // section 2.1.1, figure 5, and figure 7
  G4double hallSizeX  = 8.5 * m;
  G4double hallSizeY  = 7.2 * m;
  G4double hallSizeZ  = 65. * m;
  G4ThreeVector hallOffset(0., 0., hallSizeZ/2 -            // this offset accounts for the distance between 
      GeometricalParameters::Get()->GetHallHeadDistance()); // the entrance wall of the hall and the 
                                                            // first detector, so the first detector 
                                                            // starts at the center of the global coordinate
                                                           
  auto hallBox = new G4Box("hallBox", hallSizeX/2, hallSizeY/2, hallSizeZ/2);
  hallLV = new G4LogicalVolume(hallBox, LArBoxMaterials->Material("Air"), "hallLV");
  auto hallPV = new G4PVPlacement(nullptr,
                                  hallOffset,
                                  hallLV,
                                  "hallPV",
                                  worldLV,
                                  false, 
                                  0,
                                  fCheckOverlap);

  //-----------------------------------
  // FLArE TPC volume, HadCal, and MuonCatcher

  if (m_addFLArE) {
    FLArEDetectorConstruction *FLArEAssembler = new FLArEDetectorConstruction();
    G4double lArSizeZ               = GeometricalParameters::Get()->GetTPCSizeZ();
    G4double TPCInsulationThickness = GeometricalParameters::Get()->GetTPCInsulationThickness();
    G4double GapToHadCatcher        = TPCInsulationThickness;
    G4double HadCatcherLength       = GeometricalParameters::Get()->GetHadCalLength();
    G4double MuonFinderLength       = GeometricalParameters::Get()->GetMuonCatcherLength();
    G4double lengthFLArE            = TPCInsulationThickness + lArSizeZ + GapToHadCatcher + 
                                      HadCatcherLength + MuonFinderLength;

    G4AssemblyVolume* FLArEAssembly = FLArEAssembler->GetFLArEAssembly();
    TPCModuleLogical            = FLArEAssembler->GetFLArETPCVolume();
    HadCalXCellLogical          = FLArEAssembler->GetHadCalXVolume();
    HadCalYCellLogical          = FLArEAssembler->GetHadCalYVolume();
    HadAbsorLayersLogical       = FLArEAssembler->GetHadCalAbsorbVolume();
    MuonFinderXCellLogical      = FLArEAssembler->GetMuonCatcherXVolume();
    MuonFinderYCellLogical      = FLArEAssembler->GetMuonCatcherYVolume();
    MuonFinderAbsorLayersLogical= FLArEAssembler->GetMuonCatcherAbsorbVolume();

    // positioning
    G4ThreeVector FLArEPos = GeometricalParameters::Get()->GetFLArEPosition();
    FLArEPos -= hallOffset;
    FLArEAssembly->MakeImprint(hallLV, FLArEPos, nullptr, 0, fCheckOverlap);

    G4cout<<"Length of FLArE     : "<<lengthFLArE<<G4endl;
    G4cout<<"Center of FLArE TPC : "<<FLArEPos+hallOffset<<G4endl; // w.r.t the global coordinate
  }

  //-----------------------------------
  // FORMOSA

  if (m_addFORMOSA) {
    FORMOSADetectorConstruction *FORMOSAAssembler = new FORMOSADetectorConstruction();
    G4AssemblyVolume* FORMOSAAssembly = FORMOSAAssembler->GetFORMOSAAssembly();
    FORMOSAScintillatorBarLogical = FORMOSAAssembler->GetScintillatorBar();

    // positioning
    G4double lengthFORMOSA = GeometricalParameters::Get()->GetFORMOSATotalSizeZ();
    G4ThreeVector FORMOSAPos = GeometricalParameters::Get()->GetFORMOSAPosition();
    FORMOSAPos -= hallOffset;
    FORMOSAAssembly->MakeImprint(hallLV, FORMOSAPos, nullptr, 0, fCheckOverlap);

    G4cout<<"Length of FORMOSA : "<<lengthFORMOSA<<G4endl;
    G4cout<<"Center of FORMOSA : "<<FORMOSAPos+hallOffset<<G4endl; // w.r.t the global coordinate
  }
                         
  //-----------------------------------
  // FASERnu2 Emulsion Detector

  if (m_addFASERnu2) {
    FASERnu2DetectorConstruction *FASERnu2Assembler = new FASERnu2DetectorConstruction();
    FASERnu2EmulsionLogical = FASERnu2Assembler->GetEmulsionFilm();
    FASERnu2VetoInterfaceLogical = FASERnu2Assembler->GetVetoInterfaceDetector();
    G4AssemblyVolume* FASERnu2Assembly = FASERnu2Assembler->GetFASERnu2Assembly();
    
    // positioning
    G4double lengthFASERnu2 = GeometricalParameters::Get()->GetFASERnu2TotalSizeZ();
    G4ThreeVector FASERnu2Pos = GeometricalParameters::Get()->GetFASERnu2Position();
    FASERnu2Pos -= hallOffset;
    FASERnu2Assembly->MakeImprint(hallLV, FASERnu2Pos, nullptr, 0, fCheckOverlap);

    G4cout<<"Length of FASERnu2 : "<<lengthFASERnu2<<G4endl;
    G4cout<<"Center of FASERnu2 : "<<FASERnu2Pos+hallOffset<<G4endl; // w.r.t the global coordinate
  }

  //-----------------------------------
  // FASER2 Magnet + Tracking stations

  if (m_addFASER2) {
    SpectrometerMagnetConstruction *magnetAssembler = new SpectrometerMagnetConstruction();
    FASER2MagnetLogical = magnetAssembler->GetMagneticVolume(); //need to assign B field
    TrackingVerScinBarLogical = magnetAssembler->GetVerTrackingScinBar(); //need to assign SD
    TrackingHorScinBarLogical = magnetAssembler->GetHorTrackingScinBar(); //need to assign SD
    G4AssemblyVolume* magnetAssembly = magnetAssembler->GetSpectrometerMagnetAssembly();

    // positioning
    G4double lengthSpectrometerMagnetAssembly = GeometricalParameters::Get()->GetFASER2TotalSizeZ();
    G4ThreeVector magPos = GeometricalParameters::Get()->GetFASER2Position();
    GeometricalParameters::Get()->SetMagnetZPosition(magPos.z()); // save for momentum analysis
    magPos -= hallOffset;
    magnetAssembly->MakeImprint(hallLV, magPos, nullptr, 0, fCheckOverlap);

    //detectorGapLength = 1.2*m;
    G4cout<<"Length of FASER2 Spectrometer : "<<lengthSpectrometerMagnetAssembly<<G4endl;
    G4cout<<"Center of FASER2 Spectrometer : "<<magPos+hallOffset<<G4endl; // w.r.t the global coordinate
  }

  //-----------------------------------
  // Sampling Calorimeter (A NuTeV-like detector)

  if (m_addSamplingCalorimeter) {

    SamplingCalorimeterConstruction* SamplingCaloAssembler = new SamplingCalorimeterConstruction();
    G4AssemblyVolume* SamplingCaloAssembly = SamplingCaloAssembler->GetSamplingCalorimeterAssembly();
    AbsorbLayerLogical       = SamplingCaloAssembler->GetSamplingCalorimeterAbsorbVolume();
    CaloXCellLogical         = SamplingCaloAssembler->GetSamplingCalorimeterCaloXVolume();
    CaloYCellLogical         = SamplingCaloAssembler->GetSamplingCalorimeterCaloYVolume();

    // positioning
    G4ThreeVector SamplingCaloPos = GeometricalParameters::Get()->GetSamplingCaloPosition();
    SamplingCaloPos -= hallOffset;
    SamplingCaloAssembly->MakeImprint(hallLV, SamplingCaloPos, nullptr, 0, fCheckOverlap);

    G4cout<<"Length of the Sampling Calorimeter : "<<GeometricalParameters::Get()->GetSamplingCaloSizeZ()<<G4endl;
    G4cout<<"Sampling Calo Pos : "<<SamplingCaloPos<<G4endl; // w.r.t the global coordinate
    G4cout<<"Hall Offset       : "<<hallOffset<<G4endl; // w.r.t the global coordinate
    G4cout<<"Center of the Sampling Calorimeter : "<<SamplingCaloPos+hallOffset<<G4endl; // w.r.t the global coordinate
  }

  
  //-------------------------------------------------------------------

  // visualization
  G4VisAttributes* worldVis = new G4VisAttributes(G4Colour(167./255, 168./255, 189./255));
  worldVis->SetVisibility(false);
  worldLV->SetVisAttributes(worldVis);

  G4VisAttributes* hallVis = new G4VisAttributes(G4Colour(167./255, 168./255, 189./255));
  hallVis->SetVisibility(true);
  hallLV->SetVisAttributes(hallVis);

  if (m_saveGdml) {
    G4GDMLParser fParser;
    fParser.Write("LArBoxDetGeo.gdml", worldPV);
  }

  return worldPV;
}

void DetectorConstruction::ConstructSDandField() {
  G4SDManager* sdManager = G4SDManager::GetSDMpointer();
  sdManager->SetVerboseLevel(2);
  int SDIdx = 0;
  if (m_addFLArE) {
    LArBoxSD* TPCModuleSD = new LArBoxSD("lArBoxSD");
    TPCModuleLogical->SetSensitiveDetector(TPCModuleSD);
    sdManager->AddNewDetector(TPCModuleSD);
    GeometricalParameters::Get()->AddSD2List(SDIdx, "lArBoxSD/lar_box");
    SDIdx++;

    LArBoxSD* HadCalXSD = new LArBoxSD("HadCalXSD");
    HadCalXCellLogical->SetSensitiveDetector(HadCalXSD);
    sdManager->AddNewDetector(HadCalXSD);
    GeometricalParameters::Get()->AddSD2List(SDIdx, "HadCalXSD/lar_box");
    SDIdx++;

    LArBoxSD* HadCalYSD = new LArBoxSD("HadCalYSD");
    HadCalYCellLogical->SetSensitiveDetector(HadCalYSD);
    sdManager->AddNewDetector(HadCalYSD);
    GeometricalParameters::Get()->AddSD2List(SDIdx, "HadCalYSD/lar_box");
    SDIdx++;

    LArBoxSD* MuonFinderXSD = new LArBoxSD("MuonFinderXSD");
    MuonFinderXCellLogical->SetSensitiveDetector(MuonFinderXSD);
    sdManager->AddNewDetector(MuonFinderXSD);
    GeometricalParameters::Get()->AddSD2List(SDIdx, "MuonFinderXSD/lar_box");
    SDIdx++;

    LArBoxSD* MuonFinderYSD = new LArBoxSD("MuonFinderYSD");
    MuonFinderYCellLogical->SetSensitiveDetector(MuonFinderYSD);
    sdManager->AddNewDetector(MuonFinderYSD);
    GeometricalParameters::Get()->AddSD2List(SDIdx, "MuonFinderYSD/lar_box");
    SDIdx++;

    LArBoxSD* HadAbsorbSD = new LArBoxSD("HadAbsorbSD");
    HadAbsorLayersLogical->SetSensitiveDetector(HadAbsorbSD);
    sdManager->AddNewDetector(HadAbsorbSD);
    GeometricalParameters::Get()->AddSD2List(SDIdx, "HadAbsorbSD/lar_box");
    SDIdx++;

    LArBoxSD* MuonFinderAbsorbSD = new LArBoxSD("MuonFinderAbsorbSD");
    MuonFinderAbsorLayersLogical->SetSensitiveDetector(MuonFinderAbsorbSD);
    sdManager->AddNewDetector(MuonFinderAbsorbSD);
    GeometricalParameters::Get()->AddSD2List(SDIdx, "MuonFinderAbsorbSD/lar_box");
    SDIdx++;

    // HadCatcher + MuonFinder  magnetic field
    G4ThreeVector fieldValue = G4ThreeVector(0,fFieldValue, 0);
    magField = new G4UniformMagField(fieldValue);
    fieldMgr = new G4FieldManager();
    fieldMgr->SetDetectorField(magField);
    fieldMgr->CreateChordFinder(magField);
    HadCalXCellLogical->SetFieldManager(fieldMgr, true);
    HadCalYCellLogical->SetFieldManager(fieldMgr, true);
    HadAbsorLayersLogical->SetFieldManager(fieldMgr, true);
    MuonFinderXCellLogical->SetFieldManager(fieldMgr, true);
    MuonFinderYCellLogical->SetFieldManager(fieldMgr, true);
    MuonFinderAbsorLayersLogical->SetFieldManager(fieldMgr, true);
  }

  if (m_addFORMOSA) {
    LArBoxSD* ScintillatorBarSD = new LArBoxSD("FORMOSAScinBarSD");
    FORMOSAScintillatorBarLogical->SetSensitiveDetector(ScintillatorBarSD);
    sdManager->AddNewDetector(ScintillatorBarSD);
    GeometricalParameters::Get()->AddSD2List(SDIdx, "FORMOSAScinBarSD/lar_box");
    SDIdx++;
  }

  if (m_addFASERnu2) {
    LArBoxSD* EmulsionFilmSD = new LArBoxSD("FASERnu2EmulsionSD");
    FASERnu2EmulsionLogical->SetSensitiveDetector(EmulsionFilmSD);
    sdManager->AddNewDetector(EmulsionFilmSD);
    GeometricalParameters::Get()->AddSD2List(SDIdx, "FASERnu2EmulsionSD/lar_box");
    SDIdx++;
    
    LArBoxSD* VetoInterfaceSD = new LArBoxSD("FASERnu2VetoInterfaceSD");
    FASERnu2VetoInterfaceLogical->SetSensitiveDetector(VetoInterfaceSD);
    sdManager->AddNewDetector(VetoInterfaceSD);
    GeometricalParameters::Get()->AddSD2List(SDIdx, "FASERnu2VetoInterfaceSD/lar_box");
    SDIdx++;
  }

  if (m_addFASER2) {
    LArBoxSD* TrkHorScinSD = new LArBoxSD("TrkHorScinSD");
    TrackingHorScinBarLogical->SetSensitiveDetector(TrkHorScinSD);
    sdManager->AddNewDetector(TrkHorScinSD);
    GeometricalParameters::Get()->AddSD2List(SDIdx, "TrkHorScinSD/lar_box");
    SDIdx++;

    LArBoxSD* TrkVerScinSD = new LArBoxSD("TrkVerScinSD");
    TrackingVerScinBarLogical->SetSensitiveDetector(TrkVerScinSD);
    sdManager->AddNewDetector(TrkVerScinSD);
    GeometricalParameters::Get()->AddSD2List(SDIdx, "TrkVerScinSD/lar_box");
    SDIdx++;

    // FASER2 magnetic field
    G4ThreeVector fieldValueFASER2 = GeometricalParameters::Get()->GetSpectrometerMagnetField();
    magFieldFASER2 = new G4UniformMagField(fieldValueFASER2);
    fieldMgrFASER2 = new G4FieldManager();
    fieldMgrFASER2->SetDetectorField(magFieldFASER2);
    fieldMgrFASER2->CreateChordFinder(magFieldFASER2);
    FASER2MagnetLogical->SetFieldManager(fieldMgrFASER2, true);
  }

  if (m_addSamplingCalorimeter) {
    LArBoxSD* SamplingCaloAbsorbSD = new LArBoxSD("SamplingCaloAbsorbSD");
    AbsorbLayerLogical->SetSensitiveDetector(SamplingCaloAbsorbSD);
    sdManager->AddNewDetector(SamplingCaloAbsorbSD);
    GeometricalParameters::Get()->AddSD2List(SDIdx, "SamplingCaloAbsorbSD/lar_box");
    SDIdx++;

    LArBoxSD* SamplingCaloXSD = new LArBoxSD("SamplingCaloXSD");
    CaloXCellLogical->SetSensitiveDetector(SamplingCaloXSD);
    sdManager->AddNewDetector(SamplingCaloXSD);
    GeometricalParameters::Get()->AddSD2List(SDIdx, "SamplingCaloXSD/lar_box");
    SDIdx++;

    LArBoxSD* SamplingCaloYSD = new LArBoxSD("SamplingCaloYSD");
    CaloYCellLogical->SetSensitiveDetector(SamplingCaloYSD);
    sdManager->AddNewDetector(SamplingCaloYSD);
    GeometricalParameters::Get()->AddSD2List(SDIdx, "SamplingCaloYSD/lar_box");
    SDIdx++;
  }
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
