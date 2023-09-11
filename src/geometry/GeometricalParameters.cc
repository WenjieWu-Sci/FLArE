#include "geometry/GeometricalParameters.hh"
#include "G4SystemOfUnits.hh"

GeometricalParameters *GeometricalParameters::me = 0;

GeometricalParameters::GeometricalParameters()
{
  // default values
  fSpectrometerMagnetOption = "SAMURAI";
  fSpectrometerMagnetField = 1.0*tesla;
  fSpectrometerMagnetWindowX = 3.0*m;
  fSpectrometerMagnetWindowY = 1.0*m;
  fSpectrometerMagnetWindowZ = 4.0*m;
  fSpectrometerMagnetYokeThickX = 1.5*m;
  fSpectrometerMagnetYokeThickY = 2.0*m;

}


GeometricalParameters* GeometricalParameters::Get()
{
  if (!me)
    me = new GeometricalParameters();
  return me; 
}



