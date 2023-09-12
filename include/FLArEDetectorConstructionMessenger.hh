#ifndef FLAREDETECTORCONSTRUCTIONMESSENGER_HH
#define FLAREDETECTORCONSTRUCTIONMESSENGER_HH

#include "G4UImessenger.hh"
#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class FLArEDetectorConstruction;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class FLArEDetectorConstructionMessenger: public G4UImessenger {
  public:
    FLArEDetectorConstructionMessenger(FLArEDetectorConstruction*);
    ~FLArEDetectorConstructionMessenger();

    void SetNewValue(G4UIcommand*, G4String);

  private:
    FLArEDetectorConstruction* det;
    G4UIdirectory* detDir;
    
    // GENERAL OPTIONS
    G4UIcmdWithAString* detMatCmd;
    G4UIcmdWithAString* detGeomCmd;
    G4UIcmdWithABool* detGdmlCmd;
    G4UIcmdWithADoubleAndUnit* detFieldCmd;

    // FASER2 SPECTROMETER MAGNET
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
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
