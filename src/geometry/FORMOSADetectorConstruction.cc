#include "geometry/FORMOSADetectorConstruction.hh"
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
#include "G4PVPlacement.hh"

FORMOSADetectorConstruction::FORMOSADetectorConstruction()
{
  // load materials
  fMaterials = DetectorConstructionMaterial::GetInstance();

  G4cout << "Building FORMOSA Detector" << G4endl;

  fNScinBarsX = GeometricalParameters::Get()->GetNScinBarsX();
  fNScinBarsY = GeometricalParameters::Get()->GetNScinBarsY();
  fScintillatorBarSizeX = GeometricalParameters::Get()->GetScintillatorBarSizeX();
  fScintillatorBarSizeY = GeometricalParameters::Get()->GetScintillatorBarSizeY();
  fScintillatorBarSizeZ = GeometricalParameters::Get()->GetScintillatorBarSizeZ();

  G4int NScintillatorModules = GeometricalParameters::Get()->GetNScintillatorModules();
  fPMTSizeSpacing = GeometricalParameters::Get()->GetPMTSizeSpacing();

  G4double totLengthZ = NScintillatorModules*(fScintillatorBarSizeZ+fPMTSizeSpacing);
  Gedouble totLengthX = fNScinBarsX*fScintillatorBarSizeX;
  Gedouble totLengthY = fNScinBarsY*fScintillatorBarSizeY;
  GeometricalParameters::Get()->SetFORMOSATotalSizeZ(totLengthZ);

  BuildScintillatorAssembly();
  //BuildVetoDetector(); 
    
  auto containerSolid = new G4Box("FORMOSASolid",totLengthX/2.,totLengthY/2.,totLengthZ/2.);
  fFORMOSAAssembly = new G4LogicalVolume(containerSolid, fMaterials->Material("Air"), "FORMOSALogical");

  // center of the assembly is the center of middle PMT box
  G4ThreeVector FORMOSACenter(0.,0.,0.);
  G4RotationMatrix *noRot = new G4RotationMatrix();

  // placing the scintillator blocks: note that the center of each scintillator assembly
  // is in the middle of the scintillator, not in the geometrical baricenter
  for( int i=0; i<NScintillatorModules; i++ ){
    G4double offset = -0.5*(3.*fScintillatorBarSizeZ+4.*fPMTSizeSpacing)+i*(fScintillatorBarSizeZ+fPMTSizeSpacing);
    G4ThreeVector pos = FORMOSACenter + G4ThreeVector(0.,0., offset);
    fScintillatorAssembly->MakeImprint(fFORMOSAAssembly, pos, noRot, 0, false);
  }

  // placing the outer veto detectors
  //for( int i=-1; i<=1; i=i+2 ){
  // TODO
  //}

  // visibility
  G4VisAttributes* scinVis = new G4VisAttributes(G4Colour(34./255, 148./255, 83./255, 0.8));
  scinVis->SetVisibility(true);
  fScintillatorBar->SetVisAttributes(scinVis);  

  G4VisAttributes* pmtVis = new G4VisAttributes(G4Colour(128./255, 128./255, 128./255, 0.8));
  pmtVis->SetVisibility(true);
  fScintillatorPMT->SetVisAttributes(pmtVis);  

  //G4VisAttributes* vetoVis = new G4VisAttributes(G4Colour(234./255, 173./255, 26./255, 0.8));
  //vetoVis->SetVisibility(true);
  //fVetoDetector->SetVisAttributes(vetoVis);
  
}

FORMOSADetectorConstruction::~FORMOSADetectorConstruction()
{ 
  delete fFORMOSAAssembly;
  delete fScintillatorBar;
  delete fScintillatorAssembly;
}

void FORMOSADetectorConstruction::BuildScintillatorAssembly()
{
  fScintillatorAssembly = new G4AssemblyVolume();
  
  // build scintillator bar
  auto scintillatorBarSolid = new G4Box("scintillatorBarSolid", fScintillatorBarSizeX/2., fScintillatorBarSizeY/2., fScintillatorBarSizeZ/2.);
  fScintillatorBar = new G4LogicalVolume(scintillatorBarSolid, fMaterials->Material("Polystyrene"), "scintillatorBarLogical");

  // build scintillator block
  auto scinBlockSolid = new G4Box("scinBlockSolid",
                               (fNScinBarsX*fScintillatorBarSizeX)/2.,
                               (fNScinBarsY*fScintillatorBarSizeY)/2.,
                               fScintillatorBarSizeZ/2.);
  auto scinBlockLogical = new G4LogicalVolume(scinBlockSolid, fMaterials->Material("Polystyrene"), "scinBlockLogical");
  auto scinBlockYLayerSolid = new G4Box("scinBlockYLayerSolid",
                               (fNScinBarsX*fScintillatorBarSizeX)/2.,
                               fScintillatorBarSizeY/2.,
                               fScintillatorBarSizeZ/2.);
  auto scinBlockYLayerLogical = new G4LogicalVolume(scinBlockYLayerSolid, fMaterials->Material("Polystyrene"), "scinBlockYLayerLogical");
  // replicate along x to build one Y layer of the block
  new G4PVReplica("scinBlockYLayerPhysical", fScintillatorBar,
                  scinBlockYLayerLogical, kXAxis, fNScinBarsX, fScintillatorBarSizeX);
  // replicate along y to fill entire block
  new G4PVReplica("scinBlockPhysical", scinBlockYLayerLogical,
                  scinBlockLogical, kYAxis, fNScinBarsY, fScintillatorBarSizeY);
 
  // build PMT "box"
  auto PMTBoxSolid = new G4Box("PMTBoxSolid", 
                              (fNScinBarsX*fScintillatorBarSizeX)/2.,
                              (fNScinBarsY*fScintillatorBarSizeY)/2.,
                               fPMTSizeSpacing/2.);
  fScintillatorPMT = new G4LogicalVolume(PMTBoxSolid, fMaterials->Material("Air"), "PMTBoxLogical");

  //put the assembly together
  G4RotationMatrix *rot = new G4RotationMatrix();
  G4ThreeVector pos_block(0, 0, 0.);
  G4ThreeVector pos_pmtbox(0, 0, (fScintillatorBarSizeZ+fPMTSizeSpacing)/2.);
  fScintillatorAssembly->AddPlacedVolume(scinBlockLogical,pos_block,rot);
  fScintillatorAssembly->AddPlacedVolume(fScintillatorPMT,pos_pmtbox,rot);

}

void FORMOSADetectorConstruction::BuildVetoDetector()
{
 //TODO
}
