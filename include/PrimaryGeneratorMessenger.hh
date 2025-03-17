#ifndef PrimaryGeneratorMessenger_h
#define PrimaryGeneratorMessenger_h

#include "G4UImessenger.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"
#include "G4UIcmdWith3Vector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;
class G4UIcmdWithADoubleAndUnit;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PrimaryGeneratorMessenger: public G4UImessenger
{
  public:
    PrimaryGeneratorMessenger(PrimaryGeneratorAction*);
    ~PrimaryGeneratorMessenger();

    void SetNewValue(G4UIcommand*, G4String);
  private:
    PrimaryGeneratorAction* PrimGenAction;

    G4UIdirectory* GENIEGeneratorDir;
    G4UIcmdWithAString* GHEPInputFile;
    G4UIcmdWithAnInteger* GHEPEvtStartIdx;
    G4UIcmdWithABool* USEGENIE;
    
    G4UIdirectory* HepMCGeneratorDir;
    G4UIcmdWithABool* USEHepMC;
    G4UIcmdWithAString* HepMCInputFile;
    G4UIcmdWith3Vector* HepMCVtxOffset;
    
    G4UIdirectory* bkgGeneratorDir;
    G4UIcmdWithABool* USEBKG;
    G4UIcmdWithAString* bkgInputFile;
    G4UIcmdWithADoubleAndUnit* bkgTimeWindow;

};

#endif
