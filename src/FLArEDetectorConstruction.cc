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

G4ThreadLocal G4UniformMagField* FLArEDetectorConstruction::magField = 0;
G4ThreadLocal G4FieldManager* FLArEDetectorConstruction::fieldMgr = 0;
G4ThreadLocal G4UniformMagField* FLArEDetectorConstruction::magFieldFASER2 = 0;
G4ThreadLocal G4FieldManager* FLArEDetectorConstruction::fieldMgrFASER2 = 0;

FLArEDetectorConstruction::FLArEDetectorConstruction()
  : G4VUserDetectorConstruction(), fDetMaterialName("LAr"), fDetGeomOption("single")
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
  G4double worldSizeZ = 90 * m;
  // LAr volume: +z being beam direction
  G4double lArSizeX = 1.8 * m;
  G4double lArSizeY = 1.8 * m;
  G4double lArSizeZ = 7.0 * m;

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
  new G4PVPlacement(nullptr, // no Rotation
                    G4ThreeVector(0, 0, 0), // no transporation
                    lArBoxLog,              // current logical volume
                    "lArBox",               // name
                    worldLog,               // mother logical volume
                    false,                  // pMany
                    0,                      // Copy No
                    fCheckOverlap);

  G4VisAttributes* lArBoxVis;
  if (fDetGeomOption=="single") {
    lArBoxVis = new G4VisAttributes(G4Colour(86./255, 152./255, 195./255));
    lArBoxVis->SetVisibility(true);
    lArBoxVis->SetForceWireframe(true);
    lArBoxVis->SetForceAuxEdgeVisible(true);
    lArBoxLog->SetVisAttributes(lArBoxVis);
    lArBoxLog->SetUserLimits(new G4UserLimits(0.5*mm));
  } else if (fDetGeomOption=="3x7") {
    lArBoxVis = new G4VisAttributes(G4Colour(86./255, 152./255, 195./255));
    lArBoxVis->SetVisibility(false);
    lArBoxLog->SetVisAttributes(lArBoxVis);

    auto TPCLayerSolid
      = new G4Box("TPCLayerBox", 1.8*m/2, 1.8*m/2, 1.0*m/2);
    auto TPCLayerLogical
      = new G4LogicalVolume(TPCLayerSolid, detectorMaterial, "TPCLayerLogical");
    auto TPCModuleSolid
      = new G4Box("TPCModuleBox", 0.6*m/2, 1.8*m/2, 1.0*m/2);
    TPCModuleLogical
      = new G4LogicalVolume(TPCModuleSolid, detectorMaterial, "TPCModuleLogical");
    new G4PVReplica("TPCModulePhysical", TPCModuleLogical, TPCLayerLogical, kXAxis, 3, 0.6*m);
    new G4PVReplica("TPC", TPCLayerLogical, lArBoxLog, kZAxis, 7, 1.0*m);
    G4VisAttributes* TPCModuleVis = new G4VisAttributes(G4Colour(86./255, 152./255, 195./255));
    TPCModuleVis->SetVisibility(true);
    TPCModuleVis->SetForceWireframe(true);
    TPCModuleVis->SetForceAuxEdgeVisible(true);
    TPCLayerLogical->SetVisAttributes(lArBoxVis);
    TPCModuleLogical->SetVisAttributes(TPCModuleVis);
    TPCModuleLogical->SetUserLimits(new G4UserLimits(0.5*mm));
  }

  G4double GapToHadCatcher = 25 * cm;

  // Aluminum cryostat: 1/2 density
  auto crygapSolid = new G4Box("CryGapBox", lArSizeX/2, lArSizeY/2, GapToHadCatcher/2);
  crygapLogical = new G4LogicalVolume(crygapSolid, LArBoxMaterials->Material("FakeAluminium"), "CryGapLogical");
  new G4PVPlacement(nullptr,
                    G4ThreeVector(0, 0, lArSizeZ/2+GapToHadCatcher/2),
                    crygapLogical,
                    "CryGapPhysical",
                    worldLog,
                    false,
                    0,
                    fCheckOverlap);

  //
  // Hadron Calorimeter
  // 
  G4double thicknessAbsorber = 5 * cm;
  G4double thicknessCaloX    = 1 * cm;
  G4double thicknessCaloY    = 1 * cm;
  G4int fNbOfAbsor = 15;

  G4double thicknessOneLayer = thicknessAbsorber + thicknessCaloX + thicknessCaloY;
  G4double HadCatcherLength  = fNbOfAbsor*thicknessOneLayer;

  auto hadCatcherSolid
    = new G4Box("HadCatcherBox", lArSizeX/2, lArSizeY/2, HadCatcherLength/2);
  hadCatcherLogical
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
  HadAbsorLayersLogical
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

  //
  // Muon Finder
  // 
  thicknessAbsorber = 16 * cm;
  thicknessCaloX    = 1 * cm;
  thicknessCaloY    = 1 * cm;
  fNbOfAbsor = 2;
  thicknessOneLayer = thicknessAbsorber + thicknessCaloX + thicknessCaloY;
  G4double MuonFinderLength  = fNbOfAbsor*thicknessOneLayer;

  auto muonFinderSolid
    = new G4Box("MuonFinderBox", lArSizeX/2, lArSizeY/2, MuonFinderLength/2);
  muonFinderLogical
    = new G4LogicalVolume(muonFinderSolid, LArBoxMaterials->Material("Air"), "MuonFinderLogical");
  new G4PVPlacement(nullptr,
                    G4ThreeVector(0, 0, lArSizeZ/2+MuonFinderLength/2+GapToHadCatcher+HadCatcherLength),
                    muonFinderLogical,
                    "MuonFinderPhysical",
                    worldLog,
                    false,
                    0,
                    fCheckOverlap);

  // Absorber
  auto MuonFinderAbsorLayersSolid
    = new G4Box("MuonFinderAbsorLayersBox", lArSizeX/2, lArSizeY/2, thicknessAbsorber/2);
  MuonFinderAbsorLayersLogical
    = new G4LogicalVolume(MuonFinderAbsorLayersSolid, LArBoxMaterials->Material("Iron"), "MuonFinderAbsorLayersLogical");

  // X-Plane
  auto MuonFinderXLayersSolid
    = new G4Box("MuonFinderXLayersBox", lArSizeX/2, lArSizeY/2, thicknessCaloX/2);
  auto MuonFinderXLayersLogical
    = new G4LogicalVolume(MuonFinderXLayersSolid, LArBoxMaterials->Material("Polystyrene"), "MuonFinderXLayersLogical");

  auto MuonFinderXCellSolid
    = new G4Box("MuonFinderXCellBox", thicknessCaloX/2, lArSizeY/2, thicknessCaloX/2);
  MuonFinderXCellLogical
    = new G4LogicalVolume(MuonFinderXCellSolid, LArBoxMaterials->Material("Polystyrene"), "MuonFinderXCellLogical");
  new G4PVReplica("MuonFinderXCellPhysical", MuonFinderXCellLogical,
                  MuonFinderXLayersLogical, kXAxis, lArSizeX/thicknessCaloX, thicknessCaloX);

  // Y-Plane
  auto MuonFinderYLayersSolid
    = new G4Box("MuonFinderYLayersBox", lArSizeX/2, lArSizeY/2, thicknessCaloY/2);
  auto MuonFinderYLayersLogical
    = new G4LogicalVolume(MuonFinderYLayersSolid, LArBoxMaterials->Material("Polystyrene"), "MuonFinderYLayersLogical");

  auto MuonFinderYCellSolid
    = new G4Box("MuonFinderYCellBox", lArSizeX/2, thicknessCaloY/2, thicknessCaloY/2);
  MuonFinderYCellLogical
    = new G4LogicalVolume(MuonFinderYCellSolid, LArBoxMaterials->Material("Polystyrene"), "MuonFinderYCellLogical");
  new G4PVReplica("MuonFinderYCellPhysical", MuonFinderYCellLogical,
                  MuonFinderYLayersLogical, kYAxis, lArSizeY/thicknessCaloY, thicknessCaloY);

  auto MuonFinderAbsorAssembly = new G4AssemblyVolume();
  G4RotationMatrix Ra_muonFinder(0, 0, 0);
  G4ThreeVector Ta_muonFinder(0, 0, thicknessAbsorber/2);
  MuonFinderAbsorAssembly->AddPlacedVolume(MuonFinderAbsorLayersLogical, Ta_muonFinder, &Ra_muonFinder);
  Ta_muonFinder.setZ(thicknessAbsorber+thicknessCaloX/2);
  MuonFinderAbsorAssembly->AddPlacedVolume(MuonFinderXLayersLogical, Ta_muonFinder, &Ra_muonFinder);
  Ta_muonFinder.setZ(thicknessAbsorber+thicknessCaloX+thicknessCaloY/2);
  MuonFinderAbsorAssembly->AddPlacedVolume(MuonFinderYLayersLogical, Ta_muonFinder, &Ra_muonFinder);

  for (int i= 0; i< fNbOfAbsor; ++i) {
    G4RotationMatrix Rm(0, 0, 0);
    G4ThreeVector Tm(0, 0, thicknessOneLayer*i - MuonFinderLength/2);
    MuonFinderAbsorAssembly->MakeImprint(muonFinderLogical, Tm, &Rm);
  }

  //-----------------------------------
  // FASER2 Magnet
  
  // size: latest baseline 
  G4double magnetWindowX = 3.0*m;
  G4double magnetWindowY = 1.0*m;
  G4double magnetWindowZ = 4.0*m;

  G4double magnetYokeThicknessX = 1.5*m;
  G4double magnetYokeThicknessY = 2.0*m;

  // positioning
  G4double detectorGapLength = 1.2*m;
  G4double lengthFORMOSA = 4.0*m;
  G4double lengthFASERnu2 = 8.0*m;

  G4double lengthDecayTunnelFASER2 = 10*m;
  G4double lengthVetoStationFASER2 = 20.*cm; //guesses (including gaps)
  G4double lengthTrackStationFASER2 = 3.*m; //6 tracking stations + gas

  // from the center of FLArE lAr volume 
  G4double magnetPosZ = (lArSizeZ/2. + GapToHadCatcher + HadCatcherLength + MuonFinderLength) + 
			detectorGapLength + lengthFORMOSA + detectorGapLength + lengthFASERnu2 +
			detectorGapLength + lengthVetoStationFASER2 + lengthDecayTunnelFASER2 +
			lengthTrackStationFASER2 + magnetWindowZ/2.;  

  auto magnetYokeBlock = new G4Box("MagnetYokeBlock", magnetWindowX/2.+magnetYokeThicknessX, magnetWindowY/2.+magnetYokeThicknessY, magnetWindowZ/2.);
  auto magnetWindowSolid = new G4Box("MagnetYokeWindow", magnetWindowX/2., magnetWindowY/2., magnetWindowZ/2.);
  auto magnetYokeSolid = new G4SubtractionSolid("MagnetYoke",
				magnetYokeBlock, // block - window = hollow block
				magnetWindowSolid, 
				0, // no rotation
				G4ThreeVector(0,0,0)); //no translation
  
  FASER2MagnetYoke = new G4LogicalVolume(magnetYokeSolid, LArBoxMaterials->Material("Iron"), "FASER2MagnetYokeLogical");
  FASER2MagnetWindow = new G4LogicalVolume(magnetWindowSolid, LArBoxMaterials->Material("Air"), "FASER2MagnetWindowLogical"); 

  new G4PVPlacement(nullptr,      // no Rotation
                    G4ThreeVector(0, 0, magnetPosZ), // translation 
                    FASER2MagnetYoke,         //logical volume
                    "FASER2MagnetYokePhys",   // name
                    worldLog,                 // mother logical volume
                    false,                    // pMany
                    0,                        // Copy No
                    fCheckOverlap);

  new G4PVPlacement(nullptr,    // no Rotation
                    G4ThreeVector(0, 0, magnetPosZ), // translation
                    FASER2MagnetWindow,       // logical volume
                    "FASER2MagnetWindowPhys",  // name
                    worldLog,                 // mother logical volume
                    false,                    // pMany
                    0,                        // Copy No
                    fCheckOverlap);


  //-----------------------------------------------------------------
  //FASER2 tracking stations (before/after magnet)

  // 3 assemblies, each made of 2 layers
  // 1st layer: 2 horizontal modules; (tot heigth: 1m)
  // 2nd layer: 6/7 vertical modules; (tot length: 3.5m)
  
  G4double magTrkStationX = magnetWindowX + 0.5*m;
  G4double magTrkStationY = magnetWindowY;

  G4int magTrkNScintY = 2;
  G4int magTrkNScintX = 7;
  G4int NTrackingStations = 6;

  G4double magHorizontalScinSize = magTrkStationY / magTrkNScintY; // y size of horizontal scin
  G4double magVerticalScinSize = magTrkStationX / magTrkNScintX; // x size of vertical scin

  G4double scinThickness = 1 * cm; //guess (probably less, 0.5 cm)
  G4double stationThickness = 2*scinThickness;
  G4double gapThickness = 50*cm;
  G4double gapToMagnet = 50*cm; 

  G4double totThickness = 6*stationThickness + 5*gapThickness;

  auto trkStationSolid = new G4Box("trkStationBox", magTrkStationX/2, magTrkStationY/2., totThickness/2.);
  auto firstTrkStationLogical = new G4LogicalVolume(trkStationSolid, LArBoxMaterials->Material("Polystyrene"), "firstTrkStationLogical");
  auto secondTrkStationLogical = new G4LogicalVolume(trkStationSolid, LArBoxMaterials->Material("Polystyrene"), "secondTrkStationLogical");
  new G4PVPlacement(nullptr,
                    G4ThreeVector(0, 0, magnetPosZ - magnetWindowZ/2.- gapToMagnet - totThickness/2.),
                    firstTrkStationLogical,
                    "firstTrkStationPhysical",
                    worldLog,
                    false,
                    0,
                    fCheckOverlap);
  new G4PVPlacement(nullptr,
                    G4ThreeVector(0, 0, magnetPosZ + magnetWindowZ/2.+ gapToMagnet + totThickness/2.),
                    secondTrkStationLogical,
                    "secondTrkStationPhysical",
                    worldLog,
                    false,
                    0,
                    fCheckOverlap);


  // layers: same size, one with 2 horizontal, one with 7 vertical
  auto trkLayerSolid = new G4Box("trkLayerBox", magTrkStationX/2, magTrkStationY/2., scinThickness/2.);
  auto trkHorLayerLogical = new G4LogicalVolume(trkLayerSolid, LArBoxMaterials->Material("Polystyrene"), "trkHorLayerLogical"); 
  auto trkVerLayerLogical = new G4LogicalVolume(trkLayerSolid, LArBoxMaterials->Material("Polystyrene"), "trkVerLayerLogical"); 
  
  //2 horizontal pieces
  auto trkHorScinSolid = new G4Box("trkHorScinSolid", magTrkStationX/2., magHorizontalScinSize/2., scinThickness/2.);
  trkHorScinLogical = new G4LogicalVolume(trkHorScinSolid, LArBoxMaterials->Material("Polystyrene"), "trkHorScinLogical");
  new G4PVReplica("trkHorScinPhysical", trkHorScinLogical,
                  trkHorLayerLogical, kYAxis, magTrkNScintY, magHorizontalScinSize);
  //7 vertical pieces
  auto trkVerScinSolid = new G4Box("trkHorScinSolid", magVerticalScinSize/2., magTrkStationY/2., scinThickness/2.);
  trkVerScinLogical = new G4LogicalVolume(trkVerScinSolid, LArBoxMaterials->Material("Polystyrene"), "trkVerScinLogical");
  new G4PVReplica("trkVerScinPhysical", trkVerScinLogical,
                  trkVerLayerLogical, kXAxis, magTrkNScintX, magVerticalScinSize);

  auto trackingStationAssembly = new G4AssemblyVolume(); //one assembly has 2 layers
  G4RotationMatrix rot(0, 0, 0);
  G4ThreeVector pos(0, 0, -scinThickness/2.);
  trackingStationAssembly->AddPlacedVolume(trkHorLayerLogical,pos,&rot);
  pos.setZ(scinThickness/2.);
  trackingStationAssembly->AddPlacedVolume(trkVerLayerLogical,pos,&rot);

  for (int i= 0; i<NTrackingStations; ++i) { 
    G4RotationMatrix Rm(0, 0, 0);
    G4ThreeVector Tm(0, 0, -totThickness/2.+0.5*stationThickness+i*(gapThickness+stationThickness));
    trackingStationAssembly->MakeImprint(firstTrkStationLogical, Tm, &Rm); //place before magnet
    trackingStationAssembly->MakeImprint(secondTrkStationLogical, Tm, &Rm); //place after magnet
  }

  //-------------------------------------------------------------------

  // visualization
  //G4VisAttributes* crygapVis = new G4VisAttributes(G4Colour(196./255, 203./255, 207./255, 1.0));

  G4VisAttributes* absorVis = new G4VisAttributes(G4Colour(234./255, 173./255, 26./255, 0.8));
  absorVis->SetVisibility(true);
  //absorVis->SetForceWireframe(true);
  //absorVis->SetForceAuxEdgeVisible(true);
  HadAbsorLayersLogical->SetVisAttributes(absorVis);
  MuonFinderAbsorLayersLogical->SetVisAttributes(absorVis);

  G4VisAttributes* hadCalVis = new G4VisAttributes(G4Colour(34./255, 148./255, 83./255, 0.8));
  hadCalVis->SetVisibility(true);
  //hadCalVis->SetForceWireframe(true);
  //hadCalVis->SetForceAuxEdgeVisible(true);
  HadCalXLayersLogical->SetVisAttributes(hadCalVis);
  HadCalYLayersLogical->SetVisAttributes(hadCalVis);
  MuonFinderXLayersLogical->SetVisAttributes(hadCalVis);
  MuonFinderYLayersLogical->SetVisAttributes(hadCalVis);
  trkVerScinLogical->SetVisAttributes(hadCalVis);  
  trkHorScinLogical->SetVisAttributes(hadCalVis);  

  G4VisAttributes* magnetVis = new G4VisAttributes(G4Colour(234./255, 173./255, 26./255, 0.8));
  magnetVis->SetVisibility(true);
  FASER2MagnetYoke->SetVisAttributes(magnetVis);

  G4VisAttributes* nullVis = new G4VisAttributes(G4Colour(167./255, 168./255, 189./255));
  nullVis->SetVisibility(false);
  worldLog->SetVisAttributes(nullVis);
  crygapLogical->SetVisAttributes(nullVis);
  hadCatcherLogical->SetVisAttributes(nullVis);
  muonFinderLogical->SetVisAttributes(nullVis);
  HadCalXCellLogical->SetVisAttributes(nullVis);
  HadCalYCellLogical->SetVisAttributes(nullVis);
  MuonFinderXCellLogical->SetVisAttributes(nullVis);
  MuonFinderYCellLogical->SetVisAttributes(nullVis);
  FASER2MagnetWindow->SetVisAttributes(nullVis);

  if (m_saveGdml) {
    G4GDMLParser fParser;
    fParser.Write("LArBoxDetGeo.gdml", worldPhys);
  }

  return worldPhys;
}

