#ifndef FLAREDETECTORCONSTRUCTIONMESSENGER_HH
#define FLAREDETECTORCONSTRUCTIONMESSENGER_HH

#include "G4UImessenger.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class LArBoxDetectorConstruction;
class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithAString;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class FLArEDetectorConstructionMessenger: public G4UImessenger {
  public:
    FLArEDetectorConstructionMessenger(LArBoxDetectorConstruction*);
    ~FLArEDetectorConstructionMessenger();

    void SetNewValue(G4UIcommand*, G4String);

  private:
    LArBoxDetectorConstruction* det;
    G4UIdirectory* detDir;
    G4UIcmdWithAString* detMatCmd;
//    G4UIcommand* updateCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
