#include "generators/GENIEGeneratorMessenger.hh"
#include "generators/GENIEGenerator.hh"

#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithABool.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GENIEGeneratorMessenger::GENIEGeneratorMessenger(GENIEGenerator* action) 
  : fGENIEAction(action) 
{
  fGENIEGeneratorDir = new G4UIdirectory("/gen/genie/");
  fGENIEGeneratorDir->SetGuidance("genie generator control");

  fGSTInputFileCmd = new G4UIcmdWithAString("/gen/genie/genieInput", this);
  fGSTInputFileCmd->SetGuidance("set input filename of the genie generator");
  fGSTInputFileCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);

  fGSTEvtStartIdxCmd = new G4UIcmdWithAnInteger("/gen/genie/genieIStart", this);
  fGSTEvtStartIdxCmd->SetGuidance("set the index of the start event in the .ghep file");
  fGSTEvtStartIdxCmd->SetDefaultValue((G4int)0);
  fGSTEvtStartIdxCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fRandomVtxCmd = new G4UIcmdWithABool("/gen/genie/randomVtx", this);
  fRandomVtxCmd->SetGuidance("set random vertex in fiducial volume");
  fRandomVtxCmd->SetDefaultValue(true);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GENIEGeneratorMessenger::~GENIEGeneratorMessenger()
{
  delete fGSTInputFileCmd;
  delete fGSTEvtStartIdxCmd;
  delete fRandomVtxCmd;
  delete fGENIEGeneratorDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GENIEGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValues)
{
  if (command == fGSTInputFileCmd) fGENIEAction->SetGSTFilename(newValues);
  else if (command == fGSTEvtStartIdxCmd) fGENIEAction->SetEvtStartIdx(fGSTEvtStartIdxCmd->GetNewIntValue(newValues));
  else if (command == fRandomVtxCmd) fGENIEAction->SetRandomVertex(fRandomVtxCmd->GetNewBoolValue(newValues));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
