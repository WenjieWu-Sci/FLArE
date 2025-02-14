#include "geometry/FASERnu2DetectorConstruction.hh"
#include "geometry/GeometricalParameters.hh"
#include "DetectorConstructionMaterial.hh"

#include "G4AssemblyVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"
#include "G4PVReplica.hh"

FASERnu2DetectorConstruction::FASERnu2DetectorConstruction()
{
  // load materials
  fMaterials = DetectorConstructionMaterial::GetInstance();

  G4cout << "Building FASERnu2 Emulsion Detector" << G4endl;
 
  fNEmulsionTungstenLayers = GeometricalParameters::Get()->GetNEmulsionTungstenLayers();
  fTungstenThickness = GeometricalParameters::Get()->GetTungstenThickness();
  fEmulsionThickness = GeometricalParameters::Get()->GetEmulsionThickness();
  fEmulsionTungstenSizeX = GeometricalParameters::Get()->GetEmulsionTungstenSizeX();
  fEmulsionTungstenSizeY = GeometricalParameters::Get()->GetEmulsionTungstenSizeY();
  fVetoInterfaceSizeZ = GeometricalParameters::Get()->GetVetoInterfaceSizeZ();
  fVetoInterfaceSizeX = GeometricalParameters::Get()->GetVetoInterfaceSizeX();
  fVetoInterfaceSizeY = GeometricalParameters::Get()->GetVetoInterfaceSizeY();

  fModuleThickness = (fNEmulsionTungstenLayers/2.)*(fEmulsionThickness+fTungstenThickness);
  G4double totLengthZ = 2*fModuleThickness + 3*fVetoInterfaceSizeZ;
  G4double totLengthX = (fVetoInterfaceSizeX > fEmulsionTungstenSizeX) ? fVetoInterfaceSizeX : fEmulsionTungstenSizeX;
  G4double totLengthY = (fVetoInterfaceSizeY > fEmulsionTungstenSizeY) ? fVetoInterfaceSizeY : fEmulsionTungstenSizeY;
  GeometricalParameters::Get()->SetFASERnu2TotalSizeZ(totLengthZ);

  BuildEmulsionTungstenModule();
  BuildVetoInterfaceDetector(); 

  // create a top-level logical volume container
  auto containerSolid = new G4Box("FASERnu2Solid",totLengthX/2.,totLengthY/2.,totLengthZ/2.);
  fFASERnu2Assembly = new G4LogicalVolume(containerSolid, fMaterials->Material("Air"), "FASERnu2Logical");

  // center of the assembly is the center of middle interface detector
  G4ThreeVector FASERnu2Center(0.,0.,0.);
  G4RotationMatrix *noRot = new G4RotationMatrix();

  // placing middle interface detector
  new G4PVPlacement(noRot, FASERnu2Center, fInterfaceDetector, "InterfaceDetPhysical", fFASERnu2Assembly, false, 0, false);
    
  // placing the two emulsion sandwiches
  for( int i=-1; i<=1; i=i+2 ){
    G4ThreeVector pos = FASERnu2Center + i*G4ThreeVector(0.,0.,(fModuleThickness+fVetoInterfaceSizeZ)/2.);
    fEmulsionTungstenModule->MakeImprint(fFASERnu2Assembly, pos, noRot, 0, false);
  }

  // placing the outer interface detectors
  int k = 1;
  for( int i=-1; i<=1; i=i+2 ){
    G4ThreeVector pos = FASERnu2Center + i*G4ThreeVector(0.,0.,fModuleThickness+1.0*fVetoInterfaceSizeZ);
    new G4PVPlacement(noRot, pos, fInterfaceDetector, "InterfaceDetPhysical", fFASERnu2Assembly, false, k, false);
    k++;
  }

  // visibility
  G4VisAttributes* vetodetVis = new G4VisAttributes(G4Colour(234./255, 173./255, 26./255, 0.8));
  vetodetVis->SetVisibility(true);
  fInterfaceDetector->SetVisAttributes(vetodetVis);
  
  G4VisAttributes* emulsionVis = new G4VisAttributes(G4Colour(34./255, 148./255, 83./255, 0.8));
  emulsionVis->SetVisibility(true);
  fEmulsionFilm->SetVisAttributes(emulsionVis);  

  G4VisAttributes* tungstenVis = new G4VisAttributes(G4Colour(128./255, 128./255, 128./255, 0.8));
  tungstenVis->SetVisibility(true);
  fTungstenPlate->SetVisAttributes(tungstenVis);  
}

FASERnu2DetectorConstruction::~FASERnu2DetectorConstruction()
{ 
  delete fFASERnu2Assembly;
  delete fEmulsionFilm;
  delete fTungstenPlate;
  delete fInterfaceDetector;
}

void FASERnu2DetectorConstruction::BuildEmulsionTungstenModule()
{
  fEmulsionTungstenModule = new G4AssemblyVolume();
  
  // build emulsion layer
  auto emulsionFilmSolid = new G4Box("emulsionFilmSolid", fEmulsionTungstenSizeX/2., fEmulsionTungstenSizeY/2., fEmulsionThickness/2.);
  fEmulsionFilm = new G4LogicalVolume(emulsionFilmSolid, fMaterials->Material("Emulsion"), "emulsionFilmLogical");

  // build tungsten layer
  auto tungstenPlateSolid = new G4Box("tungstenPlateSolid", fEmulsionTungstenSizeX/2., fEmulsionTungstenSizeY/2., fTungstenThickness/2.);
  fTungstenPlate = new G4LogicalVolume(tungstenPlateSolid, fMaterials->Material("Tungsten"), "tungstenPlateLogical");
  
  G4RotationMatrix *rot = new G4RotationMatrix();
  G4double offset = -fModuleThickness/2.;
  for (int i=0; i< int(fNEmulsionTungstenLayers/2.); i++){
    G4ThreeVector pos_emu(0, 0, (i+0.5)*fEmulsionThickness+i*fTungstenThickness + offset);
    G4ThreeVector pos_tun(0, 0, (i+1)*fEmulsionThickness+(i+0.5)*fTungstenThickness + offset);
    fEmulsionTungstenModule->AddPlacedVolume(fEmulsionFilm,pos_emu,rot);
    fEmulsionTungstenModule->AddPlacedVolume(fTungstenPlate,pos_tun,rot);
  }
}

void FASERnu2DetectorConstruction::BuildVetoInterfaceDetector()
{
  auto interfaceDetectorSolid = new G4Box("VetoBox", fVetoInterfaceSizeX/2., fVetoInterfaceSizeY/2., fVetoInterfaceSizeZ/2.);
  fInterfaceDetector = new G4LogicalVolume(interfaceDetectorSolid, fMaterials->Material("Polystyrene"), "VetoInterfaceLogical");
}
