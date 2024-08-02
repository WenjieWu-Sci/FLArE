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
    
    G4UIcmdWithABool* detGdmlCmd;
    G4UIcmdWithABool* detCheckOverlapCmd;

    // FLArE
    G4UIcmdWithABool* detAddFLArECmd;
    G4UIcmdWith3VectorAndUnit* detFLArEPosCmd;
    G4UIcmdWithAString* detMatCmd;
    G4UIcmdWithAString* detGeomCmd;
    G4UIcmdWithADoubleAndUnit* detFieldCmd;
    //BabyMIND
    G4UIcmdWithABool* detAddBabyMINDCmd;
    // FORMOSA
    G4UIcmdWithABool* detAddFORMOSACmd;
    G4UIcmdWith3VectorAndUnit* detFORMOSAPosCmd;
    // FASERnu2
    G4UIcmdWithABool* detAddFASERnu2Cmd;
    G4UIcmdWith3VectorAndUnit* detFASERnu2PosCmd;
    // FASER2 SPECTROMETER MAGNET
    G4UIcmdWithABool* detAddFASER2Cmd;
    G4UIcmdWith3VectorAndUnit* detFASER2PosCmd;
    // SAMURAI
    G4UIcmdWithAString* magnetGeomCmd;
    G4UIcmdWithADoubleAndUnit* magnetFieldCmd;
    G4UIcmdWithADoubleAndUnit* magnetWinXCmd;
    G4UIcmdWithADoubleAndUnit* magnetWinYCmd;
    G4UIcmdWithADoubleAndUnit* magnetWinZCmd;
    G4UIcmdWithADoubleAndUnit* yokeThickXCmd;
    G4UIcmdWithADoubleAndUnit* yokeThickYCmd;
    // CrystalPulling
    G4UIcmdWithADoubleAndUnit* magnetInnerRCmd;
    G4UIcmdWithADoubleAndUnit* magnetOuterRCmd;
    G4UIcmdWithADoubleAndUnit* magnetLengthZCmd;
    G4UIcmdWithADoubleAndUnit* magnetGapCmd;
    G4UIcmdWithAnInteger* magnetNumberCmd;
    // Tracking stations
    G4UIcmdWithAnInteger* trackingNumberCmd;
    G4UIcmdWithAnInteger* trackingNBarsYCmd;
    G4UIcmdWithAnInteger* trackingNBarsXCmd;
    G4UIcmdWithADoubleAndUnit* trackingScinThickCmd;
    G4UIcmdWithADoubleAndUnit* trackingGapCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
