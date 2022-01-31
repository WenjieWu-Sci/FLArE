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
    /// @param aMass A mass of the particle.
    /// @param aMomentum An initial particle momentum (at the primary vertex).
    /// @param aVertex An initial particle vertex.
    /// @param interaction type of the neutrino interaction if it is from GENIE
    /// @param scattering type of the neutrino interaction if it is from GENIE
    PrimaryParticleInformation(G4int aID, G4int aPDG, G4double aMass,
        G4ThreeVector aMomentum, G4ThreeVector aVertex,
        G4int aInttype, G4int aScatteringtype);

    virtual ~PrimaryParticleInformation();

    /// Get the particle unique ID (within event). Can be set only in the constructor.
    inline G4int GetPartID() const { return fPartID; };

    /// Get the particle PDG code
    inline G4int GetPDG() const { return fPDG; };

    /// Get the particle mass in MeV
    inline G4double GetMass() const { return fMass; };

    /// Get the particle initial momentum.
    inline G4ThreeVector GetMomentumMC() const { return fMomentumMC; };

    /// Get the particle initial vertex.
    inline G4ThreeVector GetVertexMC() const { return fVertexMC; };

    /// Get the truth information of the neutrino interaction if it's from GENIE
    inline G4int GetInteractionTypeId() const  { return fInteractionType; };
    inline G4int GetScatteringTypeId()  const  { return fScatteringType;  };

    /// Prints the information about the particle.
    virtual void Print() const;
    
  private:

    /// A particle unique ID.
    G4int fPartID;

    /// A particle type (PDG code).
    G4int fPDG;
    
    /// A particle mass
    G4double fMass;

    /// A particle initial momentum (from particle generator)
    G4ThreeVector fMomentumMC;

    /// A particle initial vertex (from particle generator)
    G4ThreeVector fVertexMC;

    /// Interaction type of the neutrino interaction if it is from GENIE
    G4int fInteractionType;

    /// Scattering type of the neutrino interaction if it is from GENIE
    G4int fScatteringType;
};

#endif
