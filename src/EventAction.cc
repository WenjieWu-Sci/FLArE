#include "EventAction.hh"

#include <G4Event.hh>
#include <G4AccumulableManager.hh>

#include "AnalysisManager.hh"

using namespace std;

EventAction::EventAction() :
  G4UserEventAction(),
  fNSecondaryTrack("NSecondaryTrack", 0),
  fNSecondaryTrackNotGamma("NSecondaryTrackNotGamma", 0) {
    // Register created accumulables
    G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
    accumulableManager->RegisterAccumulable(fNSecondaryTrack);
    accumulableManager->RegisterAccumulable(fNSecondaryTrackNotGamma);
  }

EventAction::~EventAction() {;}

void EventAction::BeginOfEventAction(const G4Event* event) {
  // Reset all accumulables to their initial values
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Reset();

  AnalysisManager* ana = AnalysisManager::GetInstance();
  ana->BeginOfEvent();
}

void EventAction::EndOfEventAction(const G4Event* event) {
  G4cout<<"This is the "<<event->GetEventID()<<"th event"<<G4endl;

  if (fNSecondaryTrack.GetValue()) 
    G4cout << " * Produced "<< fNSecondaryTrack.GetValue() << " secondary tracks." << G4endl;
  else
    G4cout << " * No secondary tracks produced" << G4endl;
  if (fNSecondaryTrackNotGamma.GetValue()) 
    G4cout << " * Produced "<< fNSecondaryTrackNotGamma.GetValue() << " secondary tracks (excluding gamma)." << G4endl;
  else
    G4cout << " * No secondary tracks (excluding gamma) produced" << G4endl;

  AnalysisManager* ana = AnalysisManager::GetInstance();
  ana->EndOfEvent(event);
}

void EventAction::AddSecondaryTrack() {
  fNSecondaryTrack += 1;
}

void EventAction::AddSecondaryTrackNotGamma() {
  fNSecondaryTrackNotGamma += 1;
}
