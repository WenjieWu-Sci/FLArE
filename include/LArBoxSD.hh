#ifndef LARBOXSD_HH
#define LARBOXSD_HH

#include <G4VSensitiveDetector.hh>
#include "LArBoxHit.hh"

class LArBoxSD : public G4VSensitiveDetector {
  public:
    LArBoxSD(G4String name);

    void Initialize(G4HCofThisEvent*) override;

  protected:
    G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* R0hist) override;

  private:
    LArBoxHitsCollection* fHitsCollection { nullptr };
    G4int fHitsCollectionId { -1 };
};

#endif


