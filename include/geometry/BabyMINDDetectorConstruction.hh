#ifndef BabyMINDDetectorConstruction_hh
#define BabyMINDDetectorConstruction_hh

#include "G4LogicalVolume.hh"
#include "G4AssemblyVolume.hh"

#include "DetectorConstructionMaterial.hh"

/// \class BabyMINDDetectorConstruction
/// This class builds the BabyMIND detector, according to the available
/// public information and lenghts and size from the engineering model

class BabyMINDDetectorConstruction {
  public:
    BabyMINDDetectorConstruction();
    ~BabyMINDDetectorConstruction();

    // Returns assembly volume for placement    
    G4AssemblyVolume* GetBabyMINDAssembly(){ return fBabyMINDAssembly;}
  
    // Returns logical volumes
    G4LogicalVolume* GetMagnetPlate(){ return fMagnetPlate;}
    G4LogicalVolume* GetHorizontalBar(){ return fHorizontalBar;}
    G4LogicalVolume* GetVerticalBar(){ return fVerticalBar;}

    // Construction methods
    void BuildMagnetModule();
    void BuildScintillatorModule();

    // Supporting methods
    G4double ComputeTotalSize();

  private:

    G4AssemblyVolume* fBabyMINDAssembly;
    DetectorConstructionMaterial* fMaterials;
    G4LogicalVolume* fMagnetPlate;
    G4LogicalVolume* fHorizontalBar;
    G4LogicalVolume* fVerticalBar;
    G4LogicalVolume* fFirstVerLayerLogical;
    G4LogicalVolume* fFirstHorLayerLogical;
    G4LogicalVolume* fSecondVerLayerLogical;
    G4LogicalVolume* fSecondHorLayerLogical;

    G4AssemblyVolume* fDetectorModule;

    // Spacing
    G4double fMagnetToScinSpacing;
    G4double fMagnetToMagnetSpacing;
    G4double fBlockToBlockSpacing;
    G4double fBlockPadding;
    G4String fBlockSequence;

    // MagnetPlate
    G4double fMagnetPlateThickness;
    G4double fMagnetPlateSizeX;
    G4double fMagnetPlateSizeY;
    G4double fMagnetCentralPlateY;
    G4double fSlitSizeX;
    G4double fSlitSizeY;

    // DetectorModule
    G4int fNVerticalBars;
    G4int fNHorizontalBars;
    G4double fBarThickness;
    G4double fVerticalBarSizeX;
    G4double fVerticalBarSizeY;
    G4double fHorizontalBarSizeX;
    G4double fHorizontalBarSizeY;
};

#endif
