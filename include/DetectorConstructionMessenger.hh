#ifndef DETECTORCONSTRUCTIONMESSENGER_HH
#define DETECTORCONSTRUCTIONMESSENGER_HH

#include "G4UImessenger.hh"
#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstructionMessenger: public G4UImessenger {
  public:
    DetectorConstructionMessenger(DetectorConstruction*);
    ~DetectorConstructionMessenger();

    void SetNewValue(G4UIcommand*, G4String);

  private:
    DetectorConstruction* det;

    G4UIdirectory* detDir;
    G4UIdirectory* flareDir;
    G4UIdirectory* babymindDir;
    G4UIdirectory* formosaDir;
    G4UIdirectory* faserDir;
    G4UIdirectory* fasernuDir;
 
    // ------------------------------------
    // General /det/ commands   
    G4UIcmdWithABool* detGdmlSaveCmd;
    G4UIcmdWithAString* detGdmlFileCmd;
    G4UIcmdWithABool* detCheckOverlapCmd;
    G4UIcmdWithABool* detAddFLArECmd;
    G4UIcmdWithABool* detAddFORMOSACmd;
    G4UIcmdWithABool* detAddFASERnu2Cmd;
    G4UIcmdWithABool* detAddFASER2Cmd;

    // ------------------------------------
    // Detector /det/flare/ commands
    G4UIcmdWith3VectorAndUnit* flarePosCmd;
    G4UIcmdWithAString* flareMatCmd;
    G4UIcmdWithAString* flareGeomCmd;
    G4UIcmdWithADoubleAndUnit* flareFieldCmd;
    G4UIcmdWithABool* flareUseBabyMINDCmd;
    
    // ------------------------------------
    // Detector /det/babymind/ commands
    G4UIcmdWithADoubleAndUnit* babymindMagnetPlateThickCmd;
    G4UIcmdWithADoubleAndUnit* babymindMagnetPlateSizeXCmd;
    G4UIcmdWithADoubleAndUnit* babymindMagnetPlateSizeYCmd;
    G4UIcmdWithADoubleAndUnit* babymindMagnetCentralPlateYCmd;
    G4UIcmdWithADoubleAndUnit* babymindSlitSizeXCmd;
    G4UIcmdWithADoubleAndUnit* babymindSlitSizeYCmd;
    G4UIcmdWithADoubleAndUnit* babymindFieldStrengthCmd;
    G4UIcmdWithAnInteger* babymindNVerticalBarsCmd;
    G4UIcmdWithAnInteger* babymindNHorizontalBarsCmd;
    G4UIcmdWithADoubleAndUnit* babymindBarThicknessCmd;
    G4UIcmdWithADoubleAndUnit* babymindVerticalBarSizeXCmd;
    G4UIcmdWithADoubleAndUnit* babymindVerticalBarSizeYCmd;
    G4UIcmdWithADoubleAndUnit* babymindHorizontalBarSizeXCmd;
    G4UIcmdWithADoubleAndUnit* babymindHorizontalBarSizeYCmd;
    G4UIcmdWithADoubleAndUnit* babymindMagnetToScinSpacingCmd;
    G4UIcmdWithADoubleAndUnit* babymindMagnetToMagnetSpacingCmd;
    G4UIcmdWithADoubleAndUnit* babymindBlockToBlockSpacingCmd;
    G4UIcmdWithADoubleAndUnit* babymindBlockPaddingCmd;
    G4UIcmdWithAString* babymindBlockSequenceCmd;  

    // ------------------------------------
    // Detector /det/formosa/ commands
    G4UIcmdWith3VectorAndUnit* formosaPosCmd;

    // ------------------------------------
    // Detector /det/fasernu/ commands
    G4UIcmdWith3VectorAndUnit* fasernuPosCmd;
    
    // ------------------------------------
    // Detector /det/faser/ commands
    G4UIcmdWith3VectorAndUnit* faserPosCmd;
    // SAMURAI
    G4UIcmdWithAString* faserMagnetGeomCmd;
    G4UIcmdWithADoubleAndUnit* faserMagnetFieldCmd;
    G4UIcmdWithADoubleAndUnit* faserMagnetWinXCmd;
    G4UIcmdWithADoubleAndUnit* faserMagnetWinYCmd;
    G4UIcmdWithADoubleAndUnit* faserMagnetWinZCmd;
    G4UIcmdWithADoubleAndUnit* faserYokeThickXCmd;
    G4UIcmdWithADoubleAndUnit* faserYokeThickYCmd;
    // CrystalPulling
    G4UIcmdWithADoubleAndUnit* faserMagnetInnerRCmd;
    G4UIcmdWithADoubleAndUnit* faserMagnetOuterRCmd;
    G4UIcmdWithADoubleAndUnit* faserMagnetLengthZCmd;
    G4UIcmdWithADoubleAndUnit* faserMagnetGapCmd;
    G4UIcmdWithAnInteger* faserMagnetNumberCmd;
    // Tracking stations
    G4UIcmdWithAnInteger* faserTrackingNumberCmd;

    G4UIcmdWithADoubleAndUnit* faserTrackingScinThickCmd;
    G4UIcmdWithADoubleAndUnit* faserTrackingGapCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
