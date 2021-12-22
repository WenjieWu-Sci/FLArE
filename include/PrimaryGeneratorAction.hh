#ifndef PRIMARY_GENERATOR_ACTION_HH
#define PRIMARY_GENERATOR_ACTION_HH

#include <G4VUserPrimaryGeneratorAction.hh>
#include <globals.hh>

class G4ParticleGun;
class G4GeneralParticleSource;
class PrimaryGeneratorMessenger;
class GENIEPrimaryGeneratorAction;


class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
  public:
    PrimaryGeneratorAction();
    ~PrimaryGeneratorAction();
//    static PrimaryGeneratorAction* GetInstance();
//    void BeginOfRun();
//    void EndOfRun();
    void GeneratePrimaries(G4Event* anEvent) override;

    void setUseGenie(G4bool val) { useGenie = val; }
    void setGenieInputFile(G4String val) { ghepFileName = val; }
    void setGenieStartEvt(G4int val) { ghepEvtStartIdx = val; }

  private:
//    static PrimaryGeneratorAction* GeneratorInstance;
    PrimaryGeneratorMessenger* genMessenger;

    G4ParticleGun* fGun;
    G4GeneralParticleSource* fGPS;

    GENIEPrimaryGeneratorAction* fActionGenie;

    G4bool useGenie;
    G4String ghepFileName;
    G4int ghepEvtStartIdx;
};

#endif
