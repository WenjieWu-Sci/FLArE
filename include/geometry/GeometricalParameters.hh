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
    G4double GetHallOffsetX() { return fHallOffsetX; }
    G4double GetHallOffsetY() { return fHallOffsetY; }

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

    // BabyMIND
    G4double GetBabyMINDMagnetPlateThickness() { return fBabyMINDMagnetPlateThickness; }
    G4double GetBabyMINDMagnetPlateSizeX() { return fBabyMINDMagnetPlateSizeX; }
    G4double GetBabyMINDMagnetPlateSizeY() { return fBabyMINDMagnetPlateSizeY; }
    G4double GetBabyMINDMagnetCentralPlateY() { return fBabyMINDMagnetCentralPlateY; }
    G4double GetBabyMINDSlitSizeX() { return fBabyMINDSlitSizeX; }
    G4double GetBabyMINDSlitSizeY() { return fBabyMINDSlitSizeY; }
    G4double GetBabyMINDFieldStrength() { return fBabyMINDFieldStrength; }
    G4double GetBabyMINDNVerticalBars() { return fBabyMINDNVerticalBars; }
    G4double GetBabyMINDNHorizontalBars() { return fBabyMINDNHorizontalBars; }
    G4double GetBabyMINDBarThickness() { return fBabyMINDBarThickness; }
    G4double GetBabyMINDVerticalBarSizeX() { return fBabyMINDVerticalBarSizeX; }
    G4double GetBabyMINDVerticalBarSizeY() { return fBabyMINDVerticalBarSizeY; }
    G4double GetBabyMINDHorizontalBarSizeX() { return fBabyMINDHorizontalBarSizeX; }
    G4double GetBabyMINDHorizontalBarSizeY() { return fBabyMINDHorizontalBarSizeY; }
    G4double GetBabyMINDTotalSizeZ() { return fBabyMINDTotalSizeZ; }
    G4double GetBabyMINDMagnetToScinSpacing() { return fBabyMINDMagnetToScinSpacing; };
    G4double GetBabyMINDMagnetToMagnetSpacing() { return fBabyMINDMagnetToMagnetSpacing; };
    G4double GetBabyMINDBlockToBlockSpacing() { return fBabyMINDBlockToBlockSpacing; };
    G4double GetBabyMINDBlockPadding() { return fBabyMINDBlockPadding; };
    G4String GetBabyMINDBlockSequence() { return fBabyMINDBlockSequence; };
    G4bool GetUseBabyMIND() { return fUseBabyMIND; }
    void SetBabyMINDMagnetPlateThickness(G4double val) { fBabyMINDMagnetPlateThickness = val; }
    void SetBabyMINDMagnetPlateSizeX(G4double val) { fBabyMINDMagnetPlateSizeX = val; }
    void SetBabyMINDMagnetPlateSizeY(G4double val) { fBabyMINDMagnetPlateSizeY = val; }
    void SetBabyMINDMagnetCentralPlateY(G4double val) { fBabyMINDMagnetCentralPlateY = val; }
    void SetBabyMINDSlitSizeX(G4double val) { fBabyMINDSlitSizeX = val; }
    void SetBabyMINDSlitSizeY(G4double val) { fBabyMINDSlitSizeY = val; }
    void SetBabyMINDFieldStrength(G4double val) { fBabyMINDFieldStrength = val; }
    void SetBabyMINDNVerticalBars(G4int val) { fBabyMINDNVerticalBars = val; }
    void SetBabyMINDNHorizontalBars(G4int val) { fBabyMINDNHorizontalBars = val; }
    void SetBabyMINDBarThickness(G4double val) { fBabyMINDBarThickness = val; }
    void SetBabyMINDVerticalBarSizeX(G4double val) { fBabyMINDVerticalBarSizeX = val; }
    void SetBabyMINDVerticalBarSizeY(G4double val) { fBabyMINDVerticalBarSizeY = val; }
    void SetBabyMINDHorizontalBarSizeX(G4double val) { fBabyMINDHorizontalBarSizeX = val; }
    void SetBabyMINDHorizontalBarSizeY(G4double val) { fBabyMINDHorizontalBarSizeY = val; }
    void SetBabyMINDMagnetToScinSpacing(G4double val) { fBabyMINDMagnetToScinSpacing = val; };
    void SetBabyMINDMagnetToMagnetSpacing(G4double val) { fBabyMINDMagnetToMagnetSpacing = val; };
    void SetBabyMINDBlockToBlockSpacing(G4double val) { fBabyMINDBlockToBlockSpacing = val; };
    void SetBabyMINDBlockPadding(G4double val)  { fBabyMINDBlockPadding = val; };
    void SetBabyMINDBlockSequence(G4String val) { fBabyMINDBlockSequence = val; };
    void SetUseBabyMIND(G4bool val) { fUseBabyMIND = val; }
    void SetBabyMINDTotalSizeZ(G4double val) { fBabyMINDTotalSizeZ = val; }

    // FASER2 Spectrometer Magnet
    enum magnetOption{ SAMURAI, CrystalPulling, unknown};
    magnetOption ConvertStringToMagnetOption(G4String val);
    void SetFASER2MagnetOption(magnetOption val) { fFASER2MagnetOption = val; }
    magnetOption GetFASER2MagnetOption() { return fFASER2MagnetOption; }
    void SetFASER2MagnetField(G4double val) { fFASER2MagnetField = val; }
    G4ThreeVector GetFASER2MagnetField();
    void SetMagnetTotalSizeZ(G4double val) { fMagnetTotalSizeZ = val; }
    G4double GetMagnetTotalSizeZ() { return fMagnetTotalSizeZ; }
    void SetTrackingStationTotalSizeZ(G4double val) { fTrackingStationTotalSizeZ = val; }
    G4double GetTrackingStationTotalSizeZ() { return fTrackingStationTotalSizeZ; }
    void SetMagnetZPosition(G4double val) { fMagnetZPos = val; }
    G4double GetMagnetZPosition() { return fMagnetZPos; }
    void SetFASER2TotalSizeZ(G4double val) { fFASER2TotalSizeZ = val; }
    G4double GetFASER2TotalSizeZ() { return fFASER2TotalSizeZ; }
    void SetfFASER2DecayVolumeLength (G4double val) { fFASER2DecayVolumeLength = val;}
    G4double GetfFASER2DecayVolumeLength() { return fFASER2DecayVolumeLength; }
    void SetFASER2TrackerX(G4double val) { fFASER2TrackerX = val; }
    G4double GetFASER2TrackerX() { return fFASER2TrackerX; }
    void SetFASER2TrackerY(G4double val) { fFASER2TrackerY = val; }
    G4double GetFASER2TrackerY() { return fFASER2TrackerY; }
    // SAMURAI design
    void SetFASER2MagnetWindowX(G4double val) { fFASER2MagnetWindowX = val; }
    G4double GetFASER2MagnetWindowX() { return fFASER2MagnetWindowX; }
    void SetFASER2MagnetWindowY(G4double val) { fFASER2MagnetWindowY = val; }
    G4double GetFASER2MagnetWindowY() { return fFASER2MagnetWindowY; }
    void SetFASER2MagnetWindowZ(G4double val) { fFASER2MagnetWindowZ = val; }
    G4double GetFASER2MagnetWindowZ() { return fFASER2MagnetWindowZ; }
    void SetFASER2MagnetYokeThickX(G4double val) { fFASER2MagnetYokeThickX = val; }
    G4double GetFASER2MagnetYokeThickX() { return fFASER2MagnetYokeThickX; }
    void SetFASER2MagnetYokeThickY(G4double val) { fFASER2MagnetYokeThickY = val; }
    G4double GetFASER2MagnetYokeThickY() { return fFASER2MagnetYokeThickY; }
    // Crystal-Pulling design
    void SetFASER2MagnetInnerR(G4double val) { fFASER2MagnetInnerR = val; }
    G4double GetFASER2MagnetInnerR() { return fFASER2MagnetInnerR; }
    void SetFASER2MagnetOuterR(G4double val) { fFASER2MagnetOuterR = val; }
    G4double GetFASER2MagnetOuterR() { return fFASER2MagnetOuterR; }
    void SetFASER2MagnetLengthZ(G4double val) { fFASER2MagnetLengthZ = val; }
    G4double GetFASER2MagnetLengthZ() { return fFASER2MagnetLengthZ; }
    void SetNFASER2Magnets(G4int val) { fNFASER2Magnets = val; }
    G4int GetNFASER2Magnets() { return fNFASER2Magnets; }
    void SetFASER2MagnetGap(G4double val) { fFASER2MagnetGap = val; }
    G4double GetFASER2MagnetGap() { return fFASER2MagnetGap; }  
    void SetFASER2MagnetSpacing(G4double val) { fFASER2MagnetSpacing = val; }
    G4double GetFASER2MagnetSpacing() { return fFASER2MagnetSpacing; }  
    // Tracking station
    void SetNTrackingStations(G4int val) { fNTrackingStations = val; }
    G4int GetNTrackingStations() { return fNTrackingStations; }  
    void SetScintillatorThickness(G4double val) { fScintillatorThickness = val; }
    G4double GetScintillatorThickness() { return fScintillatorThickness; }  
    void SetTrackingStationGap(G4double val) { fTrackingStationGap = val; }
    G4double GetTrackingStationGap() { return fTrackingStationGap; }  
    G4double GetFASER2UpstreamTrackingStationGap() { return fFASER2UpstreamTrackingStationGap; }
    G4double GetFASER2DownstreamTrackingStationGap() { return fFASER2DownstreamTrackingStationGap; }
    G4double GetFASER2NUpstreamTrackers() { return fFASER2NUpstreamTrackers; }
    G4double GetFASER2NDownstreamTrackers() { return fFASER2NDownstreamTrackers; }
    G4double GetFASER2EMCaloThickness() { return fFASER2EMCaloThickness; }
    G4double GetFASER2HadCaloThickness() { return fFASER2HadCaloThickness; }
    G4double GetFASER2IronWallThickness() { return fFASER2IronWallThickness; }
    G4double GetFASER2VetoLengthX() { return fFASER2VetoLengthX; }
    G4double GetFASER2VetoLengthY() { return fFASER2VetoLengthY; }
    G4double GetFASER2VetoShieldThickness() { return fFASER2VetoShieldThickness; }
    void SetFASER2UpstreamTrackingStationGap(G4double val) { fFASER2UpstreamTrackingStationGap = val; }
    void SetFASER2DownstreamTrackingStationGap(G4double val) { fFASER2DownstreamTrackingStationGap = val; }
    void SetFASER2NUpstreamTrackers(G4double val) { fFASER2NUpstreamTrackers = val; }
    void SetFASER2NDownstreamTrackers(G4double val) { fFASER2NDownstreamTrackers = val; }
    void SetFASER2EMCaloThickness(G4double val) { fFASER2EMCaloThickness = val; }
    void SetFASER2HadCaloThickness(G4double val) { fFASER2HadCaloThickness = val; }
    void SetFASER2IronWallThickness(G4double val) { fFASER2IronWallThickness = val; }
    void SetFASER2VetoLengthX(G4double val) { fFASER2VetoLengthX = val; }
    void SetFASER2VetoLengthY(G4double val) { fFASER2VetoLengthY = val; }
    void SetFASER2VetoShieldThickness(G4double val) { fFASER2VetoShieldThickness = val; }
    
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

    // Sensitive detectors
    void AddSD2List(int idx, std::string val) { fSDNamelist.insert(std::make_pair(idx, val)); }
    std::set<std::pair<int, std::string> > GetSDNamelist() { return fSDNamelist; }

  private:
    //the singleton
    static GeometricalParameters *me;

    // experimental hall
    G4double fHallHeadDistance; ///<- distance between the entrance wall and the first detector
    G4double fHallOffsetX; // x offset of hall center from the LOS
    G4double fHallOffsetY; // x offset of hall center from the LOS

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
 
    // BabyMIND
    G4bool fUseBabyMIND;
    G4double fBabyMINDMagnetPlateThickness;
    G4double fBabyMINDMagnetPlateSizeX;
    G4double fBabyMINDMagnetPlateSizeY;
    G4double fBabyMINDMagnetCentralPlateY;
    G4double fBabyMINDSlitSizeX;
    G4double fBabyMINDSlitSizeY;
    G4double fBabyMINDFieldStrength;
    G4int fBabyMINDNVerticalBars;
    G4int fBabyMINDNHorizontalBars;
    G4double fBabyMINDBarThickness;
    G4double fBabyMINDVerticalBarSizeX;
    G4double fBabyMINDVerticalBarSizeY;
    G4double fBabyMINDHorizontalBarSizeX;
    G4double fBabyMINDHorizontalBarSizeY;
    G4double fBabyMINDTotalSizeZ;
    G4double fBabyMINDMagnetToScinSpacing;
    G4double fBabyMINDMagnetToMagnetSpacing;
    G4double fBabyMINDBlockToBlockSpacing;
    G4double fBabyMINDBlockPadding;
    G4String fBabyMINDBlockSequence;

    // FASER2 Spectrometer Magnet
    magnetOption fFASER2MagnetOption;
    G4double fFASER2MagnetField;
    G4double fMagnetTotalSizeZ;
    G4double fTrackingStationTotalSizeZ;
    G4double fMagnetZPos;
    G4double fFASER2TotalSizeZ;
    G4double fFASER2DecayVolumeLength;
    
    // SAMURAI design
    G4double fFASER2MagnetWindowX;
    G4double fFASER2MagnetWindowY;
    G4double fFASER2MagnetWindowZ;
    G4double fFASER2MagnetYokeThickX;
    G4double fFASER2MagnetYokeThickY;
    // Crystal-Pulling design
    G4double fFASER2MagnetInnerR;
    G4double fFASER2MagnetOuterR;
    G4double fFASER2MagnetLengthZ;
    G4int fNFASER2Magnets;
    G4double fFASER2MagnetGap;
    G4double fFASER2MagnetSpacing;
    // Tracking stations
    G4int fNTrackingStations;
    G4double fFASER2TrackerX;
    G4double fFASER2TrackerY;
    G4int fNScintillatorBarsY;
    G4int fNScintillatorBarsX;
    G4double fScintillatorThickness;
    G4double fTrackingStationGap;
    G4ThreeVector fFASER2Pos;
    G4double fFASER2UpstreamTrackingStationGap;
    G4double fFASER2DownstreamTrackingStationGap;
    G4double fFASER2NUpstreamTrackers;
    G4double fFASER2NDownstreamTrackers;
    G4double fFASER2EMCaloThickness;
    G4double fFASER2HadCaloThickness;
    G4double fFASER2IronWallThickness;
    G4double fFASER2VetoLengthX;
    G4double fFASER2VetoLengthY;
    G4double fFASER2VetoShieldThickness;


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

    // Sensitive detectors
    std::set<std::pair<int, std::string> > fSDNamelist;
};

#endif 
