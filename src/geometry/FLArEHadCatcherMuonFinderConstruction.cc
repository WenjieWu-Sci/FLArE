#include "geometry/FLArEHadCatcherMuonFinderConstruction.hh"
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

FLArEHadCatcherMuonFinderConstruction::FLArEHadCatcherMuonFinderConstruction()
{
  // load materials
  fMaterials = DetectorConstructionMaterial::GetInstance();
  fLArSizeX            = GeometricalParameters::Get()->GetTPCSizeX();
  fLArSizeY            = GeometricalParameters::Get()->GetTPCSizeY();

  G4cout << "Building FLArE Hadron Catcher and Muon Finder" << G4endl;

  BuildFLArEHadCal();
  BuildFLArEMuonCatcher();

  fHadCatcherMuonFinderAssembly = new G4AssemblyVolume();
  G4RotationMatrix* noRot = new G4RotationMatrix();
  G4ThreeVector assemblyCenter(0.,0.,0.);
  G4double totalLength = fHadCalLength + fMuonCatcherLength;

  // HadCal
  G4ThreeVector hadCalCenter(0.,0.,-totalLength/2. +fHadCalLength/2.);
  fHadCatcherMuonFinderAssembly->AddPlacedAssembly(HadCalAssembly,hadCalCenter,noRot);

  // MuonCatcher
  G4ThreeVector MuonCatcherCenter(0.,0.,-totalLength/2.+fHadCalLength+fMuonCatcherLength/2.);
  fHadCatcherMuonFinderAssembly->AddPlacedAssembly(MuonCatcherAssembly,MuonCatcherCenter,noRot);

  // visualization
  G4VisAttributes* absorVis = new G4VisAttributes(G4Colour(234./255, 173./255, 26./255, 0.8));
  absorVis->SetVisibility(true);
  HadAbsorLayersLogical->SetVisAttributes(absorVis);
  MuonFinderAbsorLayersLogical->SetVisAttributes(absorVis);

  G4VisAttributes* nullVis = new G4VisAttributes(G4Colour(167./255, 168./255, 189./255));
  nullVis->SetVisibility(false);
  HadCalXCellLogical->SetVisAttributes(nullVis);
  HadCalYCellLogical->SetVisAttributes(nullVis);
  MuonFinderXCellLogical->SetVisAttributes(nullVis);
  MuonFinderYCellLogical->SetVisAttributes(nullVis);

}

FLArEHadCatcherMuonFinderConstruction::~FLArEHadCatcherMuonFinderConstruction()
{
  delete fHadCatcherMuonFinderAssembly;
  delete HadAbsorLayersLogical;
  delete HadCalXCellLogical;
  delete HadCalYCellLogical;
  delete HadCalAssembly;
  delete MuonFinderAbsorLayersLogical;
  delete MuonFinderXCellLogical;
  delete MuonFinderYCellLogical;
  delete MuonCatcherAssembly;
}

void FLArEHadCatcherMuonFinderConstruction::BuildFLArEHadCal()
{
  G4double thicknessAbsorber = 5 * cm;
  G4double thicknessCaloX    = 1 * cm;
  G4double thicknessCaloY    = 1 * cm;
  G4int fNbOfAbsor = 15;

  G4double thicknessOneLayer = thicknessAbsorber + thicknessCaloX + thicknessCaloY;
  fHadCalLength = fNbOfAbsor*thicknessOneLayer;
  GeometricalParameters::Get()->SetHadCalLength(fHadCalLength);

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

void FLArEHadCatcherMuonFinderConstruction::BuildFLArEMuonCatcher() {
  G4double thicknessAbsorber = 16 * cm;
  G4double thicknessCaloX    = 1 * cm;
  G4double thicknessCaloY    = 1 * cm;
  G4int    fNbOfAbsor = 2;
  G4double thicknessOneLayer = thicknessAbsorber + thicknessCaloX + thicknessCaloY;
  fMuonCatcherLength = fNbOfAbsor*thicknessOneLayer;
  GeometricalParameters::Get()->SetMuonCatcherLength(fMuonCatcherLength);
  
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
