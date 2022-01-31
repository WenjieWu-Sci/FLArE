#ifndef FLAREDETECTORCONSTRUCTIONMESSENGER_HH
#define FLAREDETECTORCONSTRUCTIONMESSENGER_HH

#include "G4UImessenger.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class FLArEDetectorConstruction;
class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithAString;
class G4UIcmdWithABool;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class FLArEDetectorConstructionMessenger: public G4UImessenger {
  public:
    FLArEDetectorConstructionMessenger(FLArEDetectorConstruction*);
    ~FLArEDetectorConstructionMessenger();

    void SetNewValue(G4UIcommand*, G4String);

  private:
    FLArEDetectorConstruction* det;
    G4UIdirectory* detDir;
    G4UIcmdWithAString* detMatCmd;
    G4UIcmdWithABool* detGeomCmd;
//    G4UIcommand* updateCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
