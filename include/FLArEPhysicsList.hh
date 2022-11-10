#ifndef FLAREPHYSICSLIST_HH
#define FLAREPHYSICSLIST_HH

#include "G4VModularPhysicsList.hh"

class G4PhysListFactory;
class G4StepLimiter;
class G4UserSpecialCuts;

class FLArEPhysicsList: public G4VModularPhysicsList
{
  public:
    FLArEPhysicsList();
    virtual ~FLArEPhysicsList();

    void AddStepMax();

    virtual void ConstructParticle();
    virtual void ConstructProcess();

  private:
    G4PhysListFactory* factory;
    G4StepLimiter* fStepLimiter;
    G4UserSpecialCuts* fUserSpecialCuts;
};

#endif
