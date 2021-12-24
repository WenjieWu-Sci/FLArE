#ifndef PrimaryParticleInformation_HH
#define PrimaryParticleInformation_HH

#include "G4VUserPrimaryParticleInformation.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"

// Primary particle information
class PrimaryParticleInformation : public G4VUserPrimaryParticleInformation {

  public:
    /// a constructor
    /// @param aID A unique particle ID within event.
    /// @param aPDG A PDG code of the particle.
    /// @param aMomentum An initial particle momentum (at the primary vertex).
    /// @param aVertex An initial particle vertex.
    PrimaryParticleInformation(G4int aID, G4int aPDG, G4ThreeVector aMomentum, G4ThreeVector aVertex);

    virtual ~PrimaryParticleInformation();

    /// Get the particle unique ID (within event). Can be set only in the constructor.
    inline G4int GetPartID() const { return fPartID; };

    /// Prints the information about the particle.
    virtual void Print() const;
    
  private:

    /// A particle unique ID.
    G4int fPartID;

    /// A particle type (PDG code).
    G4int fPDG;

    /// A particle initial momentum (from particle generator)
    G4ThreeVector fMomentumMC;

    /// A particle initial vertex (from particle generator)
    G4ThreeVector fVertexMC;
};

#endif
