#include "DetectorConstructionMessenger.hh"
#include "DetectorConstruction.hh"
#include "geometry/GeometricalParameters.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcommand.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4ThreeVector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstructionMessenger::DetectorConstructionMessenger(DetectorConstruction* manager) 
  : det(manager) {
    detDir = new G4UIdirectory("/det/");
    detDir->SetGuidance("detector control");
 
    // GENERAL OPTIONS  
    detGdmlCmd = new G4UIcmdWithABool("/det/saveGdml", this);
    detGdmlCmd->SetParameterName("saveGdml", true);
    detGdmlCmd->SetDefaultValue(false);
    detCheckOverlapCmd = new G4UIcmdWithABool("/det/checkOverlap", this);
    detCheckOverlapCmd->SetParameterName("checkOverlap", true);
    detCheckOverlapCmd->SetDefaultValue(false);
    
    // FLArE 
    detAddFLArECmd = new G4UIcmdWithABool("/det/addFLArE", this);
    detAddFLArECmd->SetParameterName("Add FLArE detector", true);
    detAddFLArECmd->SetDefaultValue(true);
    detFLArEPosCmd = new G4UIcmdWith3VectorAndUnit("/det/addFLArEPos", this);
    detFLArEPosCmd->SetParameterName("x", "y", "z", false, false);
    detFLArEPosCmd->SetDefaultValue(G4ThreeVector(0., 0., 4300.));
    detFLArEPosCmd->SetDefaultUnit("mm");
    detFLArEPosCmd->SetUnitCandidates("mm m");
    detMatCmd = new G4UIcmdWithAString("/det/material", this);
    detMatCmd->SetGuidance("set material of the detector: LAr, LKr");
    detGeomCmd = new G4UIcmdWithAString("/det/module", this);
    detGeomCmd->SetGuidance("set module option of the detector: single, 3x7");
    detFieldCmd = new G4UIcmdWithADoubleAndUnit("/det/field", this);
    detFieldCmd->SetUnitCategory("Magnetic flux density");
    detFieldCmd->SetDefaultUnit("tesla");
    detFieldCmd->SetUnitCandidates("tesla kG G");
    detFieldCmd->SetDefaultValue(1.0);
    // FORMOSA
    detAddFORMOSACmd = new G4UIcmdWithABool("/det/addFORMOSA", this);
    detAddFORMOSACmd->SetParameterName("Add FORMOSA detector", true);
    detAddFORMOSACmd->SetDefaultValue(true);
    detFORMOSAPosCmd = new G4UIcmdWith3VectorAndUnit("/det/addFORMOSAPos", this);
    detFORMOSAPosCmd->SetParameterName("x", "y", "z", false, false);
    detFORMOSAPosCmd->SetDefaultValue(G4ThreeVector(0., 0., 13870.));
    detFORMOSAPosCmd->SetDefaultUnit("mm");
    detFORMOSAPosCmd->SetUnitCandidates("mm m");
    // FASERnu2
    detAddFASERnu2Cmd = new G4UIcmdWithABool("/det/addFASERnu2", this);
    detAddFASERnu2Cmd->SetParameterName("Add FASERnu2 detector", true);
    detAddFASERnu2Cmd->SetDefaultValue(true);
    detFASERnu2PosCmd = new G4UIcmdWith3VectorAndUnit("/det/addFASERnu2Pos", this);
    detFASERnu2PosCmd->SetParameterName("x", "y", "z", false, false);
    detFASERnu2PosCmd->SetDefaultValue(G4ThreeVector(0., 0., 22023.));
    detFASERnu2PosCmd->SetDefaultUnit("mm");
    detFASERnu2PosCmd->SetUnitCandidates("mm m");
    // FASER2 SPECTROMETER MAGNET
    detAddFASER2Cmd = new G4UIcmdWithABool("/det/addFASER2", this);
    detAddFASER2Cmd->SetParameterName("Add FASER2 Spectrometer", true);
    detAddFASER2Cmd->SetDefaultValue(true);
    detFASER2PosCmd = new G4UIcmdWith3VectorAndUnit("/det/addFASER2Pos", this);
    detFASER2PosCmd->SetParameterName("x", "y", "z", false, false);
    detFASER2PosCmd->SetDefaultValue(G4ThreeVector(0., 0., 42636.));
    detFASER2PosCmd->SetDefaultUnit("mm");
    detFASER2PosCmd->SetUnitCandidates("mm m");
    magnetGeomCmd = new G4UIcmdWithAString("/det/magnetGeom", this);
    magnetGeomCmd->SetGuidance("Set the magnet design option");
    magnetGeomCmd->SetCandidates("SAMURAI CrystalPulling");
    magnetGeomCmd->SetDefaultValue("SAMURAI");
    magnetFieldCmd = new G4UIcmdWithADoubleAndUnit("/det/magnetField", this);
    magnetFieldCmd->SetUnitCategory("Magnetic flux density");
    magnetFieldCmd->SetDefaultUnit("tesla");
    magnetFieldCmd->SetUnitCandidates("tesla kG G");
    magnetFieldCmd->SetDefaultValue(1.0);
    // SAMURAI design
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
    // CrystalPullign design
    magnetInnerRCmd = new G4UIcmdWithADoubleAndUnit("/det/magnetInnerR", this);
    magnetInnerRCmd->SetUnitCategory("Length");
    magnetInnerRCmd->SetDefaultUnit("m");
    magnetInnerRCmd->SetUnitCandidates("cm m mm");
    magnetInnerRCmd->SetDefaultValue(0.8);
    magnetOuterRCmd = new G4UIcmdWithADoubleAndUnit("/det/magnetOuterR", this);
    magnetOuterRCmd->SetUnitCategory("Length");
    magnetOuterRCmd->SetDefaultUnit("m");
    magnetOuterRCmd->SetUnitCandidates("cm m mm");
    magnetOuterRCmd->SetDefaultValue(1.2);
    magnetLengthZCmd = new G4UIcmdWithADoubleAndUnit("/det/magnetLengthZ", this);
    magnetLengthZCmd->SetUnitCategory("Length");
    magnetLengthZCmd->SetDefaultUnit("m");
    magnetLengthZCmd->SetUnitCandidates("cm m mm");
    magnetLengthZCmd->SetDefaultValue(1.25);
    magnetGapCmd = new G4UIcmdWithADoubleAndUnit("/det/magnetGap", this);
    magnetGapCmd->SetUnitCategory("Length");
    magnetGapCmd->SetDefaultUnit("m");
    magnetGapCmd->SetUnitCandidates("cm m mm");
    magnetGapCmd->SetDefaultValue(0.5);
    magnetNumberCmd = new G4UIcmdWithAnInteger("/det/magnetNumber", this);
    magnetNumberCmd->SetDefaultValue(3);
    // Tracking stations
    trackingNumberCmd = new G4UIcmdWithAnInteger("/det/trackingNumber", this);
    trackingNumberCmd->SetDefaultValue(6);
    trackingNBarsYCmd = new G4UIcmdWithAnInteger("/det/trackingNBarsY", this);
    trackingNBarsYCmd->SetDefaultValue(3);
    trackingNBarsXCmd = new G4UIcmdWithAnInteger("/det/trackingNBarsX", this);
    trackingNBarsXCmd->SetDefaultValue(7);
    trackingScinThickCmd = new G4UIcmdWithADoubleAndUnit("/det/trackingScinThick", this);
    trackingScinThickCmd->SetUnitCategory("Length");
    trackingScinThickCmd->SetDefaultUnit("cm");
    trackingScinThickCmd->SetUnitCandidates("cm m mm");
    trackingScinThickCmd->SetDefaultValue(1.0);
    trackingGapCmd = new G4UIcmdWithADoubleAndUnit("/det/trackingGap", this);
    trackingGapCmd->SetUnitCategory("Length");
    trackingGapCmd->SetDefaultUnit("m");
    trackingGapCmd->SetUnitCandidates("cm m mm");
    trackingGapCmd->SetDefaultValue(0.5);
    // Sampling Calorimeter (A NuTeV-like detector) 
    detAddSamplingCaloCmd = new G4UIcmdWithABool("/det/addSamplingCalo", this);
    detAddSamplingCaloCmd->SetParameterName("Add a Sampling Calorimeter", true);
    detAddSamplingCaloCmd->SetDefaultValue(false);
    detSamplingCaloPosCmd = new G4UIcmdWith3VectorAndUnit("/det/addSamplingCaloPos", this);
    detSamplingCaloPosCmd->SetParameterName("x", "y", "z", false, false);
    detSamplingCaloPosCmd->SetDefaultValue(G4ThreeVector(0., 0., 495.));
    detSamplingCaloPosCmd->SetDefaultUnit("mm");
    detSamplingCaloPosCmd->SetUnitCandidates("mm m");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstructionMessenger::~DetectorConstructionMessenger() {
  delete detGdmlCmd;
  delete detCheckOverlapCmd;
  delete detAddFLArECmd;
  delete detMatCmd;
  delete detGeomCmd;
  delete detFieldCmd;

  delete detAddFORMOSACmd;

  delete magnetGeomCmd;
  delete magnetFieldCmd;
  delete magnetWinXCmd;
  delete magnetWinYCmd;
  delete magnetWinZCmd;
  delete yokeThickXCmd;
  delete yokeThickYCmd;
    
  delete magnetInnerRCmd;
  delete magnetOuterRCmd;
  delete magnetLengthZCmd;
  delete magnetGapCmd;
  delete magnetNumberCmd;

  delete trackingNumberCmd;
  delete trackingNBarsYCmd;
  delete trackingNBarsXCmd;
  delete trackingScinThickCmd;
  delete trackingGapCmd;

  delete detAddSamplingCaloCmd;
  delete detSamplingCaloPosCmd;

  delete detDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstructionMessenger::SetNewValue(G4UIcommand* command, G4String newValues) {
  
  // flare
  if (command == detGdmlCmd) det->SaveGDML(detGdmlCmd->GetNewBoolValue(newValues));
  else if (command == detCheckOverlapCmd) det->CheckDetOverlap(detCheckOverlapCmd->GetNewBoolValue(newValues));
  else if (command == detAddFLArECmd) det->AddFLArE(detAddFLArECmd->GetNewBoolValue(newValues));
  else if (command == detFLArEPosCmd) GeometricalParameters::Get()->SetFLArEPosition(detFLArEPosCmd->GetNew3VectorValue(newValues));
  else if (command == detGeomCmd) {
    GeometricalParameters::Get()->SetTPCConfigOption(GeometricalParameters::Get()->ConvertStringToTPCConfigOption(newValues));
  }
  else if (command == detMatCmd)  {
    GeometricalParameters::Get()->SetTPCMaterialOption(GeometricalParameters::Get()->ConvertStringToTPCMaterialOption(newValues));
  }
  else if (command == detFieldCmd) det->SetFieldValue(detFieldCmd->ConvertToDimensionedDouble(newValues));
  // FORMOSA
  else if (command == detAddFORMOSACmd) det->AddFORMOSA(detAddFORMOSACmd->GetNewBoolValue(newValues));
  else if (command == detFORMOSAPosCmd) GeometricalParameters::Get()->SetFORMOSAPosition(detFORMOSAPosCmd->GetNew3VectorValue(newValues));
  // FASERnu2
  else if (command == detAddFASERnu2Cmd) det->AddFASERnu2(detAddFASERnu2Cmd->GetNewBoolValue(newValues));
  else if (command == detFASERnu2PosCmd) GeometricalParameters::Get()->SetFASERnu2Position(detFASERnu2PosCmd->GetNew3VectorValue(newValues));
  // faser2 magnet
  else if (command == detAddFASER2Cmd) det->AddFASER2(detAddFASER2Cmd->GetNewBoolValue(newValues));
  else if (command == detFASER2PosCmd) GeometricalParameters::Get()->SetFASER2Position(detFASER2PosCmd->GetNew3VectorValue(newValues));
  else if (command == magnetGeomCmd)  
    GeometricalParameters::Get()->SetSpectrometerMagnetOption(GeometricalParameters::Get()->ConvertStringToMagnetOption(newValues));
  else if (command == magnetFieldCmd) 
    GeometricalParameters::Get()->SetSpectrometerMagnetField(magnetFieldCmd->ConvertToDimensionedDouble(newValues));
  else if (command == magnetWinXCmd) 
    GeometricalParameters::Get()->SetSpectrometerMagnetWindowX(magnetWinXCmd->ConvertToDimensionedDouble(newValues));
  else if (command == magnetWinYCmd) 
    GeometricalParameters::Get()->SetSpectrometerMagnetWindowY(magnetWinYCmd->ConvertToDimensionedDouble(newValues));
  else if (command == magnetWinZCmd) 
    GeometricalParameters::Get()->SetSpectrometerMagnetWindowZ(magnetWinZCmd->ConvertToDimensionedDouble(newValues));
  else if (command == yokeThickXCmd) 
    GeometricalParameters::Get()->SetSpectrometerMagnetYokeThickX(yokeThickXCmd->ConvertToDimensionedDouble(newValues));
  else if (command == yokeThickYCmd) 
    GeometricalParameters::Get()->SetSpectrometerMagnetYokeThickY(yokeThickYCmd->ConvertToDimensionedDouble(newValues));
  else if (command == magnetInnerRCmd) 
    GeometricalParameters::Get()->SetSpectrometerMagnetInnerR(magnetInnerRCmd->ConvertToDimensionedDouble(newValues));
  else if (command == magnetOuterRCmd) 
    GeometricalParameters::Get()->SetSpectrometerMagnetOuterR(magnetOuterRCmd->ConvertToDimensionedDouble(newValues));
  else if (command == magnetLengthZCmd) 
    GeometricalParameters::Get()->SetSpectrometerMagnetLengthZ(magnetLengthZCmd->ConvertToDimensionedDouble(newValues));
  else if (command == magnetGapCmd) 
    GeometricalParameters::Get()->SetSpectrometerMagnetGap(magnetGapCmd->ConvertToDimensionedDouble(newValues));
  else if (command == magnetNumberCmd) 
    GeometricalParameters::Get()->SetNSpectrometerMagnets(magnetNumberCmd->GetNewIntValue(newValues));

  //faser2 tracking stations
  else if (command == trackingNumberCmd) 
    GeometricalParameters::Get()->SetNTrackingStations(trackingNumberCmd->GetNewIntValue(newValues));
  else if (command == trackingNBarsYCmd) 
    GeometricalParameters::Get()->SetNScintillatorBarsY(trackingNBarsYCmd->GetNewIntValue(newValues));
  else if (command == trackingNBarsXCmd) 
    GeometricalParameters::Get()->SetNScintillatorBarsX(trackingNBarsXCmd->GetNewIntValue(newValues));
  else if (command == trackingScinThickCmd) 
    GeometricalParameters::Get()->SetScintillatorThickness(trackingScinThickCmd->ConvertToDimensionedDouble(newValues));
  else if (command == trackingGapCmd) 
    GeometricalParameters::Get()->SetTrackingStationGap(trackingGapCmd->ConvertToDimensionedDouble(newValues));

  // sampling calorimeter
  else if (command == detAddSamplingCaloCmd) det->AddSamplingCalorimeter(detAddSamplingCaloCmd->GetNewBoolValue(newValues));
  else if (command == detSamplingCaloPosCmd) GeometricalParameters::Get()->SetSamplingCaloPosition(detSamplingCaloPosCmd->GetNew3VectorValue(newValues));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
