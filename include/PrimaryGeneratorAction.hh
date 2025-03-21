#ifndef PRIMARY_GENERATOR_ACTION_HH
#define PRIMARY_GENERATOR_ACTION_HH 

#include <G4VUserPrimaryGeneratorAction.hh>
#include <globals.hh>

class PrimaryGeneratorMessenger;
class GeneratorBase;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction 
{
  public:
    PrimaryGeneratorAction();
    ~PrimaryGeneratorAction();
    
    void GeneratePrimaries(G4Event* anEvent) override;
    void SetGenerator(G4String name);

  private:

    PrimaryGeneratorMessenger* fGenMessenger;
    GeneratorBase* fGenerator;
    G4bool fInitialized;

};

#endif
