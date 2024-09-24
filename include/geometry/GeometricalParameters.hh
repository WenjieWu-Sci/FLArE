#ifndef GeometricalParameters_hh
#define GeometricalParameters_hh

#include <set>

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

    // experimental hall
    G4double GetHallHeadDistance() { return fHallHeadDistance; }

    // FLArE TPC volume
    enum tpcMaterialOption { LiquidArgon, LiquidKrypton};
    tpcMaterialOption ConvertStringToTPCMaterialOption(G4String val);
    void SetTPCMaterialOption(tpcMaterialOption val) { fFLArETPCMaterialOption = val; }
    tpcMaterialOption GetTPCMaterialOption() { return fFLArETPCMaterialOption; }
    enum tpcConfigOption { Single, ThreeBySeven};
    tpcConfigOption ConvertStringToTPCConfigOption(G4String val);
    void SetTPCConfigOption(tpcConfigOption val) { fFLArETPCConfigOption = val; }
    tpcConfigOption GetTPCConfigOption() { return fFLArETPCConfigOption; }
    G4double GetTPCSizeX() { return fTPCSizeX; }
    G4double GetTPCSizeY() { return fTPCSizeY; }
    G4double GetTPCSizeZ() { return fTPCSizeZ; }
    G4ThreeVector GetTPCSizeXYZ() { return G4ThreeVector(fTPCSizeX, fTPCSizeY, fTPCSizeZ); }
    G4ThreeVector GetFLArEFidVolSize() { return fTPCFidVolSize; }
    G4double GetTPCInsulationThickness() { return fInsulationThickness; }
    G4double GetHadCalLength() { return fHadCalLength; }
    void SetHadCalLength(G4double val) { fHadCalLength = val; }
    G4double GetMuonCatcherLength() { return fMuonCatcherLength; }
    void SetMuonCatcherLength(G4double val) { fMuonCatcherLength = val; }

    G4ThreeVector GetFLArEPosition() { return fFLArEPos; }
    void SetFLArEPosition(G4ThreeVector val) { fFLArEPos = val; }

    // FASER2 Spectrometer Magnet
    enum magnetOption{ SAMURAI, CrystalPulling, unknown};
    magnetOption ConvertStringToMagnetOption(G4String val);
    void SetSpectrometerMagnetOption(magnetOption val) { fSpectrometerMagnetOption = val; }
    magnetOption GetSpectrometerMagnetOption() { return fSpectrometerMagnetOption; }
    void SetSpectrometerMagnetField(G4double val) { fSpectrometerMagnetField = val; }
    G4ThreeVector GetSpectrometerMagnetField();
    void SetMagnetTotalSizeZ(G4double val) { fMagnetTotalSizeZ = val; }
    G4double GetMagnetTotalSizeZ() { return fMagnetTotalSizeZ; }
    void SetTrackingStationTotalSizeZ(G4double val) { fTrackingStationTotalSizeZ = val; }
    G4double GetTrackingStationTotalSizeZ() { return fTrackingStationTotalSizeZ; }
    void SetMagnetZPosition(G4double val) { fMagnetZPos = val; }
    G4double GetMagnetZPosition() { return fMagnetZPos; }
    void SetFASER2TotalSizeZ(G4double val) { fFASER2TotalSizeZ = val; }
    G4double GetFASER2TotalSizeZ() { return fFASER2TotalSizeZ; }
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
    void SetSpectrometerMagnetSpacing(G4double val) { fSpectrometerMagnetSpacing = val; }
    G4double GetSpectrometerMagnetSpacing() { return fSpectrometerMagnetSpacing; }  
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
    
    G4ThreeVector GetFASER2Position() { return fFASER2Pos; }
    void SetFASER2Position(G4ThreeVector val) { fFASER2Pos = val; }

    //FASERnu2 Emulsion detector
    void SetFASERnu2TotalSizeZ(G4double val) { fFASERnu2TotalSizeZ = val; }
    G4double GetFASERnu2TotalSizeZ() { return fFASERnu2TotalSizeZ; }
    //Emulsion/Tungsten
    void SetNEmulsionTungstenLayers(G4int val) { fNEmulsionTungstenLayers = val; }
    G4int GetNEmulsionTungstenLayers() { return fNEmulsionTungstenLayers; }  
    void SetTungstenThickness(G4double val) { fTungstenThickness = val; }
    G4double GetTungstenThickness() { return fTungstenThickness; }  
    void SetEmulsionThickness(G4double val) { fEmulsionThickness = val; }
    G4double GetEmulsionThickness() { return fEmulsionThickness; }  
    void SetEmulsionTungstenSizeX(G4double val) { fEmulsionTungstenSizeX = val; }
    G4double GetEmulsionTungstenSizeX() { return fEmulsionTungstenSizeX; }  
    void SetEmulsionTungstenSizeY(G4double val) { fEmulsionTungstenSizeY = val; }
    G4double GetEmulsionTungstenSizeY() { return fEmulsionTungstenSizeY; }  
    // Veto/Interface detectors
    void SetVetoInterfaceSizeX(G4double val) { fVetoInterfaceSizeX = val; }
    G4double GetVetoInterfaceSizeX() { return fVetoInterfaceSizeX; }  
    void SetVetoInterfaceSizeY(G4double val) { fVetoInterfaceSizeY = val; }
    G4double GetVetoInterfaceSizeY() { return fVetoInterfaceSizeY; }  
    void SetVetoInterfaceSizeZ(G4double val) { fVetoInterfaceSizeZ = val; }
    G4double GetVetoInterfaceSizeZ() { return fVetoInterfaceSizeZ; }  

    G4ThreeVector GetFASERnu2Position() { return fFASERnu2Pos; }
    void SetFASERnu2Position(G4ThreeVector val) { fFASERnu2Pos = val; }
    
    // FORMOSA
    void SetFORMOSATotalSizeZ(G4double val) { fFORMOSATotalSizeZ = val; }
    G4double GetFORMOSATotalSizeZ() { return fFORMOSATotalSizeZ; }
    // Scintillator blocks
    void SetNScinBarsX(G4int val) { fNScinBarsX = val; }
    G4int GetNScinBarsX() {return fNScinBarsX; }
    void SetNScinBarsY(G4int val) { fNScinBarsY = val; }
    G4int GetNScinBarsY() {return fNScinBarsY; }
    void SetScintillatorBarSizeX(G4double val) { fScintillatorBarSizeX = val; }
    G4double GetScintillatorBarSizeX() {return fScintillatorBarSizeX; }
    void SetScintillatorBarSizeY(G4double val) { fScintillatorBarSizeY = val; }
    G4double GetScintillatorBarSizeY() {return fScintillatorBarSizeY; }
    void SetScintillatorBarSizeZ(G4double val) { fScintillatorBarSizeZ = val; }
    G4double GetScintillatorBarSizeZ() {return fScintillatorBarSizeZ; }
    void SetNScintillatorModules(G4int val) { fNScintillatorModules = val; }
    G4int GetNScintillatorModules() {return fNScintillatorModules; }
    void SetPMTSizeSpacing(G4double val) { fPMTSizeSpacing = val; }
    G4double GetPMTSizeSpacing() {return fPMTSizeSpacing; }

    G4ThreeVector GetFORMOSAPosition() { return fFORMOSAPos; }
    void SetFORMOSAPosition(G4ThreeVector val) { fFORMOSAPos = val; }

    // A sampling calorimeter (NuTeV-like detector)
    G4double GetSamplingCaloThicknessCaloX()   { return fSamplingCaloThicknessCaloX; }
    G4double GetSamplingCaloThicknessCaloY()   { return fSamplingCaloThicknessCaloY; }
    G4double GetSamplingCaloThicknessAbsorb()  { return fSamplingCaloThicknessAbsorb; }
    G4double GetSamplingCaloWidthCaloX()   { return fSamplingCaloWidthCaloX; }
    G4double GetSamplingCaloWidthCaloY()   { return fSamplingCaloWidthCaloY; }
    G4int    GetSamplingCaloNLayers()          { return fSamplingCaloNLayers; }
    G4double GetSamplingCaloSizeX()   { return fSamplingCaloSizeX; }
    G4double GetSamplingCaloSizeY()   { return fSamplingCaloSizeY; }
    G4double GetSamplingCaloSizeZ()   { return fSamplingCaloSizeZ; }
    G4ThreeVector GetSamplingCaloFidVolSize() { return fSamplingCaloFidVolSize; }
    void SetSamplingCaloSizeZ(G4double val) { fSamplingCaloSizeZ = val; }
    G4ThreeVector GetSamplingCaloSizeXYZ() { return G4ThreeVector(fSamplingCaloSizeX, fSamplingCaloSizeY, fSamplingCaloSizeZ); }

    G4ThreeVector GetSamplingCaloPosition() { return fSamplingCaloPos; }
    void SetSamplingCaloPosition(G4ThreeVector val) { fSamplingCaloPos = val; }

    // Sensitive detectors
    void AddSD2List(int idx, std::string val) { fSDNamelist.insert(std::make_pair(idx, val)); }
    std::set<std::pair<int, std::string> > GetSDNamelist() { return fSDNamelist; }

  private:
    //the singleton
    static GeometricalParameters *me;

    // experimental hall
    G4double fHallHeadDistance; ///<- distance between the entrance wall and the first detector

    // FLArE TPC volume
    tpcMaterialOption fFLArETPCMaterialOption;
    tpcConfigOption   fFLArETPCConfigOption;
    G4double fTPCSizeX;
    G4double fTPCSizeY;
    G4double fTPCSizeZ;
    G4ThreeVector fTPCFidVolSize;
    G4double fInsulationThickness;
    G4double fHadCalLength;
    G4double fMuonCatcherLength;
    G4ThreeVector fFLArEPos;
    
    // FASER2 Spectrometer Magnet
    magnetOption fSpectrometerMagnetOption;
    G4double fSpectrometerMagnetField;
    G4double fMagnetTotalSizeZ;
    G4double fTrackingStationTotalSizeZ;
    G4double fMagnetZPos;
    G4double fFASER2TotalSizeZ;
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
    G4double fSpectrometerMagnetSpacing;
    // Tracking stations
    G4int fNTrackingStations;
    G4int fNScintillatorBarsY;
    G4int fNScintillatorBarsX;
    G4double fScintillatorThickness;
    G4double fTrackingStationGap;
    G4ThreeVector fFASER2Pos;

    // FASERnu2 Emulsion detector
    G4double fFASERnu2TotalSizeZ;
    // Emulsion/Tungsten
    G4int fNEmulsionTungstenLayers;
    G4double fTungstenThickness;
    G4double fEmulsionThickness;
    G4double fEmulsionTungstenSizeX;
    G4double fEmulsionTungstenSizeY;
    G4double fModuleThickness;
    // Veto/Interface detectors
    G4double fVetoInterfaceSizeZ;
    G4double fVetoInterfaceSizeX;
    G4double fVetoInterfaceSizeY;
    G4ThreeVector fFASERnu2Pos;

    // FORMOSA
    G4double fFORMOSATotalSizeZ;
    // Scintillator blocks
    G4int fNScinBarsX;
    G4int fNScinBarsY;
    G4double fScintillatorBarSizeX;
    G4double fScintillatorBarSizeY;
    G4double fScintillatorBarSizeZ;
    G4int fNScintillatorModules;
    G4double fPMTSizeSpacing;
    G4ThreeVector fFORMOSAPos;

    // A Sampling Calorimeter (NuTeV-like detector)
    G4double fSamplingCaloThicknessCaloX;
    G4double fSamplingCaloThicknessCaloY;
    G4double fSamplingCaloThicknessAbsorb;
    G4double fSamplingCaloWidthCaloX;
    G4double fSamplingCaloWidthCaloY;
    G4double fSamplingCaloSizeX;
    G4double fSamplingCaloSizeY;
    G4double fSamplingCaloSizeZ;
    G4ThreeVector fSamplingCaloFidVolSize;
    G4int    fSamplingCaloNLayers;
    G4ThreeVector fSamplingCaloPos;

    // Sensitive detectors
    std::set<std::pair<int, std::string> > fSDNamelist;
};

#endif 
