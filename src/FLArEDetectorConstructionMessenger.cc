#include "FLArEDetectorConstructionMessenger.hh"
#include "LArBoxDetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcommand.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FLArEDetectorConstructionMessenger::FLArEDetectorConstructionMessenger(LArBoxDetectorConstruction* manager) 
  : det(manager) {
    detDir = new G4UIdirectory("/det/");
    detDir->SetGuidance("detector control");
    
    detMatCmd = new G4UIcmdWithAString("/det/material", this);
    detMatCmd->SetGuidance("set material of the detector: LAr, LKr");

//    updateCmd = new G4UIcommand("/det/update", this);
//    updateCmd->SetGuidance("update the detector geometry with changed value");
//    updateCmd->SetGuidance("must be run before beamOn if detector has been changed");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FLArEDetectorConstructionMessenger::~FLArEDetectorConstructionMessenger() {
  delete detMatCmd;
//  delete updateCmd;
  delete detDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FLArEDetectorConstructionMessenger::SetNewValue(G4UIcommand* command, G4String newValues) {
  if (command == detMatCmd) det->SetDetMaterial(newValues);
//  if (command == updateCmd) det->UpdateGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
