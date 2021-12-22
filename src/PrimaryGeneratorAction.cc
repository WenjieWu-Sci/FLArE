#include <ostream>

#include "PrimaryGeneratorMessenger.hh"
#include "PrimaryGeneratorAction.hh"
#include "GENIEPrimaryGeneratorAction.hh"

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
    std::cout<<"===OOO=== Event Generator : GENIE ===OOO==="<<std::endl;
    fActionGenie->GeneratePrimaries(anEvent, ghepFileName, ghepEvtStartIdx);
  } else {
    std::cout<<"===OOO=== Event Generator : General Particle Source ===OOO==="<<std::endl;
    G4ParticleDefinition* myParticle;
    myParticle = G4ParticleTable::GetParticleTable()->FindParticle("e-");
    fGPS->SetParticleDefinition(myParticle);
    fGPS->GetCurrentSource()->GetEneDist()->SetMonoEnergy(5*GeV);
    fGPS->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(G4ThreeVector(0,0,1));

    G4double x0 = 0. * m;
    G4double y0 = 0. * m;
    G4double z0 = 0. * m;
    G4double dz = 7. * m;
    z0 += dz*(G4UniformRand()-0.5);
    fGPS->GetCurrentSource()->GetPosDist()->SetPosDisType("Point");
    fGPS->GetCurrentSource()->GetPosDist()->SetCentreCoords(G4ThreeVector(x0, y0, z0));

    fGPS->GeneratePrimaryVertex(anEvent);
  }
}
