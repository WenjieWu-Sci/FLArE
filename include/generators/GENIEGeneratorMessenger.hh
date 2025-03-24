#ifndef GENIEGeneratorMessenger_h
#define GENIEGeneratorMessenger_h

#include "G4UImessenger.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class GENIEGenerator;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class GENIEGeneratorMessenger: public G4UImessenger
{
  public:
    GENIEGeneratorMessenger(GENIEGenerator*);
    ~GENIEGeneratorMessenger();

    void SetNewValue(G4UIcommand*, G4String);
    
  private:
    GENIEGenerator* fGENIEAction;

    G4UIdirectory* fGENIEGeneratorDir;
    G4UIcmdWithAString* fGSTInputFileCmd;
    G4UIcmdWithAnInteger* fGSTEvtStartIdxCmd;
    G4UIcmdWithABool* fRandomVtxCmd;

};

#endif
