#include <ostream>
#include <time.h>

#include "PrimaryGeneratorMessenger.hh"
#include "PrimaryGeneratorAction.hh"
#include "GENIEPrimaryGeneratorAction.hh"
#include "PrimaryParticleInformation.hh"
#include "geometry/GeometricalParameters.hh"

#include <G4ParticleTable.hh>
#include <G4Event.hh>
#include <G4SystemOfUnits.hh>
#include <G4ParticleGun.hh>
#include <Randomize.hh>
#include <G4GeneralParticleSource.hh>

//PrimaryGeneratorAction* PrimaryGeneratorAction::GeneratorInstance = 0;
//
//PrimaryGeneratorAction* PrimaryGeneratorAction::GetInstance() {
//  if (!GeneratorInstance) {
//    std::cout<<"PrimaryGeneratorAction: Re-initialization"<<std::endl;
//    GeneratorInstance = new PrimaryGeneratorAction();
//  }
//  return GeneratorInstance;
//}

PrimaryGeneratorAction::PrimaryGeneratorAction() {
  fGPS = new G4GeneralParticleSource();
  
  // default setting of primary particle.
  // can be override in GeneratePrimaries or in macros.
  G4ParticleDefinition* myParticle;
  myParticle = G4ParticleTable::GetParticleTable()->FindParticle("e-");
  fGPS->SetParticleDefinition(myParticle);
  fGPS->GetCurrentSource()->GetEneDist()->SetMonoEnergy(5*GeV);  // kinetic energy
  fGPS->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(G4ThreeVector(0,0,1));
  G4long seeds[2];
  time_t systime = time(NULL);
  seeds[0] = (G4long) systime;
  seeds[1] = (G4long) (systime*G4UniformRand());
  G4Random::setTheSeeds(seeds);
  G4Random::showEngineStatus();
  G4double x0 = G4UniformRand();
  G4double y0 = G4UniformRand();
  G4double z0 = G4UniformRand();
  x0 = GeometricalParameters::Get()->GetFLArEPosition().x() +
       (x0-0.5)*GeometricalParameters::Get()->GetFLArEFidVolSize().x();
  y0 = GeometricalParameters::Get()->GetFLArEPosition().y() +
       (y0-0.5)*GeometricalParameters::Get()->GetFLArEFidVolSize().y();
  z0 = GeometricalParameters::Get()->GetFLArEPosition().z() +
       (z0-0.5)*GeometricalParameters::Get()->GetFLArEFidVolSize().z();;
  fGPS->GetCurrentSource()->GetPosDist()->SetPosDisType("Point");
  fGPS->GetCurrentSource()->GetPosDist()->SetCentreCoords(G4ThreeVector(x0, y0, z0));

  fActionGenie = new GENIEPrimaryGeneratorAction(fGPS);

  // create a messenger for this class
  genMessenger = new PrimaryGeneratorMessenger(this);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
  delete fGPS;
  delete fActionGenie;
  delete genMessenger;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
  if (useGenie) {
    std::cout<<std::endl;
    std::cout<<"===oooOOOooo=== Event Generator (# "<<anEvent->GetEventID()<<") : GENIE ===oooOOOooo==="<<std::endl;
    fActionGenie->GeneratePrimaries(anEvent, gstFileName, gstEvtStartIdx, 1);
    neuidx          = fActionGenie->NeuIdx();
    neupdg          = fActionGenie->NeuPDG();
    neup4           = fActionGenie->NeuP4();
    neux4           = fActionGenie->NeuX4();
    int_type        = fActionGenie->InteractionTypeId();
    scattering_type = fActionGenie->ScatteringTypeId();
    w               = fActionGenie->GetW();
    fslpdg          = fActionGenie->FSLPDG();
    fslp4           = fActionGenie->FSLP4();
  } else {
    std::cout<<std::endl;
    std::cout<<"===oooOOOooo=== Event Generator (# "<<anEvent->GetEventID()<<"): General Particle Source ===oooOOOooo==="<<std::endl;
    fGPS->GeneratePrimaryVertex(anEvent);
  }

  // loop over the vertices, and then over primary particles,
  // and for each primary particle create an info object, in 
  // which to store MC truth information
  G4int count_particles = 0;
  for (G4int ivtx = 0; ivtx < anEvent->GetNumberOfPrimaryVertex(); ++ivtx) {
    for (G4int ipp = 0; ipp < anEvent->GetPrimaryVertex(ivtx)->GetNumberOfParticle(); ++ipp) {
      G4PrimaryParticle* primary_particle = 
        anEvent->GetPrimaryVertex(ivtx)->GetPrimary(ipp);
      if (primary_particle) {
        primary_particle->SetUserInformation(new PrimaryParticleInformation(
              count_particles, primary_particle->GetPDGcode(), primary_particle->GetMass(),
              primary_particle->GetMomentum(), anEvent->GetPrimaryVertex(ivtx)->GetPosition(),
              neuidx, neupdg, neup4, neux4, int_type, scattering_type, w,
              fslpdg, fslp4));
        count_particles++;
      }
    }
  }
}
