#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
//! Optional user classes
#include "RunAction.hh"
#include "EventAction.hh"
#include "StackingAction.hh"
#include "SteppingAction.hh"

void ActionInitialization::Build() const {
  SetUserAction(new PrimaryGeneratorAction());
  RunAction* theRunAction = new RunAction();
  SetUserAction(theRunAction);
  EventAction* theEventAction = new EventAction();
  SetUserAction(theEventAction);
  SetUserAction(new StackingAction(theRunAction, theEventAction));
  SetUserAction(new SteppingAction(theRunAction));
}

void ActionInitialization::BuildForMaster() const {
  // By default, don't do anything. This applies only in MT mode
  //SetUserAction(new RunAction);
}
