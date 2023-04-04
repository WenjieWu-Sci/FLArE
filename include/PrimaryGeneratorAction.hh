#ifndef PRIMARY_GENERATOR_ACTION_HH
#define PRIMARY_GENERATOR_ACTION_HH

#include <TLorentzVector.h>
#include <G4VUserPrimaryGeneratorAction.hh>
#include <globals.hh>
#include "Framework/Interaction/InteractionType.h"
#include "Framework/Interaction/ScatteringType.h"

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
    G4int neuidx;
    G4int neupdg;
    TLorentzVector neup4;
    TLorentzVector neux4;
    G4int int_type        { -1 };
    G4int scattering_type { -1 };
    G4double w { -1 };
    G4int fslpdg;
    TLorentzVector fslp4;
    TLorentzVector fslx4;

    G4bool useGenie;
    G4String ghepFileName;
    G4int ghepEvtStartIdx;
};

#endif
