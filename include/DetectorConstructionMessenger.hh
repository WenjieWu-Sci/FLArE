#ifndef DETECTORCONSTRUCTIONMESSENGER_HH
#define DETECTORCONSTRUCTIONMESSENGER_HH

#include "G4UImessenger.hh"
#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
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
    
    // FLArE
    G4UIcmdWithABool* detGdmlCmd;
    G4UIcmdWithABool* detAddFLArECmd;
    G4UIcmdWithAString* detMatCmd;
    G4UIcmdWithAString* detGeomCmd;
    G4UIcmdWithADoubleAndUnit* detFieldCmd;
    // FORMOSA
    G4UIcmdWithABool* detAddFormosaCmd;
    // FASERnu2
    G4UIcmdWithABool* detAddFASERnu2Cmd;
    // FASER2 SPECTROMETER MAGNET
    G4UIcmdWithABool* detAddFASER2Cmd;
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
