#ifndef BabyMINDMagneticField_H
#define BabyMINDMagneticField_H

#include "globals.hh"
#include "G4MagneticField.hh"

/// Magnetic field for BabyMIND
/// Simple implementation using uniform magnetic fields.
/// Full magnetic field map should be added in the future.

class BabyMINDMagneticField : public G4MagneticField
{
  public:
    BabyMINDMagneticField();
    ~BabyMINDMagneticField() override;

    void GetFieldValue(const G4double point[4],double* bField ) const override;

  private:
    
    G4double fBx; //  field strength
    G4double fUpperSlitY; // upper slit height in global y
    G4double fLowerSlitY; // lower slit height in global y
};


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
