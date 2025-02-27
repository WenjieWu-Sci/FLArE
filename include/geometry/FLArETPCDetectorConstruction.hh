#ifndef FLARETPCDETECTORCONSTRUCTION_HH
#define FLARETPCDETECTORCONSTRUCTION_HH

#include "G4LogicalVolume.hh"
#include "G4AssemblyVolume.hh"

#include "DetectorConstructionMaterial.hh"
#include "GeometricalParameters.hh"

/// \class FLArETPCDetectorConstruction
/// This class builds the FLArE TPC

class FLArETPCDetectorConstruction {
  public:
    FLArETPCDetectorConstruction();
    ~FLArETPCDetectorConstruction();

    // Return assembly volume for placement
    G4LogicalVolume* GetFLArETPCAssembly() { return fFLArETPCAssembly; }

    // Return logical volumes
    // TPC volume
    G4LogicalVolume* GetFLArETPCVolume() { return fFLArETPCLog; }

    // Construction methods
    void BuildFLArETPC();
    void BuildCryostatInsulation();

  private:
    DetectorConstructionMaterial* fMaterials;
    G4Material* detectorMaterial;

    G4LogicalVolume* fFLArETPCAssembly;

    G4LogicalVolume* fFLArETPCLog;
    G4LogicalVolume* lArBoxLog;
    G4LogicalVolume* cryoInsulationLog;

    GeometricalParameters::tpcConfigOption fDetGeomOption;
    G4double fLArSizeX;
    G4double fLArSizeY;
    G4double fLArSizeZ;
    G4double fThicknessInsulation;
};

#endif
