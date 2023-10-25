#ifndef DETECTORCONSTRUCTION_HH
#define DETECTORCONSTRUCTION_HH

#include <G4VUserDetectorConstruction.hh>
#include <G4String.hh>
#include <G4UniformMagField.hh>
#include <G4FieldManager.hh>
#include <G4AssemblyVolume.hh>

class G4LogicalVolume;
class DetectorConstructionMessenger;
class DetectorConstructionMaterial;

/**
 *
 * Obligatory class responsible for geometry - volumes, materials, fields, etc.
 *
 */

class DetectorConstruction : public G4VUserDetectorConstruction {
  public:
    // Main method that has to be override in all detectors
    DetectorConstruction();
    ~DetectorConstruction();
    G4VPhysicalVolume* Construct() override;

    void ConstructSDandField() override;
    void saveGDML(G4bool i) { m_saveGdml = i; };
    void SetFieldValue(G4double val) { fFieldValue = val; }
    void UpdateGeometry();

  private:
    void DefineMaterial();

    G4LogicalVolume* worldLog;
    //G4LogicalVolume* lArBoxLog;
    //G4LogicalVolume* cryoInsulationLog;
    G4LogicalVolume* hadCatcherLogical;
    G4LogicalVolume* HadCalXCellLogical;
    G4LogicalVolume* HadCalYCellLogical;
    G4LogicalVolume* muonFinderLogical;
    G4LogicalVolume* MuonFinderXCellLogical;
    G4LogicalVolume* MuonFinderYCellLogical;
    G4LogicalVolume* HadAbsorLayersLogical;
    G4LogicalVolume* MuonFinderAbsorLayersLogical;
    G4LogicalVolume* TPCModuleLogical;
    G4double fFieldValue;
    G4bool m_saveGdml;

    G4LogicalVolume* FASER2MagnetLogical;
    G4LogicalVolume* TrackingVerScinBarLogical;
    G4LogicalVolume* TrackingHorScinBarLogical; 

    G4LogicalVolume* FASERnu2EmulsionLogical;
    G4LogicalVolume* FASERnu2VetoInterfaceLogical;

    G4LogicalVolume *FORMOSAScintillatorBarLogical;

    DetectorConstructionMaterial* LArBoxMaterials;
    DetectorConstructionMessenger* messenger;

    static G4ThreadLocal G4UniformMagField* magField;
    static G4ThreadLocal G4FieldManager* fieldMgr;
    static G4ThreadLocal G4UniformMagField* magFieldFASER2;
    static G4ThreadLocal G4FieldManager* fieldMgrFASER2;
};

#endif
