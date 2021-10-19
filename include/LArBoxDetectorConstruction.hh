#ifndef LARBOXDETECTORCONSTRUCTION_HH
#define LARBOXDETECTORCONSTRUCTION_HH

#include <G4VUserDetectorConstruction.hh>

class G4LogicalVolume;

/**
 *
 * Obligatory class responsible for geometry - volumes, materials, fields, etc.
 *
 */

class LArBoxDetectorConstruction : public G4VUserDetectorConstruction {
  public:
    // Main method that has to be override in all detectors
    G4VPhysicalVolume* Construct() override;
    void ConstructSDandField() override;

  private:
    G4LogicalVolume* worldLog;
    G4LogicalVolume* lArBoxLog;
};

#endif
