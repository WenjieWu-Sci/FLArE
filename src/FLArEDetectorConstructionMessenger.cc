#include "FLArEDetectorConstructionMessenger.hh"
#include "FLArEDetectorConstruction.hh"
#include "geometry/GeometricalParameters.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcommand.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FLArEDetectorConstructionMessenger::FLArEDetectorConstructionMessenger(FLArEDetectorConstruction* manager) 
  : det(manager) {
    detDir = new G4UIdirectory("/det/");
    detDir->SetGuidance("detector control");
 
    // GENERAL OPTIONS  
    detGdmlCmd = new G4UIcmdWithABool("/det/saveGdml", this);
    detGdmlCmd->SetParameterName("saveGdml", true);
    detGdmlCmd->SetDefaultValue(false);
    
    // FLARE 
    detMatCmd = new G4UIcmdWithAString("/det/material", this);
    detMatCmd->SetGuidance("set material of the detector: LAr, LKr");

    detGeomCmd = new G4UIcmdWithAString("/det/module", this);
    detGeomCmd->SetGuidance("set module option of the detector: single, 3x7");

    detFieldCmd = new G4UIcmdWithADoubleAndUnit("/det/field", this);
    detFieldCmd->SetUnitCategory("Magnetic flux density");
    detFieldCmd->SetDefaultUnit("tesla");
    detFieldCmd->SetUnitCandidates("tesla kG G");
    detFieldCmd->SetDefaultValue(1.0);

    // FASER2 SPECTROMETER MAGNET
    magnetGeomCmd = new G4UIcmdWithAString("/det/magnetGeom", this);
    magnetGeomCmd->SetGuidance("Set the magnet design option");
    magnetGeomCmd->SetCandidates("SAMURAI CrystalPulling");
    magnetGeomCmd->SetDefaultValue("SAMURAI");

    magnetFieldCmd = new G4UIcmdWithADoubleAndUnit("/det/magnetField", this);
    magnetFieldCmd->SetUnitCategory("Magnetic flux density");
    magnetFieldCmd->SetDefaultUnit("tesla");
    magnetFieldCmd->SetUnitCandidates("tesla kG G");
    magnetFieldCmd->SetDefaultValue(1.0);
    
    magnetWinXCmd = new G4UIcmdWithADoubleAndUnit("/det/magnetWinX", this);
    magnetWinXCmd->SetUnitCategory("Length");
    magnetWinXCmd->SetDefaultUnit("m");
    magnetWinXCmd->SetUnitCandidates("cm m mm");
    magnetWinXCmd->SetDefaultValue(3.0);
    
    magnetWinYCmd = new G4UIcmdWithADoubleAndUnit("/det/magnetWinY", this);
    magnetWinYCmd->SetUnitCategory("Length");
    magnetWinYCmd->SetDefaultUnit("m");
    magnetWinYCmd->SetUnitCandidates("cm m mm");
    magnetWinYCmd->SetDefaultValue(1.0);
    
    magnetWinZCmd = new G4UIcmdWithADoubleAndUnit("/det/magnetWinZ", this);
    magnetWinZCmd->SetUnitCategory("Length");
    magnetWinZCmd->SetDefaultUnit("m");
    magnetWinZCmd->SetUnitCandidates("cm m mm");
    magnetWinZCmd->SetDefaultValue(4.0);
    
    yokeThickXCmd = new G4UIcmdWithADoubleAndUnit("/det/yokeThickX", this);
    yokeThickXCmd->SetUnitCategory("Length");
    yokeThickXCmd->SetDefaultUnit("m");
    yokeThickXCmd->SetUnitCandidates("cm m mm");
    yokeThickXCmd->SetDefaultValue(1.5);
    
    yokeThickYCmd = new G4UIcmdWithADoubleAndUnit("/det/yokeThickY", this);
    yokeThickYCmd->SetUnitCategory("Length");
    yokeThickYCmd->SetDefaultUnit("m");
    yokeThickYCmd->SetUnitCandidates("cm m mm");
    yokeThickYCmd->SetDefaultValue(2.0);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FLArEDetectorConstructionMessenger::~FLArEDetectorConstructionMessenger() {
  delete detMatCmd;
  delete detGeomCmd;
  delete detGdmlCmd;
  delete detFieldCmd;

  delete magnetGeomCmd;
  delete magnetFieldCmd;
  delete magnetWinXCmd;
  delete magnetWinYCmd;
  delete magnetWinZCmd;
  delete yokeThickXCmd;
  delete yokeThickYCmd;

  delete detDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FLArEDetectorConstructionMessenger::SetNewValue(G4UIcommand* command, G4String newValues) {
  
  // general
  if (command == detGeomCmd) det->SetGeomOption(newValues);
  
  // flare
  if (command == detMatCmd)  det->SetDetMaterial(newValues);
  if (command == detGdmlCmd) det->saveGDML(detGdmlCmd->GetNewBoolValue(newValues));
  if (command == detFieldCmd) det->SetFieldValue(detFieldCmd->ConvertToDimensionedDouble(newValues));

  // faser2 magnet
  if (command == magnetGeomCmd)  GeometricalParameters::Get()->SetSpectrometerMagnetOption(newValues);
  if (command == magnetFieldCmd) GeometricalParameters::Get()->SetSpectrometerMagnetField(magnetFieldCmd->ConvertToDimensionedDouble(newValues));
  if (command == magnetWinXCmd) GeometricalParameters::Get()->SetSpectrometerMagnetWindowX(magnetWinXCmd->ConvertToDimensionedDouble(newValues));
  if (command == magnetWinYCmd) GeometricalParameters::Get()->SetSpectrometerMagnetWindowY(magnetWinYCmd->ConvertToDimensionedDouble(newValues));
  if (command == magnetWinZCmd) GeometricalParameters::Get()->SetSpectrometerMagnetWindowZ(magnetWinZCmd->ConvertToDimensionedDouble(newValues));
  if (command == yokeThickXCmd) GeometricalParameters::Get()->SetSpectrometerMagnetYokeThickX(yokeThickXCmd->ConvertToDimensionedDouble(newValues));
  if (command == yokeThickYCmd) GeometricalParameters::Get()->SetSpectrometerMagnetYokeThickY(yokeThickYCmd->ConvertToDimensionedDouble(newValues));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
