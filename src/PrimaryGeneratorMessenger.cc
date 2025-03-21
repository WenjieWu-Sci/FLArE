#include "PrimaryGeneratorMessenger.hh"

#include "PrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIcmdWithAString.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(PrimaryGeneratorAction* action) 
  : fPrimGenAction(action) 
{
  fGeneratorDir = new G4UIdirectory("/gen/");
  fGeneratorDir->SetGuidance("generator control");

  fGeneratorOption = new G4UIcmdWithAString("/gen/select", this);
  fGeneratorOption->SetGuidance("select generator option");
  fGeneratorOption->SetParameterName("generator",false);
  fGeneratorOption->SetCandidates("gun genie hepmc background");
  fGeneratorOption->SetDefaultValue("gun");
  fGeneratorOption->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{
  delete fGeneratorOption;
  delete fGeneratorDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValues)
{
  if (command == fGeneratorOption) 
    fPrimGenAction->SetGenerator(newValues);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
