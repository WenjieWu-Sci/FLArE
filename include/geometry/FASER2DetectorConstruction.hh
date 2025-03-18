#ifndef FASER2DetectorConstruction_hh
#define FASER2DetectorConstruction_hh

#include "G4LogicalVolume.hh"
#include "G4AssemblyVolume.hh"
#include <vector>

#include "DetectorConstructionMaterial.hh"

/// \class FASER2DetectorConstruction
/// This class builds the FASER2 spectrometer, using either the SAMURAI design
/// or multiple crystal-pulling magnets from Toshiba.  

class FASER2DetectorConstruction {
  public:
    FASER2DetectorConstruction();
    ~FASER2DetectorConstruction();

    // Returns assembly volume for placement    
    G4LogicalVolume* GetFASER2Assembly(){ return fFASER2Assembly;}
  
    // Returns logical volumes
    G4LogicalVolume* GetMagneticVolume(){ return fMagnetWindow;}
    G4LogicalVolume* GetTrackingStation(){ return fTrackingLogical;}
    std::vector<G4LogicalVolume*> GetTrackingStations(){ return fTrackingStationsLogical;}

    // Construction methods
    void BuildSAMURAIDesign();
    void BuildCrystalPullingDesign();
    void BuildTrackingStation();

  private:

    G4LogicalVolume* fFASER2Assembly;
    DetectorConstructionMaterial* fMaterials;
    G4LogicalVolume* fMagnetYoke;
    G4LogicalVolume* fMagnetWindow;
    G4LogicalVolume* fTrackingLogical;
    std::vector<G4LogicalVolume*> fTrackingStationsLogical;
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
