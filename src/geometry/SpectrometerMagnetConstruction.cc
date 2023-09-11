#include "geometry/SpectrometerMagnetConstruction.hh"
#include "geometry/GeometricalParameters.hh"
#include "DetectorConstructionMaterial.hh"

#include "G4AssemblyVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4SubtractionSolid.hh"
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

SpectrometerMagnetConstruction::SpectrometerMagnetConstruction()
{
  // load materials
  fMaterials = DetectorConstructionMaterial::GetInstance();

  // choose magnet option
  G4String opt = GeometricalParameters::Get()->GetSpectrometerMagnetOption(); 
  if( opt == "SAMURAI" || opt == "samurai" ){
    
    G4cout << "Building SAMURAI spectrometer magnet" << G4endl;
    fMagnetWindowX = GeometricalParameters::Get()->GetSpectrometerMagnetWindowX();
    fMagnetWindowY = GeometricalParameters::Get()->GetSpectrometerMagnetWindowY();
    fMagnetWindowZ = GeometricalParameters::Get()->GetSpectrometerMagnetWindowZ();
    fMagnetYokeThicknessX = GeometricalParameters::Get()->GetSpectrometerMagnetYokeThickX();
    fMagnetYokeThicknessY = GeometricalParameters::Get()->GetSpectrometerMagnetYokeThickY();

    BuildSAMURAIDesign(); //sets logical volumes
    
    fMagnetAssembly = new G4AssemblyVolume();
    G4RotationMatrix *noRot = new G4RotationMatrix();

    // center of the assembly is the center of the magnet window
    G4ThreeVector magCenter(0.,0.,0.);

    fMagnetAssembly->AddPlacedVolume(fMagnetWindow,magCenter,noRot);
    fMagnetAssembly->AddPlacedVolume(fMagnetYoke,magCenter,noRot);

  } else if ( opt == "CrystalPulling" || opt == "crystalpulling" ){
 
    G4cout << "Building CrystalPulling spectrometer magnet" << G4endl;
    BuildCrystalPullingDesign(); //sets logical volumes

    // to do ....

  } else {
    G4cout << "ERROR: unknown FASER2 spectrometer magnet option!" << G4endl;  
  }

  // visibility
  G4VisAttributes* nullVis = new G4VisAttributes(G4Colour(167./255, 168./255, 189./255));
  nullVis->SetVisibility(false);
  fMagnetWindow->SetVisAttributes(nullVis);

  G4VisAttributes* magnetVis = new G4VisAttributes(G4Colour(234./255, 173./255, 26./255, 0.8));
  magnetVis->SetVisibility(true);
  fMagnetYoke->SetVisAttributes(magnetVis);
}

SpectrometerMagnetConstruction::~SpectrometerMagnetConstruction()
{ 
  delete fMagnetAssembly;
  delete fMagnetWindow;
  delete fMagnetYoke;
}

void SpectrometerMagnetConstruction::BuildSAMURAIDesign()
{
  auto magnetYokeBlock = new G4Box("MagnetYokeBlock", fMagnetWindowX/2.+fMagnetYokeThicknessX, fMagnetWindowY/2.+fMagnetYokeThicknessY, fMagnetWindowZ/2.);
  auto magnetWindowSolid = new G4Box("MagnetYokeWindow", fMagnetWindowX/2., fMagnetWindowY/2., fMagnetWindowZ/2.);
  auto magnetYokeSolid = new G4SubtractionSolid("MagnetYoke",
				magnetYokeBlock, // block - window = hollow block
				magnetWindowSolid, 
				0, // no rotation
				G4ThreeVector(0,0,0)); //no translation
  
  fMagnetYoke = new G4LogicalVolume(magnetYokeSolid, fMaterials->Material("Iron"), "FASER2MagnetYokeLogical");
  fMagnetWindow = new G4LogicalVolume(magnetWindowSolid, fMaterials->Material("Air"), "FASER2MagnetWindowLogical"); 
}

void SpectrometerMagnetConstruction::BuildCrystalPullingDesign()
{
  //TODO
}
