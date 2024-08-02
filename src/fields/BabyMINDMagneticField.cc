#include "fields/BabyMINDMagneticField.hh"
#include "geometry/GeometricalParameters.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BabyMINDMagneticField::BabyMINDMagneticField()
{
  // Get magnetic field parameters from usual storage
  // Do it here to avoid too many calls in GetFieldValue() !!
  fBx = GeometricalParameters::Get()->GetBabyMINDFieldStrength();
  
  G4double centralPlateSizeY = GeometricalParameters::Get()->GetBabyMINDMagnetCentralPlateY();
  G4double slitSizeY = GeometricalParameters::Get()->GetBabyMINDSlitSizeY();
  G4double slitPosY = centralPlateSizeY/2. + slitSizeY/2.;
  
  fUpperSlitY = slitPosY; 
  fLowerSlitY = -slitPosY;
}

BabyMINDMagneticField::~BabyMINDMagneticField() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// This functions describes the magnetic field:
// point[4] is the (x,y,z,t) position in GLOBAL coordinates
// bField[3] are the components of the B field in GLOBAL coordinates
// Simple implementation: three "bands" of uniform magnetic field along x
// no attempt is made at modelling the return field in the sides.
void BabyMINDMagneticField::GetFieldValue(const G4double point[4], double *bField) const
{
  // uppoer/lower plate: field is in positive x
  bField[0] = fBx;
  bField[1] = 0.;
  bField[2] = 0.;

  // central plate, between the two slits, field is negative x
  if( (point[1] > fLowerSlitY) && (point[1] < fUpperSlitY) ){
    bField[0] = -fBx;
    bField[1] = 0.;
    bField[2] = 0.;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
