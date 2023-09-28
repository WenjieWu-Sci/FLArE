#ifndef FORMOSADetectorConstruction_hh
#define FORMOSADetectorConstruction_hh

#include "G4LogicalVolume.hh"
#include "G4AssemblyVolume.hh"

#include "DetectorConstructionMaterial.hh"

/// \class FORMOSADetectorConstruction
/// This class builds the FORMOSA detector, according to the preliminary
/// size described in the P5 summary paper and PFP 6th Workshop slides

class FORMOSADetectorConstruction {
  public:
    FORMOSADetectorConstruction();
    ~FORMOSADetectorConstruction();

    // Returns assembly volume for placement    
    G4AssemblyVolume* GetFORMOSAAssembly(){ return fFORMOSAAssembly;}
  
    // Returns logical volumes
    G4LogicalVolume* GetScintillatorBar(){ return fScintillatorBar;}

    // Construction methods
    void BuildScintillatorAssembly();
    void BuildVetoDetector();

  private:

    G4AssemblyVolume* fFORMOSAAssembly;
    DetectorConstructionMaterial* fMaterials;
    G4LogicalVolume* fScintillatorBar;
    G4AssemblyVolume* fScintillatorAssembly;
    G4LogicalVolume* fScintillatorPMT;

    // Scintillator bars
    G4int fNScintillatorBarsX;
    G4int fNScintillatorBarsY;
    G4double fScintillatorBarSizeX;
    G4double fScintillatorBarSizeY;
    G4double fScintillatorBarSizeZ;

    // spacing
    G4double fPMTSizeSpacing;
};

#endif
