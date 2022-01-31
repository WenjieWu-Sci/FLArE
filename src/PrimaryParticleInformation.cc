#include "PrimaryParticleInformation.hh"

PrimaryParticleInformation::PrimaryParticleInformation(G4int aID, G4int aPDG, G4double aMass,
    G4ThreeVector aMomentum, G4ThreeVector aVertex, 
    G4int aInttype, G4int aScatteringtype) :
  fPartID(aID), fPDG(aPDG), fMass(aMass),
  fMomentumMC(aMomentum), fVertexMC(aVertex), 
  fInteractionType(aInttype), fScatteringType(aScatteringtype)
{
}

PrimaryParticleInformation::~PrimaryParticleInformation()
{
}

void PrimaryParticleInformation::Print() const {
  G4cout<<"ooooo  OOOOO  ooooo"<<G4endl;
  G4cout<<"PrimaryParticleInformation: PDG code "<<fPDG<<G4endl
    <<"Particle unique ID : "<<fPartID<<G4endl
    <<"MC momentum : "<<fMomentumMC<<G4endl
    <<"MC vertex : "<<fVertexMC<<G4endl;
  if (fInteractionType!=-1 && fScatteringType!=-1) {
    G4cout<<"NeutrinoInteractionInformation : "
          <<fInteractionType<<" "<<fScatteringType<<G4endl;
  }
}

