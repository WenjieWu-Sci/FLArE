#include "generators/HepMCGeneratorMessenger.hh"
#include "generators/HepMCGenerator.hh"

#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithABool.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HepMCGeneratorMessenger::HepMCGeneratorMessenger(HepMCGenerator* action) 
  : fHepMCAction(action) 
{
  fHepMCGeneratorDir = new G4UIdirectory("/gen/hepmc/");
  fHepMCGeneratorDir->SetGuidance("hepmc generator control");

  fHepMCInputFileCmd = new G4UIcmdWithAString("/gen/hepmc/hepmcInput", this);
  fHepMCInputFileCmd->SetGuidance("set input filename of the HepMC generator");
  fHepMCInputFileCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);

  fHepMCVertexOffsetCmd = new G4UIcmdWith3VectorAndUnit("/gen/hepmc/vtxOffset", this);
  fHepMCVertexOffsetCmd->SetGuidance("set the offset of the primary vertex - useful when there is a mismatch in the geometry");
  fHepMCVertexOffsetCmd->SetParameterName("x", "y", "z", false, false);
  fHepMCVertexOffsetCmd->SetUnitCandidates("mm m cm");
  fHepMCVertexOffsetCmd->SetDefaultUnit("mm");
  fHepMCVertexOffsetCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);
  
  fUseHepMC2Cmd = new G4UIcmdWithABool("/gen/hepmc/useHepMC2", this);
  fUseHepMC2Cmd->SetGuidance("set generator to hepmc2");
  fUseHepMC2Cmd->SetParameterName("useHepMC2", true);
  fUseHepMC2Cmd->SetDefaultValue(false);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HepMCGeneratorMessenger::~HepMCGeneratorMessenger()
{
  delete fHepMCInputFileCmd;
  delete fHepMCVertexOffsetCmd;
  delete fUseHepMC2Cmd;
  delete fHepMCGeneratorDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HepMCGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValues)
{
  if (command == fHepMCInputFileCmd) fHepMCAction->SetHepMCFilename(newValues);
  else if (command == fHepMCVertexOffsetCmd) fHepMCAction->SetHepMCVertexOffset(fHepMCVertexOffsetCmd->GetNew3VectorValue(newValues));
  else if (command == fUseHepMC2Cmd) fHepMCAction->SetUseHepMC2(fUseHepMC2Cmd->GetNewBoolValue(newValues));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
