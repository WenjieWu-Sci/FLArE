#ifndef SAMPLINGCALORIMETERCONSTRUCTION_HH
#define SAMPLINGCALORIMETERCONSTRUCTION_HH

#include "G4LogicalVolume.hh"
#include "G4AssemblyVolume.hh"

#include "DetectorConstructionMaterial.hh"

class SamplingCalorimeterConstruction {
  public:
    SamplingCalorimeterConstruction();
    ~SamplingCalorimeterConstruction();

    // Return assembly volume for placement
    G4AssemblyVolume* GetSamplingCalorimeterAssembly() { return fSamplingCalorimeterAssembly; }

    // Return logical volumes
    G4LogicalVolume* GetSamplingCalorimeterAbsorbVolume() { return AbsorbLayerLogical; }
    G4LogicalVolume* GetSamplingCalorimeterCaloXVolume()  { return CaloXCellLogical; }
    G4LogicalVolume* GetSamplingCalorimeterCaloYVolume()  { return CaloYCellLogical; }
    G4LogicalVolume* GetSamplingCalorimeterFiberXVolume() { return FiberXCellLogical; }
    G4LogicalVolume* GetSamplingCalorimeterFiberYVolume() { return FiberYCellLogical; }

    // Construction methods
    void BuildScintBarDetector();

  private:
    DetectorConstructionMaterial* fMaterials;

    G4AssemblyVolume* fSamplingCalorimeterAssembly;

    G4LogicalVolume* AbsorbLayerLogical;
    G4LogicalVolume* AbsorbSupportLayerLogical;
    G4LogicalVolume* CaloXCellLogical;
    G4LogicalVolume* CaloYCellLogical;
    G4LogicalVolume* FiberXCellLogical;
    G4LogicalVolume* FiberYCellLogical;
    G4AssemblyVolume* SamplingCaloAssembly;

    G4double thicknessAbsorber;
    G4double thicknessCaloX;
    G4double thicknessCaloY;
    G4double thicknessFiberX;
    G4double thicknessFiberY;
    G4double widthCaloX;
    G4double widthCaloY;
    G4double widthFiberX;
    G4double widthFiberY;
    G4double fSamplingCaloSizeX;
    G4double fSamplingCaloSizeY;
    G4double fSamplingCaloSizeZ;
    G4int    fSamplingCaloNLayers;
};

#endif
