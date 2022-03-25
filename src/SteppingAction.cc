#include "SteppingAction.hh"
#include "RunAction.hh"

#include <G4Step.hh>
#include <G4Electron.hh>

SteppingAction::SteppingAction(RunAction* runAction)
  : fRunAction(runAction)
{
}

void SteppingAction::UserSteppingAction(const G4Step* aStep) {
  //G4Track* aTrack = aStep->GetTrack();
  //if (aTrack->GetTrackID()==1) {
  //  G4cout<<aTrack->GetParticleDefinition()->GetPDGEncoding()<<G4endl;
  //  const G4TrackVector* nextTracks = aStep->GetSecondary();
  //  for (auto nextTrack: *nextTracks) {
  //    G4cout<<nextTrack->GetParticleDefinition()->GetPDGEncoding()<<G4endl;
  //  }
  //}
  //G4VPhysicalVolume* volume = aStep->GetPreStepPoint()->GetTouchable()->GetVolume();
  //if (volume != nullptr) {
  //  G4cout<<volume->GetName()<<G4endl;
  //}
}
