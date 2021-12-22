#include "PrimaryGeneratorMessenger.hh"

#include "PrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIparameter.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithABool.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(PrimaryGeneratorAction* action) 
  : PrimGenAction(action) 
{
  GeneratorDir = new G4UIdirectory("/genie/");
  GeneratorDir->SetGuidance("genie input control");

  USEGENIE = new G4UIcmdWithABool("/genie/useGenie", this);
  USEGENIE->SetGuidance("set generator to genie");
  USEGENIE->SetParameterName("useGenie", true);
  USEGENIE->SetDefaultValue(false);

  GHEPInputFile = new G4UIcmdWithAString("/genie/genieInput", this);
  GHEPInputFile->SetGuidance("set input filename of the genie generator");
  //GHEPInputFile->SetDefaultValue("");
  GHEPInputFile->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);

  GHEPEvtStartIdx = new G4UIcmdWithAnInteger("/genie/genieIStart", this);
  GHEPEvtStartIdx->SetGuidance("set the index of the start event in the .ghep file");
  GHEPEvtStartIdx->SetDefaultValue((G4int)0);
  GHEPEvtStartIdx->AvailableForStates(G4State_PreInit, G4State_Idle);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{
  delete GHEPInputFile;
  delete GHEPEvtStartIdx;
  delete USEGENIE;
  delete GeneratorDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValues)
{
  if (command == GHEPInputFile) PrimGenAction->setGenieInputFile(newValues);
  if (command == GHEPEvtStartIdx) PrimGenAction->setGenieStartEvt(GHEPEvtStartIdx->GetNewIntValue(newValues));
  if (command == USEGENIE) PrimGenAction->setUseGenie(USEGENIE->GetNewBoolValue(newValues));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
