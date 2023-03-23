#ifndef STEPPINGACTION_HH
#define STEPPINGACTION_HH

#include <G4UserSteppingAction.hh>

class RunAction;

class SteppingAction : public G4UserSteppingAction {
  public:
    SteppingAction(RunAction*);

    void UserSteppingAction(const G4Step*) override;
    void TrackLiveDebugging(const G4Step*);

  private:
    RunAction* fRunAction;
};

#endif
