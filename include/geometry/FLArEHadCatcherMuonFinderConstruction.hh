#ifndef FLAREHADCATCHERMUONFINDERCONSTRUCTION_HH
#define FLAREHADCATCHERMUONFINDERCONSTRUCTION_HH

#include "G4LogicalVolume.hh"
#include "G4AssemblyVolume.hh"

#include "DetectorConstructionMaterial.hh"
#include "GeometricalParameters.hh"

/// \class FLArEHadCatcherMuonFinderConstruction
/// This class builds the FLArE Hadron Catcher and Muon Finder

class FLArEHadCatcherMuonFinderConstruction {
  public:
    FLArEHadCatcherMuonFinderConstruction();
    ~FLArEHadCatcherMuonFinderConstruction();

    // Return assembly volume for placement
    G4AssemblyVolume* GetHadCatcherMuonFinderAssembly() { return fHadCatcherMuonFinderAssembly; }

    // Return logical volumes
    // Hadronic calorimetry
    G4LogicalVolume* GetHadCalAbsorbVolume() { return HadAbsorLayersLogical; }
    G4LogicalVolume* GetHadCalXVolume()      { return HadCalXCellLogical; }
    G4LogicalVolume* GetHadCalYVolume()      { return HadCalYCellLogical; }
    // Muon catcher
    G4LogicalVolume* GetMuonCatcherAbsorbVolume() { return MuonFinderAbsorLayersLogical; }
    G4LogicalVolume* GetMuonCatcherXVolume()      { return MuonFinderXCellLogical; }
    G4LogicalVolume* GetMuonCatcherYVolume()      { return MuonFinderYCellLogical; }

    // Construction methods
    void BuildFLArEHadCal();
    void BuildFLArEMuonCatcher();

  private:
    DetectorConstructionMaterial* fMaterials;
  
    G4AssemblyVolume* fHadCatcherMuonFinderAssembly;

    G4LogicalVolume* HadAbsorLayersLogical;
    G4LogicalVolume* HadCalXCellLogical;
    G4LogicalVolume* HadCalYCellLogical;
    G4AssemblyVolume* HadCalAssembly;

    G4LogicalVolume* MuonFinderAbsorLayersLogical;
    G4LogicalVolume* MuonFinderXCellLogical;
    G4LogicalVolume* MuonFinderYCellLogical;
    G4AssemblyVolume* MuonCatcherAssembly;

    G4double fLArSizeX;
    G4double fLArSizeY;
    G4double fHadCalLength;
    G4double fMuonCatcherLength;
};

#endif