void FLArEDetectorConstruction::ConstructSDandField() {
  G4SDManager* sdManager = G4SDManager::GetSDMpointer();
  sdManager->SetVerboseLevel(2);

  LArBoxSD* TPCModuleSD = new LArBoxSD("lArBoxSD");
  if (fDetGeomOption=="single") {
//    LArBoxSD* lArBoxSD = new LArBoxSD("lArBoxSD");
    lArBoxLog->SetSensitiveDetector(TPCModuleSD);
    sdManager->AddNewDetector(TPCModuleSD);
  } else if (fDetGeomOption=="3x7") {
    TPCModuleLogical->SetSensitiveDetector(TPCModuleSD);
    sdManager->AddNewDetector(TPCModuleSD);
  }

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

  LArBoxSD* CryGapSD = new LArBoxSD("CryGapSD");
  crygapLogical->SetSensitiveDetector(CryGapSD);
  sdManager->AddNewDetector(CryGapSD);

  LArBoxSD* TrkHorScinSD = new LArBoxSD("TrkHorScinSD");
  trkHorScinLogical->SetSensitiveDetector(TrkHorScinSD);
  sdManager->AddNewDetector(TrkHorScinSD);

  LArBoxSD* TrkVerScinSD = new LArBoxSD("TrkVerScinSD");
  trkVerScinLogical->SetSensitiveDetector(TrkVerScinSD);
  sdManager->AddNewDetector(TrkVerScinSD);

  // HadCatcher + MuonFinder  magnetic field
  G4ThreeVector fieldValue = G4ThreeVector(0,1.*tesla, 0);
  magField = new G4UniformMagField(fieldValue);
  fieldMgr = new G4FieldManager();
  fieldMgr->SetDetectorField(magField);
  fieldMgr->CreateChordFinder(magField);
  hadCatcherLogical->SetFieldManager(fieldMgr, true);
  muonFinderLogical->SetFieldManager(fieldMgr, true);

  // FASER2 magnetic field
  G4ThreeVector fieldValueFASER2(0.,1*tesla,0.); // 1T, horizonal bending
  magFieldFASER2 = new G4UniformMagField(fieldValueFASER2);
  fieldMgrFASER2 = new G4FieldManager();
  fieldMgrFASER2->SetDetectorField(magFieldFASER2);
  fieldMgrFASER2->CreateChordFinder(magFieldFASER2);
  FASER2MagnetWindow->SetFieldManager(fieldMgrFASER2, true);
  
}

void FLArEDetectorConstruction::SetDetMaterial(G4String detMaterial) {
//  if (!lArBoxLog) {
//    G4cerr<<"Detector has not yet been constructed"<<G4endl;
//    return;
//  }

  fDetMaterialName = detMaterial;
//  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

void FLArEDetectorConstruction::SetGeomOption(G4String detGeomOption) {
  fDetGeomOption = detGeomOption;
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
