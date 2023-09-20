#include "geometry/GeometricalParameters.hh"
#include "G4SystemOfUnits.hh"

GeometricalParameters *GeometricalParameters::me = 0;

GeometricalParameters::GeometricalParameters()
{
  // **** DEFAULT VALUES ***
  
  // FASER2 magnet
  fSpectrometerMagnetOption = magnetOption::SAMURAI;
  fSpectrometerMagnetField = 1.0*tesla;
  fMagnetTotalSizeZ = 4*m;
  fTrackingStationTotalSizeZ = 3.12*m;
  fMagnetZPos = 40*m;
  // SAMURAI design
  fSpectrometerMagnetWindowX = 3.0*m;
  fSpectrometerMagnetWindowY = 1.0*m;
  fSpectrometerMagnetWindowZ = 4.0*m;
  fSpectrometerMagnetYokeThickX = 1.5*m;
  fSpectrometerMagnetYokeThickY = 2.0*m;
  // CrystalPulling design
  fSpectrometerMagnetLengthZ = 1.25*m;
  fSpectrometerMagnetInnerR = 0.8*m;
  fSpectrometerMagnetOuterR = 1.2*m;
  fNSpectrometerMagnets = 3;
  fSpectrometerMagnetGap = 0.5*m;
  // Tracking stations
  fNTrackingStations = 6;
  fNScintillatorBarsY = 3;
  fNScintillatorBarsX = 7;
  fScintillatorThickness = 1*cm;
  fTrackingStationGap = 0.5*m;

  // FASERnu2 emulsion detector
  fFASERnu2TotalSizeZ = 8.5*m;
}

GeometricalParameters* GeometricalParameters::Get()
{
  if (!me)
    me = new GeometricalParameters();
  return me; 
}

GeometricalParameters::magnetOption GeometricalParameters::ConvertStringToMagnetOption(G4String val)
{
  if (val == "SAMURAI" || val == "samurai")
    return magnetOption::SAMURAI;
  else if (val == "CrystalPulling" || val == "crystalpulling")     
    return magnetOption::CrystalPulling;
  else {
    G4cout << "ERROR: unknown FASER2 magnet geometry!" << G4endl;
    return magnetOption::unknown;
  }
}

G4ThreeVector GeometricalParameters::GetSpectrometerMagnetField()
{
  if (fSpectrometerMagnetOption == magnetOption::SAMURAI )
    return G4ThreeVector(0.,fSpectrometerMagnetField,0.); //field along Y
  else if (fSpectrometerMagnetOption == magnetOption::CrystalPulling)     
    return G4ThreeVector(0.,fSpectrometerMagnetField,0.); //field along Y (for now, FIXME?)
  else
    return G4ThreeVector(0.,0.,0.);
}

