#ifndef PrimaryGeneratorMessenger_h
#define PrimaryGeneratorMessenger_h

#include "G4UImessenger.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PrimaryGeneratorMessenger: public G4UImessenger
{
  public:
    PrimaryGeneratorMessenger(PrimaryGeneratorAction*);
    ~PrimaryGeneratorMessenger();

    void SetNewValue(G4UIcommand*, G4String);
  private:
    PrimaryGeneratorAction* PrimGenAction;

    G4UIdirectory* GeneratorDir;
    G4UIcmdWithAString* GHEPInputFile;
    G4UIcmdWithAnInteger* GHEPEvtStartIdx;
    G4UIcmdWithABool* USEGENIE;
};

#endif
