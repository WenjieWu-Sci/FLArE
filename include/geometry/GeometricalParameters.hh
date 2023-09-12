#ifndef GeometricalParameters_hh
#define GeometricalParameters_hh

#include "G4String.hh"
#include "G4ThreeVector.hh"

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

    // FASER2 Spectrometer Magnet
    enum magnetOption{ SAMURAI, CrystalPulling, unknown};
    magnetOption ConvertStringToMagnetOption(G4String val);
    void SetSpectrometerMagnetOption(magnetOption val) { fSpectrometerMagnetOption = val; }
    magnetOption GetSpectrometerMagnetOption() { return fSpectrometerMagnetOption; }
    void SetSpectrometerMagnetField(G4double val) { fSpectrometerMagnetField = val; }
    G4ThreeVector GetSpectrometerMagnetField();
    void SetSpectrometerMagnetTotalSizeZ(G4double val) { fSpectrometerMagnetTotalSizeZ = val; }
    G4double GetSpectrometerMagnetTotalSizeZ() { return fSpectrometerMagnetTotalSizeZ; }
    // SAMURAI design
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
    // Crystal-Pulling design
    void SetSpectrometerMagnetInnerR(G4double val) { fSpectrometerMagnetInnerR = val; }
    G4double GetSpectrometerMagnetInnerR() { return fSpectrometerMagnetInnerR; }
    void SetSpectrometerMagnetOuterR(G4double val) { fSpectrometerMagnetOuterR = val; }
    G4double GetSpectrometerMagnetOuterR() { return fSpectrometerMagnetOuterR; }
    void SetSpectrometerMagnetLengthZ(G4double val) { fSpectrometerMagnetLengthZ = val; }
    G4double GetSpectrometerMagnetLengthZ() { return fSpectrometerMagnetLengthZ; }
    void SetNSpectrometerMagnets(G4int val) { fNSpectrometerMagnets = val; }
    G4int GetNSpectrometerMagnets() { return fNSpectrometerMagnets; }
    void SetSpectrometerMagnetGap(G4double val) { fSpectrometerMagnetGap = val; }
    G4double GetSpectrometerMagnetGap() { return fSpectrometerMagnetGap; }  
    // Tracking station
    void SetNTrackingStations(G4int val) { fNTrackingStations = val; }
    G4int GetNTrackingStations() { return fNTrackingStations; }  
    void SetNScintillatorBarsY(G4int val) { fNScintillatorBarsY = val; }
    G4int GetNScintillatorBarsY() { return fNScintillatorBarsY; }  
    void SetNScintillatorBarsX(G4int val) { fNScintillatorBarsX = val; }
    G4int GetNScintillatorBarsX() { return fNScintillatorBarsX; }  
    void SetScintillatorThickness(G4double val) { fScintillatorThickness = val; }
    G4double GetScintillatorThickness() { return fScintillatorThickness; }  
    void SetTrackingStationGap(G4double val) { fTrackingStationGap = val; }
    G4double GetTrackingStationGap() { return fTrackingStationGap; }  

  private:
    //the singleton
    static GeometricalParameters *me;
    
    // FASER2 Spectrometer Magnet
    magnetOption fSpectrometerMagnetOption;
    G4double fSpectrometerMagnetField;
    G4double fSpectrometerMagnetTotalSizeZ;
    // SAMURAI design
    G4double fSpectrometerMagnetWindowX;
    G4double fSpectrometerMagnetWindowY;
    G4double fSpectrometerMagnetWindowZ;
    G4double fSpectrometerMagnetYokeThickX;
    G4double fSpectrometerMagnetYokeThickY;
    // Crystal-Pulling design
    G4double fSpectrometerMagnetInnerR;
    G4double fSpectrometerMagnetOuterR;
    G4double fSpectrometerMagnetLengthZ;
    G4int fNSpectrometerMagnets;
    G4double fSpectrometerMagnetGap;
    // Tracking stations
    G4int fNTrackingStations;
    G4int fNScintillatorBarsY;
    G4int fNScintillatorBarsX;
    G4double fScintillatorThickness;
    G4double fTrackingStationGap;
};

#endif 
