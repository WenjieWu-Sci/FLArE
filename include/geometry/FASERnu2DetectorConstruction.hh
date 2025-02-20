#ifndef FASERnu2DetectorConstruction_hh
#define FASERnu2DetectorConstruction_hh

#include "G4LogicalVolume.hh"
#include "G4AssemblyVolume.hh"

#include "DetectorConstructionMaterial.hh"

/// \class FASERnu2DetectorConstruction
/// This class builds the FASERnu2 emulsion detector, according to the preliminary
/// size described in the P5 summary paper and PFP 6th Workshop slides

class FASERnu2DetectorConstruction {
  public:
    FASERnu2DetectorConstruction();
    ~FASERnu2DetectorConstruction();

    // Returns assembly volume for placement    
    G4LogicalVolume* GetFASERnu2Assembly(){ return fFASERnu2Assembly;}
  
    // Returns logical volumes
    G4LogicalVolume* GetEmulsionFilm(){ return fEmulsionFilm;}
    G4LogicalVolume* GetVetoInterfaceDetector(){ return fInterfaceDetector;}

    // Construction methods
    void BuildEmulsionTungstenModule();
    void BuildVetoInterfaceDetector();

  private:

    G4LogicalVolume* fFASERnu2Assembly;
    DetectorConstructionMaterial* fMaterials;
    G4LogicalVolume* fEmulsionFilm;
    G4LogicalVolume* fTungstenPlate;
    G4LogicalVolume* fInterfaceDetector;
    G4AssemblyVolume* fEmulsionTungstenModule;

    // Emulsion/Tungsten
    G4int fNEmulsionTungstenLayers;
    G4double fTungstenThickness;
    G4double fEmulsionThickness;
    G4double fEmulsionTungstenSizeX;
    G4double fEmulsionTungstenSizeY;
    G4double fModuleThickness;

    // Veto/Interface detectors
    G4double fVetoInterfaceSizeZ;
    G4double fVetoInterfaceSizeX;
    G4double fVetoInterfaceSizeY;
};

#endif
