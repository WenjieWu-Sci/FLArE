#ifndef FLAREDETECTORCONSTRUCTION_HH
#define FLAREDETECTORCONSTRUCTION_HH

#include "G4LogicalVolume.hh"
#include "G4AssemblyVolume.hh"

#include "DetectorConstructionMaterial.hh"

/// \class FLArEDetectorConstruction
/// This class builds the FLArE TPC

class FLArEDetectorConstruction {
  public:
    FLArEDetectorConstruction();
    ~FLArEDetectorConstruction();

    // Return assembly volume for placement
    G4AssemblyVolume* GetFLArEAssembly() { return fFLArETPCAssembly; }

    // Return logical volumes
    // TPC volume
    G4LogicalVolume* GetFLArETPCVolume() { return fFLArETPCLog; }
    // Hadronic calorimetry
    G4LogicalVolume* GetHadCalAbsorbVolume() { return HadAbsorLayersLogical; }
    G4LogicalVolume* GetHadCalXVolume()      { return HadCalXCellLogical; }
    G4LogicalVolume* GetHadCalYVolume()      { return HadCalYCellLogical; }
    // Muon catcher
    G4LogicalVolume* GetMuonCatcherAbsorbVolume() { return MuonFinderAbsorLayersLogical; }
    G4LogicalVolume* GetMuonCatcherXVolume()      { return MuonFinderXCellLogical; }
    G4LogicalVolume* GetMuonCatcherYVolume()      { return MuonFinderYCellLogical; }


    // Construction methods
    void BuildFLArETPC();
    void BuildFLArEHadCal();
    void BuildFLArEMuonCatcher();

  private:
    DetectorConstructionMaterial* fMaterials;
    G4Material* detectorMaterial;

    G4AssemblyVolume* fFLArETPCAssembly;

    G4LogicalVolume* fFLArETPCLog;
    G4LogicalVolume* lArBoxLog;
    G4LogicalVolume* cryoInsulationLog;

    G4LogicalVolume* HadAbsorLayersLogical;
    G4LogicalVolume* HadCalXCellLogical;
    G4LogicalVolume* HadCalYCellLogical;
    G4AssemblyVolume* HadCalAssembly;

    G4LogicalVolume* MuonFinderAbsorLayersLogical;
    G4LogicalVolume* MuonFinderXCellLogical;
    G4LogicalVolume* MuonFinderYCellLogical;
    G4AssemblyVolume* MuonCatcherAssembly;

    //G4String fDetMaterialName;
    G4String fDetGeomOption;
    G4double fLArSizeX;
    G4double fLArSizeY;
    G4double fLArSizeZ;
    G4double fThicknessInsulation;
    G4double fHadCalLength;
    G4double fMuonCatcherLength;
};

#endif
