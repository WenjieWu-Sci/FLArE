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
 
    // -------------------------
    // GENERAL OPTIONS  
    detDir = new G4UIdirectory("/det/");
    detDir->SetGuidance("Detector control");
    
    // saving gdml
    detGdmlCmd = new G4UIcmdWithABool("/det/saveGdml", this);
    detGdmlCmd->SetParameterName("saveGdml", true);
    detGdmlCmd->SetDefaultValue(false);
    // check overlap
    detCheckOverlapCmd = new G4UIcmdWithABool("/det/checkOverlap", this);
    detCheckOverlapCmd->SetParameterName("checkOverlap", true);
    detCheckOverlapCmd->SetDefaultValue(false);
    // add FLARE volume
    detAddFLArECmd = new G4UIcmdWithABool("/det/addFLArE", this);
    detAddFLArECmd->SetParameterName("Add FLArE detector", true);
    detAddFLArECmd->SetDefaultValue(true);
    // add FORMOSA volume
    detAddFORMOSACmd = new G4UIcmdWithABool("/det/addFORMOSA", this);
    detAddFORMOSACmd->SetParameterName("Add FORMOSA detector", true);
    detAddFORMOSACmd->SetDefaultValue(true);
    // add FASERnu2 volume
    detAddFASERnu2Cmd = new G4UIcmdWithABool("/det/addFASERnu2", this);
    detAddFASERnu2Cmd->SetParameterName("Add FASERnu2 detector", true);
    detAddFASERnu2Cmd->SetDefaultValue(true);
    // add FASER2 volume
    detAddFASER2Cmd = new G4UIcmdWithABool("/det/addFASER2", this);
    detAddFASER2Cmd->SetParameterName("Add FASER2 Spectrometer", true);
    detAddFASER2Cmd->SetDefaultValue(true);
    
    // -------------------------
    // FLARE COMMANDS
    flareDir = new G4UIdirectory("/det/flare/");
    flareDir->SetGuidance("FLArE Detector control");
    
    // FLArE position
    flarePosCmd = new G4UIcmdWith3VectorAndUnit("/det/flare/addFLArEPos", this);
    flarePosCmd->SetParameterName("x", "y", "z", false, false);
    flarePosCmd->SetUnitCandidates("mm m");
    // FLArE material
    flareMatCmd = new G4UIcmdWithAString("/det/flare/material", this);
    flareMatCmd->SetGuidance("set material of the detector: LAr, LKr");
    // TPC geometry option
    flareGeomCmd = new G4UIcmdWithAString("/det/flare/module", this);
    flareGeomCmd->SetGuidance("set module option of the detector: single, 3x7");
    // FLArE magnetic field (for HadCatcher+MuonFinder)
    flareFieldCmd = new G4UIcmdWithADoubleAndUnit("/det/flare/field", this);
    flareFieldCmd->SetGuidance("set magnetic field for HadCatcher+MuonFinder");
    flareFieldCmd->SetUnitCategory("Magnetic flux density");
    flareFieldCmd->SetUnitCandidates("tesla kG G");
    // use BabyMIND (instead of for HadCatcher+MuonFinder)
    flareUseBabyMINDCmd = new G4UIcmdWithABool("/det/flare/useBabyMIND", this);
    flareUseBabyMINDCmd->SetParameterName("Use BabyMIND detector", true);
    flareUseBabyMINDCmd->SetDefaultValue(false);
    
    // -------------------------
    // BABYMIND COMMANDS
    babymindDir = new G4UIdirectory("/det/babymind/");
    babymindDir->SetGuidance("BabyMIND Detector control");

    // magnet plate thickness
    babymindMagnetPlateThickCmd = new G4UIcmdWithADoubleAndUnit("/det/babymind/magnetPlateThickness", this);
    babymindMagnetPlateThickCmd->SetUnitCategory("Length");
    babymindMagnetPlateThickCmd->SetUnitCandidates("cm m mm");
    // magnet plate size x/y
    babymindMagnetPlateSizeXCmd = new G4UIcmdWithADoubleAndUnit("/det/babymind/magnetPlateSizeX", this);
    babymindMagnetPlateSizeXCmd->SetUnitCategory("Length");
    babymindMagnetPlateSizeXCmd->SetUnitCandidates("cm m mm");
    babymindMagnetPlateSizeYCmd = new G4UIcmdWithADoubleAndUnit("/det/babymind/magnetPlateSizeY", this);
    babymindMagnetPlateSizeYCmd->SetUnitCategory("Length");
    babymindMagnetPlateSizeYCmd->SetUnitCandidates("cm m mm");
    // central magnet plate height (y)
    babymindMagnetCentralPlateYCmd = new G4UIcmdWithADoubleAndUnit("/det/babymind/magnetCentralPlateY", this);
    babymindMagnetCentralPlateYCmd->SetUnitCategory("Length");
    babymindMagnetCentralPlateYCmd->SetUnitCandidates("cm m mm");
    // slit size x/y
    babymindSlitSizeXCmd = new G4UIcmdWithADoubleAndUnit("/det/babymind/slitSizeX",this);
    babymindSlitSizeXCmd->SetUnitCategory("Length");
    babymindSlitSizeXCmd->SetUnitCandidates("cm m mm");
    babymindSlitSizeYCmd = new G4UIcmdWithADoubleAndUnit("/det/babymind/slitSizeY",this);
    babymindSlitSizeYCmd->SetUnitCategory("Length");
    babymindSlitSizeYCmd->SetUnitCandidates("cm m mm");
    // magnetic field in magnet plate
    babymindFieldStrengthCmd = new G4UIcmdWithADoubleAndUnit("/det/babymind/fieldStrength",this);
    babymindFieldStrengthCmd->SetUnitCategory("Magnetic flux density");
    babymindFieldStrengthCmd->SetUnitCandidates("tesla kG G");
    // bar thickness
    babymindBarThicknessCmd = new G4UIcmdWithADoubleAndUnit("/det/babymind/barThickness",this);
    babymindBarThicknessCmd->SetUnitCategory("Length");
    babymindBarThicknessCmd->SetUnitCandidates("cm m mm");
    // vertical bars
    babymindNVerticalBarsCmd = new G4UIcmdWithAnInteger("/det/babymind/verticalNbars", this);;
    babymindVerticalBarSizeXCmd = new G4UIcmdWithADoubleAndUnit("/det/babymind/verticalBarSizeX",this);
    babymindVerticalBarSizeXCmd->SetUnitCategory("Length");
    babymindVerticalBarSizeXCmd->SetUnitCandidates("cm m mm");
    babymindVerticalBarSizeYCmd = new G4UIcmdWithADoubleAndUnit("/det/babymind/verticalBarSizeY",this);
    babymindVerticalBarSizeYCmd->SetUnitCategory("Length");
    babymindVerticalBarSizeYCmd->SetUnitCandidates("cm m mm");
    // horizontal bars
    babymindNHorizontalBarsCmd = new G4UIcmdWithAnInteger("/det/babymind/horizontalNbars", this);;
    babymindHorizontalBarSizeXCmd = new G4UIcmdWithADoubleAndUnit("/det/babymind/horizontalBarSizeX",this);
    babymindHorizontalBarSizeXCmd->SetUnitCategory("Length");
    babymindHorizontalBarSizeXCmd->SetUnitCandidates("cm m mm");
    babymindHorizontalBarSizeYCmd = new G4UIcmdWithADoubleAndUnit("/det/babymind/horizontalBarSizeY",this);
    babymindHorizontalBarSizeYCmd->SetUnitCategory("Length");
    babymindHorizontalBarSizeYCmd->SetUnitCandidates("cm m mm");
    // spacing between modules/plates
    babymindMagnetToScinSpacingCmd = new G4UIcmdWithADoubleAndUnit("/det/babymind/magnetToScinSpacing",this);
    babymindMagnetToScinSpacingCmd->SetUnitCategory("Length");
    babymindMagnetToScinSpacingCmd->SetUnitCandidates("cm m mm");
    babymindMagnetToMagnetSpacingCmd = new G4UIcmdWithADoubleAndUnit("/det/babymind/magnetToMagnetSpacing",this);
    babymindMagnetToMagnetSpacingCmd->SetUnitCategory("Length");
    babymindMagnetToMagnetSpacingCmd->SetUnitCandidates("cm m mm");
    babymindBlockToBlockSpacingCmd = new G4UIcmdWithADoubleAndUnit("/det/babymind/blockToBlockSpacing",this);
    babymindBlockToBlockSpacingCmd->SetUnitCategory("Length");
    babymindBlockToBlockSpacingCmd->SetUnitCandidates("cm m mm");
    babymindBlockPaddingCmd = new G4UIcmdWithADoubleAndUnit("/det/babymind/blockPadding",this);
    babymindBlockPaddingCmd->SetUnitCategory("Length");
    babymindBlockPaddingCmd->SetUnitCandidates("cm m mm");
    // module/plates sequence
    babymindBlockSequenceCmd = new G4UIcmdWithAString("/det/babymind/blockSequence",this);
    babymindBlockSequenceCmd->SetGuidance("set modules/plates sequence for babymind");
  
    // -------------------------
    // FORMOSA COMMANDS
    formosaDir = new G4UIdirectory("/det/formosa/");
    formosaDir->SetGuidance("FORMOSA Detector control");
    
    formosaPosCmd = new G4UIcmdWith3VectorAndUnit("/det/formosa/addFORMOSAPos", this);
    formosaPosCmd->SetParameterName("x", "y", "z", false, false);
    formosaPosCmd->SetDefaultValue(G4ThreeVector(0., 0., 13870.));
    formosaPosCmd->SetDefaultUnit("mm");
    formosaPosCmd->SetUnitCandidates("mm m cm");
    
    // -------------------------
    // FASERnu2 COMMANDS
    fasernuDir = new G4UIdirectory("/det/fasernu/");
    fasernuDir->SetGuidance("FASERnu2 Detector control");
    
    fasernuPosCmd = new G4UIcmdWith3VectorAndUnit("/det/fasernu/addFASERnu2Pos", this);
    fasernuPosCmd->SetParameterName("x", "y", "z", false, false);
    fasernuPosCmd->SetUnitCandidates("mm m cm");
    
    // -------------------------
    // FASER2 SPECTROMETER MAGNET COMMANDS
    faserDir = new G4UIdirectory("/det/faser/");
    faserDir->SetGuidance("FASER2 Detector control");
    
    faserPosCmd = new G4UIcmdWith3VectorAndUnit("/det/faser/addFASER2Pos", this);
    faserPosCmd->SetParameterName("x", "y", "z", false, false);
    faserPosCmd->SetUnitCandidates("mm m cm");
    faserMagnetGeomCmd = new G4UIcmdWithAString("/det/faser/magnetGeom", this);
    faserMagnetGeomCmd->SetGuidance("Set the magnet design option");
    faserMagnetGeomCmd->SetCandidates("SAMURAI CrystalPulling");
    faserMagnetGeomCmd->SetDefaultValue("SAMURAI");
    faserMagnetFieldCmd = new G4UIcmdWithADoubleAndUnit("/det/faser/magnetField", this);
    faserMagnetFieldCmd->SetUnitCategory("Magnetic flux density");
    faserMagnetFieldCmd->SetUnitCandidates("tesla kG G");
    // SAMURAI design
    faserMagnetWinXCmd = new G4UIcmdWithADoubleAndUnit("/det/faser/magnetWinX", this);
    faserMagnetWinXCmd->SetUnitCategory("Length");
    faserMagnetWinXCmd->SetUnitCandidates("cm m mm");
    faserMagnetWinYCmd = new G4UIcmdWithADoubleAndUnit("/det/faser/magnetWinY", this);
    faserMagnetWinYCmd->SetUnitCategory("Length");
    faserMagnetWinYCmd->SetUnitCandidates("cm m mm");
    faserMagnetWinZCmd = new G4UIcmdWithADoubleAndUnit("/det/faser/magnetWinZ", this);
    faserMagnetWinZCmd->SetUnitCategory("Length");
    faserMagnetWinZCmd->SetUnitCandidates("cm m mm");
    faserYokeThickXCmd = new G4UIcmdWithADoubleAndUnit("/det/faser/yokeThickX", this);
    faserYokeThickXCmd->SetUnitCategory("Length");
    faserYokeThickXCmd->SetUnitCandidates("cm m mm");
    faserYokeThickYCmd = new G4UIcmdWithADoubleAndUnit("/det/faser/yokeThickY", this);
    faserYokeThickYCmd->SetUnitCategory("Length");
    faserYokeThickYCmd->SetUnitCandidates("cm m mm");
    // CrystalPullign design
    faserMagnetInnerRCmd = new G4UIcmdWithADoubleAndUnit("/det/faser/magnetInnerR", this);
    faserMagnetInnerRCmd->SetUnitCategory("Length");
    faserMagnetInnerRCmd->SetUnitCandidates("cm m mm");
    faserMagnetOuterRCmd = new G4UIcmdWithADoubleAndUnit("/det/faser/magnetOuterR", this);
    faserMagnetOuterRCmd->SetUnitCategory("Length");
    faserMagnetOuterRCmd->SetUnitCandidates("cm m mm");
    faserMagnetLengthZCmd = new G4UIcmdWithADoubleAndUnit("/det/faser/magnetLengthZ", this);
    faserMagnetLengthZCmd->SetUnitCategory("Length");
    faserMagnetLengthZCmd->SetUnitCandidates("cm m mm");
    faserMagnetGapCmd = new G4UIcmdWithADoubleAndUnit("/det/faser/magnetGap", this);
    faserMagnetGapCmd->SetUnitCategory("Length");
    faserMagnetGapCmd->SetUnitCandidates("cm m mm");
    faserMagnetNumberCmd = new G4UIcmdWithAnInteger("/det/faser/magnetNumber", this);
    // Tracking stations
    faserTrackingNumberCmd = new G4UIcmdWithAnInteger("/det/faser/trackingNumber", this);
    faserTrackingNBarsYCmd = new G4UIcmdWithAnInteger("/det/faser/trackingNBarsY", this);
    faserTrackingNBarsXCmd = new G4UIcmdWithAnInteger("/det/faser/trackingNBarsX", this);
    faserTrackingScinThickCmd = new G4UIcmdWithADoubleAndUnit("/det/faser/trackingScinThick", this);
    faserTrackingScinThickCmd->SetUnitCategory("Length");
    faserTrackingScinThickCmd->SetUnitCandidates("cm m mm");
    faserTrackingGapCmd = new G4UIcmdWithADoubleAndUnit("/det/faser/trackingGap", this);
    faserTrackingGapCmd->SetUnitCategory("Length");
    faserTrackingGapCmd->SetUnitCandidates("cm m mm");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstructionMessenger::~DetectorConstructionMessenger() {
  delete detGdmlCmd;
  delete detCheckOverlapCmd;
  delete detAddFLArECmd;
  delete detAddFORMOSACmd;
  delete detAddFASER2Cmd;
  delete detAddFASERnu2Cmd;

  delete flarePosCmd;
  delete flareMatCmd;
  delete flareGeomCmd;
  delete flareFieldCmd;
  delete flareUseBabyMINDCmd;
    
  delete babymindMagnetPlateThickCmd;
  delete babymindMagnetPlateSizeXCmd;
  delete babymindMagnetPlateSizeYCmd;
  delete babymindMagnetCentralPlateYCmd;
  delete babymindSlitSizeXCmd;
  delete babymindSlitSizeYCmd;
  delete babymindFieldStrengthCmd;
  delete babymindBarThicknessCmd; 
  delete babymindNVerticalBarsCmd;
  delete babymindVerticalBarSizeXCmd;
  delete babymindVerticalBarSizeYCmd;
  delete babymindNHorizontalBarsCmd; 
  delete babymindHorizontalBarSizeXCmd;
  delete babymindHorizontalBarSizeYCmd;
  delete babymindMagnetToScinSpacingCmd;
  delete babymindMagnetToMagnetSpacingCmd;
  delete babymindBlockToBlockSpacingCmd;
  delete babymindBlockPaddingCmd; 
  delete babymindBlockSequenceCmd;

  delete formosaPosCmd;
  
  delete fasernuPosCmd;

  delete faserPosCmd;
  delete faserMagnetGeomCmd;
  delete faserMagnetFieldCmd;
  delete faserMagnetWinXCmd;
  delete faserMagnetWinYCmd;
  delete faserMagnetWinZCmd;
  delete faserYokeThickXCmd;
  delete faserYokeThickYCmd;
  delete faserMagnetInnerRCmd;
  delete faserMagnetOuterRCmd;
  delete faserMagnetLengthZCmd;
  delete faserMagnetGapCmd;
  delete faserMagnetNumberCmd;
  delete faserTrackingNumberCmd;
  delete faserTrackingNBarsYCmd;
  delete faserTrackingNBarsXCmd;
  delete faserTrackingScinThickCmd;
  delete faserTrackingGapCmd;

  delete detDir;
  delete flareDir;
  delete babymindDir;
  delete formosaDir;
  delete faserDir;
  delete fasernuDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstructionMessenger::SetNewValue(G4UIcommand* command, G4String newValues) {
 
  // GENERAL COMMANDS
  if (command == detGdmlCmd) det->SaveGDML(detGdmlCmd->GetNewBoolValue(newValues));
  else if (command == detCheckOverlapCmd) det->CheckDetOverlap(detCheckOverlapCmd->GetNewBoolValue(newValues));
  else if (command == detAddFLArECmd) det->AddFLArE(detAddFLArECmd->GetNewBoolValue(newValues)); 
  else if (command == detAddFORMOSACmd) det->AddFORMOSA(detAddFORMOSACmd->GetNewBoolValue(newValues));
  else if (command == detAddFASERnu2Cmd) det->AddFASERnu2(detAddFASERnu2Cmd->GetNewBoolValue(newValues));
  else if (command == detAddFASER2Cmd) det->AddFASER2(detAddFASER2Cmd->GetNewBoolValue(newValues));
  
  // FLARE COMMANDS
  else if (command == flarePosCmd)
    GeometricalParameters::Get()->SetFLArEPosition(flarePosCmd->GetNew3VectorValue(newValues));
  else if (command == flareGeomCmd)
    GeometricalParameters::Get()->SetTPCConfigOption(GeometricalParameters::Get()->ConvertStringToTPCConfigOption(newValues));
  else if (command == flareMatCmd)
    GeometricalParameters::Get()->SetTPCMaterialOption(GeometricalParameters::Get()->ConvertStringToTPCMaterialOption(newValues));
  else if (command == flareFieldCmd) 
    det->SetFieldValue(flareFieldCmd->ConvertToDimensionedDouble(newValues));
  else if (command == flareUseBabyMINDCmd){
    det->UseBabyMIND(flareUseBabyMINDCmd->GetNewBoolValue(newValues));
    GeometricalParameters::Get()->SetUseBabyMIND(flareUseBabyMINDCmd->GetNewBoolValue(newValues));
  }

  //BABYMIND COMMANDS
  else if (command == babymindMagnetPlateThickCmd)
    GeometricalParameters::Get()->SetBabyMINDMagnetPlateThickness(babymindMagnetPlateThickCmd->ConvertToDimensionedDouble(newValues));
  else if (command == babymindMagnetPlateSizeXCmd)
    GeometricalParameters::Get()->SetBabyMINDMagnetPlateSizeX(babymindMagnetPlateSizeXCmd->ConvertToDimensionedDouble(newValues));
  else if (command == babymindMagnetPlateSizeYCmd)
    GeometricalParameters::Get()->SetBabyMINDMagnetPlateSizeY(babymindMagnetPlateSizeYCmd->ConvertToDimensionedDouble(newValues));
  else if (command == babymindMagnetCentralPlateYCmd)
    GeometricalParameters::Get()->SetBabyMINDMagnetCentralPlateY(babymindMagnetCentralPlateYCmd->ConvertToDimensionedDouble(newValues));
  else if (command == babymindSlitSizeXCmd)
    GeometricalParameters::Get()->SetBabyMINDSlitSizeX(babymindSlitSizeXCmd->ConvertToDimensionedDouble(newValues));
  else if (command == babymindSlitSizeYCmd)
    GeometricalParameters::Get()->SetBabyMINDSlitSizeY(babymindSlitSizeYCmd->ConvertToDimensionedDouble(newValues));
  else if (command == babymindFieldStrengthCmd)
    GeometricalParameters::Get()->SetBabyMINDFieldStrength(babymindFieldStrengthCmd->ConvertToDimensionedDouble(newValues));
  else if (command == babymindBarThicknessCmd)
    GeometricalParameters::Get()->SetBabyMINDBarThickness(babymindBarThicknessCmd->ConvertToDimensionedDouble(newValues));  
  else if (command == babymindNVerticalBarsCmd)
    GeometricalParameters::Get()->SetBabyMINDNVerticalBars(babymindNVerticalBarsCmd->GetNewIntValue(newValues));
  else if (command == babymindNHorizontalBarsCmd)
    GeometricalParameters::Get()->SetBabyMINDNHorizontalBars(babymindNHorizontalBarsCmd->GetNewIntValue(newValues));
  else if (command == babymindVerticalBarSizeXCmd)
    GeometricalParameters::Get()->SetBabyMINDVerticalBarSizeX(babymindVerticalBarSizeXCmd->ConvertToDimensionedDouble(newValues));
  else if (command == babymindVerticalBarSizeYCmd)
    GeometricalParameters::Get()->SetBabyMINDVerticalBarSizeY(babymindVerticalBarSizeYCmd->ConvertToDimensionedDouble(newValues));
  else if (command == babymindHorizontalBarSizeXCmd)
    GeometricalParameters::Get()->SetBabyMINDHorizontalBarSizeX(babymindHorizontalBarSizeXCmd->ConvertToDimensionedDouble(newValues));
  else if (command == babymindHorizontalBarSizeYCmd)
    GeometricalParameters::Get()->SetBabyMINDHorizontalBarSizeY(babymindHorizontalBarSizeYCmd->ConvertToDimensionedDouble(newValues));
  else if (command == babymindMagnetToScinSpacingCmd)
    GeometricalParameters::Get()->SetBabyMINDMagnetToScinSpacing(babymindMagnetToScinSpacingCmd->ConvertToDimensionedDouble(newValues));
  else if (command == babymindMagnetToMagnetSpacingCmd)
    GeometricalParameters::Get()->SetBabyMINDMagnetToMagnetSpacing(babymindMagnetToMagnetSpacingCmd->ConvertToDimensionedDouble(newValues));
  else if (command == babymindBlockToBlockSpacingCmd)
    GeometricalParameters::Get()->SetBabyMINDBlockToBlockSpacing(babymindBlockToBlockSpacingCmd->ConvertToDimensionedDouble(newValues));
  else if (command == babymindBlockPaddingCmd)
    GeometricalParameters::Get()->SetBabyMINDBlockPadding(babymindBlockPaddingCmd->ConvertToDimensionedDouble(newValues));
  else if (command == babymindBlockSequenceCmd)
    GeometricalParameters::Get()->SetBabyMINDBlockSequence(newValues);

  // FORMOSA COMMANDS
  else if (command == formosaPosCmd) 
    GeometricalParameters::Get()->SetFORMOSAPosition(formosaPosCmd->GetNew3VectorValue(newValues));
  
  // FASERnu2 COMMANDS
  else if (command == fasernuPosCmd)
   GeometricalParameters::Get()->SetFASERnu2Position(fasernuPosCmd->GetNew3VectorValue(newValues));
  
  // FASER2 COMMANDS
  else if (command == faserPosCmd) 
    GeometricalParameters::Get()->SetFASER2Position(faserPosCmd->GetNew3VectorValue(newValues));
  else if (command == faserMagnetGeomCmd)  
    GeometricalParameters::Get()->SetFASER2MagnetOption(GeometricalParameters::Get()->ConvertStringToMagnetOption(newValues));
  else if (command == faserMagnetFieldCmd) 
    GeometricalParameters::Get()->SetFASER2MagnetField(faserMagnetFieldCmd->ConvertToDimensionedDouble(newValues));
  else if (command == faserMagnetWinXCmd) 
    GeometricalParameters::Get()->SetFASER2MagnetWindowX(faserMagnetWinXCmd->ConvertToDimensionedDouble(newValues));
  else if (command == faserMagnetWinYCmd) 
    GeometricalParameters::Get()->SetFASER2MagnetWindowY(faserMagnetWinYCmd->ConvertToDimensionedDouble(newValues));
  else if (command == faserMagnetWinZCmd) 
    GeometricalParameters::Get()->SetFASER2MagnetWindowZ(faserMagnetWinZCmd->ConvertToDimensionedDouble(newValues));
  else if (command == faserYokeThickXCmd) 
    GeometricalParameters::Get()->SetFASER2MagnetYokeThickX(faserYokeThickXCmd->ConvertToDimensionedDouble(newValues));
  else if (command == faserYokeThickYCmd) 
    GeometricalParameters::Get()->SetFASER2MagnetYokeThickY(faserYokeThickYCmd->ConvertToDimensionedDouble(newValues));
  else if (command == faserMagnetInnerRCmd) 
    GeometricalParameters::Get()->SetFASER2MagnetInnerR(faserMagnetInnerRCmd->ConvertToDimensionedDouble(newValues));
  else if (command == faserMagnetOuterRCmd) 
    GeometricalParameters::Get()->SetFASER2MagnetOuterR(faserMagnetOuterRCmd->ConvertToDimensionedDouble(newValues));
  else if (command == faserMagnetLengthZCmd) 
    GeometricalParameters::Get()->SetFASER2MagnetLengthZ(faserMagnetLengthZCmd->ConvertToDimensionedDouble(newValues));
  else if (command == faserMagnetGapCmd) 
    GeometricalParameters::Get()->SetFASER2MagnetGap(faserMagnetGapCmd->ConvertToDimensionedDouble(newValues));
  else if (command == faserMagnetNumberCmd) 
    GeometricalParameters::Get()->SetNFASER2Magnets(faserMagnetNumberCmd->GetNewIntValue(newValues));
  //faser2 tracking stations
  else if (command == faserTrackingNumberCmd) 
    GeometricalParameters::Get()->SetNTrackingStations(faserTrackingNumberCmd->GetNewIntValue(newValues));
  else if (command == faserTrackingNBarsYCmd) 
    GeometricalParameters::Get()->SetNScintillatorBarsY(faserTrackingNBarsYCmd->GetNewIntValue(newValues));
  else if (command == faserTrackingNBarsXCmd) 
    GeometricalParameters::Get()->SetNScintillatorBarsX(faserTrackingNBarsXCmd->GetNewIntValue(newValues));
  else if (command == faserTrackingScinThickCmd) 
    GeometricalParameters::Get()->SetScintillatorThickness(faserTrackingScinThickCmd->ConvertToDimensionedDouble(newValues));
  else if (command == faserTrackingGapCmd) 
    GeometricalParameters::Get()->SetTrackingStationGap(faserTrackingGapCmd->ConvertToDimensionedDouble(newValues));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
