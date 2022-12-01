#ifndef FLARETRACKINGACTION_HH
#define FLARETRACKINGACTION_HH

#include "G4UserTrackingAction.hh"

class FLArETrackingAction : public G4UserTrackingAction
{
  public:
    FLArETrackingAction();
    virtual ~FLArETrackingAction() {};

    virtual void PreUserTrackingAction(const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);
};

#endif
