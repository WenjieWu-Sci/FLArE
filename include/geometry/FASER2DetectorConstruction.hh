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
    G4LogicalVolume* GetVetoStations(){ return fVetoScinLogical;}
    G4LogicalVolume* GetMuonStations(){ return fMuonDetLogical;}
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
    G4LogicalVolume* fVetoScinLogical;
    G4LogicalVolume* fVetoShieldLogical;
    G4LogicalVolume* fDecayVolumeLogical;
    std::vector<G4LogicalVolume*> fTrackingStationsLogical;
    G4AssemblyVolume* fTrackingStation;
    G4LogicalVolume* fEMCalLogical;
    G4LogicalVolume* fHadCalLogical;
    G4LogicalVolume* fIronWallLogical;
    G4LogicalVolume* fMuonDetLogical;

    // Veto
    G4double fVetoLengthX;
    G4double fVetoLengthY;
    G4double fVetoShieldThickness;

    // Decay volume
    G4double fDecayVolumeLength;

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
    G4double fTrackingStationGap;
    G4double fUpstreamTrackingStationGap;
    G4double fDownstreamTrackingStationGap;
    G4int fNUpstreamTrackers;
    G4int fNDownstreamTrackers;

    // Calorimeter guess??
    G4double fEMCaloThickness;
    G4double fHadCaloThickness;
    G4double fIronWallThickness;
};

#endif
