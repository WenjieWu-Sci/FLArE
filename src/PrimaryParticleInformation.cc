#include "PrimaryParticleInformation.hh"

PrimaryParticleInformation::PrimaryParticleInformation(G4int aID, G4int aPDG, G4double aMass,
    G4ThreeVector aMomentum, G4ThreeVector aVertex, 
    G4int aneuIdx, G4int aneuPDG, TLorentzVector aneuP4, TLorentzVector aneuX4,
    G4int aInttype, G4int aScatteringtype, G4double aW,
    G4int afslPDG, TLorentzVector afslP4, TLorentzVector afslX4) :
  fPartID(aID), fPDG(aPDG), fMass(aMass),
  fMomentumMC(aMomentum), fVertexMC(aVertex), 
  fNeuIdx(aneuIdx), fNeuPDG(aneuPDG), fNeuP4(aneuP4), fNeuX4(aneuX4),
  fInteractionType(aInttype), fScatteringType(aScatteringtype), fW(aW),
  fFSLPDG(afslPDG), fFSLP4(afslP4), fFSLX4(afslX4)
{
}

PrimaryParticleInformation::~PrimaryParticleInformation()
{
}

void PrimaryParticleInformation::Print() const {
  G4cout<<G4endl;
  G4cout<<"PrimaryParticleInformation: PDG code "<<fPDG<<G4endl
    <<"Particle unique ID : "<<fPartID<<G4endl
    <<"MC momentum : "<<fMomentumMC<<" MeV"<<G4endl
    <<"MC vertex : "<<fVertexMC<<" mm"<<G4endl;
  // omit this printout
  /***************************************************
  if (fInteractionType!=-1 && fScatteringType!=-1) {
    G4cout<<"NeutrinoInteractionInformation : "
          <<fInteractionType<<" "<<fScatteringType;
    G4cout<<", FSL : "<<fFSLPDG<<" (" <<fFSLP4.X()<<", "
      <<fFSLP4.Y()<<", "<<fFSLP4.Z()<<", "<<fFSLP4.T() <<") GeV"<<G4endl;
  }
  ***************************************************/
}

