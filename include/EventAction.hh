#ifndef EVENTACTION_HH
#define EVENTACTION_HH

#include <G4UserEventAction.hh>
#include <globals.hh>
#include <G4Accumulable.hh>

class EventAction : public G4UserEventAction {
  public:
    EventAction();
    ~EventAction();
    void BeginOfEventAction(const G4Event* event) override;
    void EndOfEventAction(const G4Event* event) override;

    void AddPrimaryTrack();
    void AddSecondaryTrack();
    void AddSecondaryTrackNotGamma();

  private:
    G4Accumulable<G4int> fNPrimaryTrack;
    G4Accumulable<G4int> fNSecondaryTrack;
    G4Accumulable<G4int> fNSecondaryTrackNotGamma;
};

#endif
