#ifndef PrimaryGeneratorMessenger_h
#define PrimaryGeneratorMessenger_h

#include "G4UImessenger.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithAString;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PrimaryGeneratorMessenger: public G4UImessenger
{
  public:
    PrimaryGeneratorMessenger(PrimaryGeneratorAction*);
    ~PrimaryGeneratorMessenger();

    void SetNewValue(G4UIcommand*, G4String);
    
  private:
    PrimaryGeneratorAction* fPrimGenAction;

    G4UIdirectory* fGeneratorDir;
    G4UIcmdWithAString* fGeneratorOption;

};

#endif
