#include <G4RunManager.hh>
#include <G4UImanager.hh>

#include <G4VisExecutive.hh>

#include <G4UIExecutive.hh>

#include <G4String.hh>
#include "ActionInitialization.hh"
#include "LArBoxDetectorConstruction.hh"
//#include "FLArEDetectorConstructionMessenger.hh"
#include "QGSP_BERT.hh"
//#include "AnalysisManager.hh"
//#include "PrimaryGeneratorAction.hh"
#include "G4PhysListFactory.hh"

using namespace std;

/* Main function that enables to:
 * - run macros
 * - start interactive UI mode (no arguments)
 */
int main(int argc, char** argv) {
  std::cout<<"Application starting..."<<std::endl;
  G4long myseed = 345354;
  CLHEP::HepRandom::setTheSeed(myseed);

//  AnalysisManager* analysis = AnalysisManager::GetInstance();

  // Create the run manager (MT or non-MT) and make it a bit verbose.
  auto runManager = new G4RunManager();
  runManager->SetVerboseLevel(1);

  runManager->SetUserInitialization(new LArBoxDetectorConstruction());
  //runManager->SetUserInitialization(new QGSP_BERT());
  G4PhysListFactory factory;
  runManager->SetUserInitialization(factory.ReferencePhysList());
  runManager->SetUserInitialization(new ActionInitialization());

  G4VisManager* visManager = new G4VisExecutive();
  visManager->SetVerboseLevel(1);   // Default, you can always override this using macro commands
  visManager->Initialize();

  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // Parse command line arguments
  if (argc==1) {
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
    UImanager->ApplyCommand("/control/execute macros/vis.mac");
    ui->SessionStart();
    delete ui;
  } else {
    //G4UIExecutive* ui = new G4UIExecutive(argc, argv);
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
    //ui->SessionStart();
    //delete ui;
  }

  delete visManager;
  delete runManager;

  std::cout<<"Application sucessfully ended.\nBye :-)"<<std::endl;

  return 0;
}
