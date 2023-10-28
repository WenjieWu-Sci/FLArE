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
    void SaveGDML(G4bool i) { m_saveGdml = i; }
    void AddFLArE(G4bool i) { m_addFLArE = i; }
    void SetFieldValue(G4double val) { fFieldValue = val; }
    void AddFORMOSA(G4bool i) { m_addFORMOSA = i; }
    void AddFASERnu2(G4bool i) { m_addFASERnu2 = i; }
    void AddFASER2(G4bool i) { m_addFASER2 = i; }
    void AddNDLAr(G4bool i) { m_addNDLAr = i; }
    void UpdateGeometry();

  private:
    void DefineMaterial();

    G4LogicalVolume* worldLog;
    G4VPhysicalVolume* worldPhys;
    G4bool m_saveGdml;

    G4bool m_addFLArE;
    G4LogicalVolume* TPCModuleLogical;
    G4LogicalVolume* hadCatcherLogical;
    G4LogicalVolume* HadCalXCellLogical;
    G4LogicalVolume* HadCalYCellLogical;
    G4LogicalVolume* muonFinderLogical;
    G4LogicalVolume* MuonFinderXCellLogical;
    G4LogicalVolume* MuonFinderYCellLogical;
    G4LogicalVolume* HadAbsorLayersLogical;
    G4LogicalVolume* MuonFinderAbsorLayersLogical;
    G4double fFieldValue;

    G4bool m_addFASERnu2;

    G4bool m_addFASER2;
    G4LogicalVolume* FASER2MagnetLogical;
    G4LogicalVolume* TrackingVerScinBarLogical;
    G4LogicalVolume* TrackingHorScinBarLogical; 

    G4LogicalVolume* FASERnu2EmulsionLogical;
    G4LogicalVolume* FASERnu2VetoInterfaceLogical;

    G4bool m_addFORMOSA;
    G4LogicalVolume *FORMOSAScintillatorBarLogical;

    G4bool m_addNDLAr;

    DetectorConstructionMaterial* LArBoxMaterials;
    DetectorConstructionMessenger* messenger;

    static G4ThreadLocal G4UniformMagField* magField;
    static G4ThreadLocal G4FieldManager* fieldMgr;
    static G4ThreadLocal G4UniformMagField* magFieldFASER2;
    static G4ThreadLocal G4FieldManager* fieldMgrFASER2;
};

#endif
