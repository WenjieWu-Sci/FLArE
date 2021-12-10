#ifndef LARBOXDETECTORCONSTRUCTION_HH
#define LARBOXDETECTORCONSTRUCTION_HH

#include <G4VUserDetectorConstruction.hh>
#include <G4String.hh>

//#include "DetectorConstructionMessenger.hh"

class G4LogicalVolume;
class FLArEDetectorConstructionMessenger;
class DetectorConstructionMaterial;

/**
 *
 * Obligatory class responsible for geometry - volumes, materials, fields, etc.
 *
 */

class LArBoxDetectorConstruction : public G4VUserDetectorConstruction {
  public:
    // Main method that has to be override in all detectors
    LArBoxDetectorConstruction();
    ~LArBoxDetectorConstruction();
    G4VPhysicalVolume* Construct() override;

    void ConstructSDandField() override;
    void SetDetMaterial(G4String detMaterial);
    void saveGDML(G4bool i) { m_saveGdml = i; };
    void UpdateGeometry();

  private:
    void DefineMaterial();

    G4LogicalVolume* worldLog;
    G4LogicalVolume* lArBoxLog;
    G4String fDetMaterialName;
    G4bool m_saveGdml;

    DetectorConstructionMaterial* LArBoxMaterials;
    FLArEDetectorConstructionMessenger* messenger;
};

#endif
