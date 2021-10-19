#include "StackingAction.hh"
#include "G4TrackStatus.hh"
#include "RunAction.hh"
#include "EventAction.hh"

StackingAction::StackingAction(RunAction* aRunAction, EventAction* aEventAction) :
  G4UserStackingAction(), fRunAction(aRunAction), fEventAction(aEventAction)
{;}

G4ClassificationOfNewTrack StackingAction::ClassifyNewTrack (const G4Track* aTrack) {
  // Register only secondaries, i.e. tracks having ParentID > 0
  if (aTrack->GetParentID()) {
    fEventAction->AddSecondaryTrack();
    if (aTrack->GetParticleDefinition()->GetPDGEncoding() != 22) {
      fEventAction->AddSecondaryTrackNotGamma();
    }
  }

  // Do not affect track classification. Just return what would have
  // been returned by the base class
  return G4UserStackingAction::ClassifyNewTrack(aTrack);
}
