#ifndef GENERATOR_BASE_HH
#define GENERATOR_BASE_HH

#include "G4Event.hh"
#include "G4UImessenger.hh"

class GeneratorBase
{
  public:

    GeneratorBase() : fGeneratorName("unknown"), fMessenger(nullptr) {}
    virtual ~GeneratorBase() {}

    // Called once (by PrimaryGeneratorAction) to load the external file,
    // open files, set branch addresses, or load histograms, etc..
    virtual void LoadData() = 0;

    // Called for each event to generate primaries
    virtual void GeneratePrimaries(G4Event *event) = 0;

    // return name of current generator
    G4String GetGeneratorName() { return fGeneratorName; }

  protected : 

    G4String fGeneratorName; 
    G4UImessenger* fMessenger;
    
};

#endif
