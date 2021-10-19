#include "PrimaryGeneratorAction.hh"

#include <G4ParticleTable.hh>
#include <G4Event.hh>
#include <G4SystemOfUnits.hh>
#include <G4ParticleGun.hh>
#include <Randomize.hh>
#include <G4GeneralParticleSource.hh>

#include <TMath.h>

using namespace std;

PrimaryGeneratorAction::PrimaryGeneratorAction() {
  fGPS = new G4GeneralParticleSource();

  G4ParticleDefinition* myParticle;
  myParticle = G4ParticleTable::GetParticleTable()->FindParticle("e-");
  fGPS->SetParticleDefinition(myParticle);
  fGPS->GetCurrentSource()->GetEneDist()->SetMonoEnergy(5*GeV);
  fGPS->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(G4ThreeVector(0,0,1));
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
  delete fGPS;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
  G4double x0 = 0. * m;
  G4double y0 = 0. * m;
  G4double z0 = -7. * m;
//  G4double dz = 7. * m;
//  z0 += dz*(G4UniformRand()-0.5);
  fGPS->SetParticlePosition(G4ThreeVector(x0, y0, z0));

  fGPS->GeneratePrimaryVertex(anEvent);
}
