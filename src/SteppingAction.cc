#include "SteppingAction.hh"
#include "RunAction.hh"

#include <G4Step.hh>
#include <G4Electron.hh>

SteppingAction::SteppingAction(RunAction* runAction)
  : fRunAction(runAction)
{
}

void SteppingAction::UserSteppingAction(const G4Step* aStep) {
  G4VPhysicalVolume* volume = aStep->GetPreStepPoint()->GetTouchable()->GetVolume();
  //if (volume != nullptr) {
  //  G4cout<<volume->GetName()<<G4endl;
  //}
}
