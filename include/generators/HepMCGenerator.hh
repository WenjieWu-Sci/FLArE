#ifndef HepMCGenerator_HH
#define HepMCGenerator_HH

#include "generators/GeneratorBase.hh"

#include "HepMC3/ReaderAscii.h"
#include "HepMC3/ReaderAsciiHepMC2.h"
#include <HepMC3/Print.h>

#include "globals.hh"

class G4Event;

class HepMCGenerator : public GeneratorBase 
{
  public:
    HepMCGenerator();
    ~HepMCGenerator();

    // override methods from common base class
    void LoadData() override;
    void GeneratePrimaries(G4Event* anEvent) override;

    // setter methods for messenger
    void SetHepMCFilename(G4String val) { fHepMCFilename = val; }
    void SetUseHepMC2(G4bool val) { fUseHepMC2 = val; }
    void SetHepMCVertexOffset(G4ThreeVector val) { fVtxOffset = val; }

  private:

    G4String fHepMCFilename;
    G4bool fUseHepMC2;
    G4ThreeVector fVtxOffset;
    HepMC3::Reader* fAsciiInput;
        
    // specific internal functions
    std::shared_ptr<HepMC3::GenEvent> GenerateHepMCEvent();
    G4bool CheckVertexInsideWorld (const G4ThreeVector& pos) const;
    void HepMC2G4(const std::shared_ptr<HepMC3::GenEvent> hepmcevt, G4Event* g4event);
        
};


#endif