#ifndef SpectrometerMagnetConstruction_hh
#define SpectrometerMagnetConstruction_hh

#include "G4LogicalVolume.hh"
#include "G4AssemblyVolume.hh"

#include "DetectorConstructionMaterial.hh"

/// \class SpectrometerMagnetConstruction
/// This class builds the FASER2 spectrometer magnet, using either the SAMURAI design
/// or multiple crystal-pulling magnets from Toshiba.  

class SpectrometerMagnetConstruction {
  public:
    SpectrometerMagnetConstruction();
    ~SpectrometerMagnetConstruction();

    // Returns assembly volume for placement    
    G4AssemblyVolume* GetSpectrometerMagnetAssembly(){ return fMagnetAssembly;}
  
    // Returns logical volumes
    G4LogicalVolume* GetMagneticVolume(){ return fMagnetWindow;}
    G4LogicalVolume* GetHorTrackingScinBar(){ return fHorTrackingScinBar;}
    G4LogicalVolume* GetVerTrackingScinBar(){ return fVerTrackingScinBar;}

    // Construction methods
    void BuildSAMURAIDesign();
    void BuildCrystalPullingDesign();
    void BuildTrackingStation();

  private:

    G4AssemblyVolume* fMagnetAssembly;
    DetectorConstructionMaterial* fMaterials;
    G4LogicalVolume* fMagnetYoke;
    G4LogicalVolume* fMagnetWindow;
    G4AssemblyVolume* fTrackingStation;
    G4LogicalVolume* fHorTrackingScinBar;   
    G4LogicalVolume* fVerTrackingScinBar;   

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

    // Tracking stations
    G4int fNTrackingStations;
    G4double fTrackingStationX;
    G4double fTrackingStationY;
    G4double fScinThickness;
    G4int fNScinBarsY;
    G4int fNScinBarsX;
    G4double fTrackingStationGap;
};

#endif
