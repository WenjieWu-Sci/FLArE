#ifndef BackgroundGeneratorMessenger_h
#define BackgroundGeneratorMessenger_h

#include "G4UImessenger.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class BackgroundGenerator;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class BackgroundGeneratorMessenger: public G4UImessenger
{
  public:
    BackgroundGeneratorMessenger(BackgroundGenerator*);
    ~BackgroundGeneratorMessenger();

    void SetNewValue(G4UIcommand*, G4String);
    
  private:
    BackgroundGenerator* fBkgAction;

    G4UIdirectory* fBkgGeneratorDir;
    G4UIcmdWithAString* fBkgInputFileCmd;
    G4UIcmdWithADoubleAndUnit* fBkgTimeWindowCmd;

};

#endif
