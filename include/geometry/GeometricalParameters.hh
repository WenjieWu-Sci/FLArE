#ifndef GeometricalParameters_hh
#define GeometricalParameters_hh

#include "G4String.hh"

/** This class stores geometrical parameters for quick access
 *  It can be called form any other class to retrieve 
 *  configuration options or parameters whenever needed
**/

class GeometricalParameters  {
  private:
    GeometricalParameters();
  public:
    static GeometricalParameters* Get();
    virtual ~GeometricalParameters() {}

    // set & get methods
    void SetSpectrometerMagnetOption(G4String val) { fSpectrometerMagnetOption = val; }
    G4String GetSpectrometerMagnetOption() { return fSpectrometerMagnetOption; }
    void SetSpectrometerMagnetField(G4double val) { fSpectrometerMagnetField = val; }
    G4double GetSpectrometerMagnetField() { return fSpectrometerMagnetField; }
    void SetSpectrometerMagnetWindowX(G4double val) { fSpectrometerMagnetWindowX = val; }
    G4double GetSpectrometerMagnetWindowX() { return fSpectrometerMagnetWindowX; }
    void SetSpectrometerMagnetWindowY(G4double val) { fSpectrometerMagnetWindowY = val; }
    G4double GetSpectrometerMagnetWindowY() { return fSpectrometerMagnetWindowY; }
    void SetSpectrometerMagnetWindowZ(G4double val) { fSpectrometerMagnetWindowZ = val; }
    G4double GetSpectrometerMagnetWindowZ() { return fSpectrometerMagnetWindowZ; }
    void SetSpectrometerMagnetYokeThickX(G4double val) { fSpectrometerMagnetYokeThickX = val; }
    G4double GetSpectrometerMagnetYokeThickX() { return fSpectrometerMagnetYokeThickX; }
    void SetSpectrometerMagnetYokeThickY(G4double val) { fSpectrometerMagnetYokeThickY = val; }
    G4double GetSpectrometerMagnetYokeThickY() { return fSpectrometerMagnetYokeThickY; }

  private:
    //the singleton
    static GeometricalParameters *me;
    
    G4String fSpectrometerMagnetOption;
    G4double fSpectrometerMagnetField;
    G4double fSpectrometerMagnetWindowX;
    G4double fSpectrometerMagnetWindowY;
    G4double fSpectrometerMagnetWindowZ;
    G4double fSpectrometerMagnetYokeThickX;
    G4double fSpectrometerMagnetYokeThickY;

};

#endif 
