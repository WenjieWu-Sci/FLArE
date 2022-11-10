#include "FLArEPhysicsList.hh"
#include "G4PhysListFactory.hh"
#include "G4StepLimiterPhysics.hh"
#include "G4ProcessManager.hh"
#include "G4StepLimiter.hh"
#include "G4UserSpecialCuts.hh"

#include <algorithm>

FLArEPhysicsList::FLArEPhysicsList() : G4VModularPhysicsList()
{
  factory = new G4PhysListFactory();
  G4VModularPhysicsList* physicsList = factory->ReferencePhysList();

  for (G4int i = 0; ; ++i) {
    G4VPhysicsConstructor* elem = 
      const_cast<G4VPhysicsConstructor*> (physicsList->GetPhysics(i));
    if (elem == NULL) break;
    G4cout<<"RegisterPhysics: "<<elem->GetPhysicsName()<<G4endl;
    RegisterPhysics(elem);
  }
  G4StepLimiterPhysics* stepLimiterPhys = new G4StepLimiterPhysics();
//  stepLimiterPhys->SetApplyToAll(true);     // activates step limit for ALL particles
  RegisterPhysics(stepLimiterPhys);
}

FLArEPhysicsList::~FLArEPhysicsList()
{
  delete factory;
  delete fStepLimiter;
  delete fUserSpecialCuts;
}

void FLArEPhysicsList::ConstructParticle()
{
  G4VModularPhysicsList::ConstructParticle();
}

void FLArEPhysicsList::ConstructProcess()
{
  G4VModularPhysicsList::ConstructProcess();

  AddStepMax();
}

void FLArEPhysicsList::AddStepMax()
{
  // Step limitation seen as a process
  auto particleIterator = GetParticleIterator();
  particleIterator->reset();

  fStepLimiter = new G4StepLimiter();
  fUserSpecialCuts = new G4UserSpecialCuts();
  G4int nullNeutralPDG[7] = {-16, -14, -12, 12, 14, 16, 22};
  while ((*particleIterator)()) {
    G4ParticleDefinition* particle = particleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4double charge = particle->GetPDGCharge();
    G4int particlePDG = particle->GetPDGEncoding();

    if (!particle->IsShortLived()) {
      if (charge != 0.0) {
        pmanager->AddDiscreteProcess(fStepLimiter);
        pmanager->AddDiscreteProcess(fUserSpecialCuts);
      } else {
        if (std::find(std::begin(nullNeutralPDG), std::end(nullNeutralPDG), particlePDG) != std::end(nullNeutralPDG) ) {
          pmanager->AddDiscreteProcess(fUserSpecialCuts);
        } else {
          pmanager->AddDiscreteProcess(fStepLimiter);
          pmanager->AddDiscreteProcess(fUserSpecialCuts);
        }
      } 
    }
  }
}
