#ifndef STACKINGACTION_HH
#define STACKINGACTION_HH

#include <G4UserStackingAction.hh>
#include <G4Track.hh>

class RunAction;
class EventAction;

class StackingAction : public G4UserStackingAction {
  public:
    StackingAction(RunAction*, EventAction*);
    ~StackingAction() {;};

    //! Main interface
    G4ClassificationOfNewTrack ClassifyNewTrack (const G4Track*);

  private:
    RunAction* fRunAction;
    EventAction* fEventAction;
};

#endif
