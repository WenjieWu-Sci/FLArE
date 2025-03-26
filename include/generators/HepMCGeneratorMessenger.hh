#ifndef HepMCGeneratorMessenger_h
#define HepMCGeneratorMessenger_h

#include "G4UImessenger.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class HepMCGenerator;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithABool;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class HepMCGeneratorMessenger: public G4UImessenger
{
  public:
    HepMCGeneratorMessenger(HepMCGenerator*);
    ~HepMCGeneratorMessenger();

    void SetNewValue(G4UIcommand*, G4String);
    
  private:
    HepMCGenerator* fHepMCAction;

    G4UIdirectory* fHepMCGeneratorDir;
    G4UIcmdWithAString* fHepMCInputFileCmd;
    G4UIcmdWith3VectorAndUnit* fHepMCVertexOffsetCmd;
    G4UIcmdWithABool* fUseHepMC2Cmd;
    G4UIcmdWithABool* fHepMCPlaceInDecayVolumeCmd;

};

#endif
