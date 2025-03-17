#include "PrimaryGeneratorMessenger.hh"

#include "PrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIparameter.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(PrimaryGeneratorAction* action) 
  : PrimGenAction(action) 
{
  GENIEGeneratorDir = new G4UIdirectory("/genie/");
  GENIEGeneratorDir->SetGuidance("genie input control");

  HepMCGeneratorDir = new G4UIdirectory("/hepmc/");
  HepMCGeneratorDir->SetGuidance("hepmc input control");

  USEGENIE = new G4UIcmdWithABool("/genie/useGenie", this);
  USEGENIE->SetGuidance("set generator to genie");
  USEGENIE->SetParameterName("useGenie", true);
  USEGENIE->SetDefaultValue(false);

  GHEPInputFile = new G4UIcmdWithAString("/genie/genieInput", this);
  GHEPInputFile->SetGuidance("set input filename of the genie generator");
  GHEPInputFile->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);

  GHEPEvtStartIdx = new G4UIcmdWithAnInteger("/genie/genieIStart", this);
  GHEPEvtStartIdx->SetGuidance("set the index of the start event in the .ghep file");
  GHEPEvtStartIdx->SetDefaultValue((G4int)0);
  GHEPEvtStartIdx->AvailableForStates(G4State_PreInit, G4State_Idle);

  USEHepMC = new G4UIcmdWithABool("/hepmc/useHepMC", this);
  USEHepMC->SetGuidance("set generator to hepmc");
  USEHepMC->SetParameterName("useHepMC", true);
  USEHepMC->SetDefaultValue(false);

  HepMCInputFile = new G4UIcmdWithAString("/hepmc/hepmcInput", this);
  HepMCInputFile->SetGuidance("set input filename of the hepmc generator");
  HepMCInputFile->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);
  
  HepMCVtxOffset = new G4UIcmdWith3Vector("/hepmc/vtxOffset", this);
  HepMCVtxOffset->SetGuidance("set the offset of the primary vertex - useful when there is a mismatch in the geometry");
  HepMCVtxOffset->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);

  bkgGeneratorDir = new G4UIdirectory("/bkg/");
  bkgGeneratorDir->SetGuidance("background input control");

  USEBKG = new G4UIcmdWithABool("/bkg/useBackground", this);
  USEBKG->SetGuidance("set generator to background");
  USEBKG->SetParameterName("useBackground", true);
  USEBKG->SetDefaultValue(false);

  bkgInputFile = new G4UIcmdWithAString("/bkg/backgroundInput", this);
  bkgInputFile->SetGuidance("set input filename of the background generator");
  bkgInputFile->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);
    
  bkgTimeWindow = new G4UIcmdWithADoubleAndUnit("/bkg/backgroundWindow", this);
  bkgTimeWindow->SetGuidance("set time window for background extraction");
  bkgTimeWindow->SetUnitCategory("Time");
  bkgTimeWindow->SetUnitCandidates("s us ms ns");

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{
  delete GHEPInputFile;
  delete GHEPEvtStartIdx;
  delete USEGENIE;
  delete GENIEGeneratorDir;

  delete HepMCGeneratorDir;
  delete USEHepMC;
  delete HepMCInputFile;
  delete HepMCVtxOffset;
  
  delete USEBKG;
  delete bkgInputFile;
  delete bkgTimeWindow;
  delete bkgGeneratorDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValues)
{
  if (command == GHEPInputFile) PrimGenAction->setGenieInputFile(newValues);
  else if (command == GHEPEvtStartIdx) PrimGenAction->setGenieStartEvt(GHEPEvtStartIdx->GetNewIntValue(newValues));
  else if (command == USEGENIE) PrimGenAction->setUseGenie(USEGENIE->GetNewBoolValue(newValues));
  
  else if (command == USEHepMC) PrimGenAction->setUseHepMC(USEHepMC->GetNewBoolValue(newValues));
  else if (command == HepMCInputFile) PrimGenAction->setHepMCInputFile(newValues);
  else if (command == HepMCVtxOffset) PrimGenAction->setHepMCVtxOffset(HepMCVtxOffset->GetNew3VectorValue(newValues));

  else if (command == bkgInputFile) PrimGenAction->setBkgInputFile(newValues);
  else if (command == bkgTimeWindow) PrimGenAction->setBkgTimeWindow(bkgTimeWindow->GetNewDoubleValue(newValues));
  else if (command == USEBKG) PrimGenAction->setUseBackground(USEBKG->GetNewBoolValue(newValues));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
