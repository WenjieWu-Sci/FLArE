#ifndef SpectrometerMagnetConstruction_hh
#define SpectrometerMagnetConstruction_hh

#include "G4LogicalVolume.hh"
#include "G4AssemblyVolume.hh"
#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"

#include "DetectorConstructionMaterial.hh"

/// \class SpectrometerMagnetDetectorConstruction
/// This class builds the FASER2 spectrometer magnet, using either the SAMURAI design
/// or multiple crystal-pulling magnets from Toshiba.  

class SpectrometerMagnetConstructionMessenger;

class SpectrometerMagnetConstruction {
  public:
    SpectrometerMagnetConstruction();
    ~SpectrometerMagnetConstruction();

    // Returns assembly volume for placement    
    G4AssemblyVolume* GetSpectrometerMagnetAssembly(){ return fMagnetAssembly;}
  
    // Returns magnetic logical volume
    G4LogicalVolume* GetMagneticVolume(){ return fMagnetWindow;}

    // Construction methods
    void BuildSAMURAIDesign();
    void BuildCrystalPullingDesign();

  private:

    G4AssemblyVolume* fMagnetAssembly;
    DetectorConstructionMaterial* fMaterials;
    G4LogicalVolume* fMagnetYoke;
    G4LogicalVolume* fMagnetWindow;
    
    // SAMURAI design
    G4double fMagnetWindowX;
    G4double fMagnetWindowY;
    G4double fMagnetWindowZ;
    G4double fMagnetYokeThicknessX;
    G4double fMagnetYokeThicknessY;

    // Crystal-pulling design
    G4double fMagnetInnerR;
    G4double fMagnetOuterR;
    G4double fMagnetLengthZ;
    G4int fNMagnets;
    G4double fMagnetGap;
};

#endif
