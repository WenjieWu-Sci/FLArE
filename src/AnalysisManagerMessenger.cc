// 
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "AnalysisManagerMessenger.hh"

//#include <sstream>

#include "AnalysisManager.hh"
#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIparameter.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithABool.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  AnalysisManagerMessenger::AnalysisManagerMessenger(AnalysisManager* manager)
:histo (manager)
{
  histoDir = new G4UIdirectory("/histo/");
  histoDir->SetGuidance("histograms control");

  factoryCmd = new G4UIcmdWithAString("/histo/fileName", this);
  factoryCmd->SetGuidance("set name for the histograms file");
  factoryCmd->AvailableForStates(G4State_PreInit,G4State_Idle); 

  saveSecCmd = new G4UIcmdWithABool("/histo/saveSecondary", this);
  saveSecCmd->SetGuidance("whether save the information of secondary particles");
  saveSecCmd->SetParameterName("saveSecondary", true);
  saveSecCmd->SetDefaultValue(false);

  saveEvdCmd = new G4UIcmdWithABool("/histo/saveEvd", this);
  saveEvdCmd->SetGuidance("whether save event display");
  saveEvdCmd->SetParameterName("saveEvd", true);
  saveEvdCmd->SetDefaultValue(false);

  //  histoCmd = new G4UIcmdWithAnInteger("/histo/setSeed",this);
  //  histoCmd->SetGuidance("Set random seed :");
  //  histoCmd->SetDefaultValue(1);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

AnalysisManagerMessenger::~AnalysisManagerMessenger()
{
  delete factoryCmd;
  delete saveSecCmd;
  delete saveEvdCmd;
  delete histoDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void AnalysisManagerMessenger::SetNewValue(G4UIcommand* command,G4String newValues)
{
  if (command == factoryCmd) histo->setFileName(newValues);
  if (command == saveSecCmd) histo->saveSecondary(saveSecCmd->GetNewBoolValue(newValues));
  if (command == saveEvdCmd) histo->saveEvd(saveEvdCmd->GetNewBoolValue(newValues));

  //  if(command == histoCmd) histo->setSeed(newValues);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
