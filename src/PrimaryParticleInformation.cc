#include "PrimaryParticleInformation.hh"

PrimaryParticleInformation::PrimaryParticleInformation(G4int aID, G4int aPDG, G4ThreeVector aMomentum, G4ThreeVector aVertex) :
  fPartID(aID), fPDG(aPDG), fMomentumMC(aMomentum), fVertexMC(aVertex)
{
}

PrimaryParticleInformation::~PrimaryParticleInformation()
{
}

void PrimaryParticleInformation::Print() const {
  G4cout<<"PrimaryParticleInformation: PDG code "<<fPDG<<G4endl
    <<"Particle unique ID : "<<fPartID<<G4endl
    <<"MC momentum : "<<fMomentumMC<<G4endl
    <<"MC vertex : "<<fVertexMC<<G4endl;
}

