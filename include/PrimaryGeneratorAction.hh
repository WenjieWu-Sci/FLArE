#ifndef PRIMARY_GENERATOR_ACTION_HH
#define PRIMARY_GENERATOR_ACTION_HH 
#include <TLorentzVector.h>
#include <G4VUserPrimaryGeneratorAction.hh>
#include <globals.hh>

class G4ParticleGun;
class G4GeneralParticleSource;
class PrimaryGeneratorMessenger;
class GENIEPrimaryGeneratorAction;
class BackgroundPrimaryGeneratorAction;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
  public:
    PrimaryGeneratorAction();
    ~PrimaryGeneratorAction();
    
    void GeneratePrimaries(G4Event* anEvent) override;

    void setUseGenie(G4bool val) { useGenie = val; }
    void setGenieInputFile(G4String val) { gstFileName = val; }
    void setGenieStartEvt(G4int val) { gstEvtStartIdx = val; }

    void setUseBackground(G4bool val) { useBackground = val; }
    void setBkgInputFile(G4String val) { bkgFileName = val; }

  private:
    PrimaryGeneratorMessenger* genMessenger;

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
    G4bool useGenie;
    G4String gstFileName;
    G4int gstEvtStartIdx;
    
    BackgroundPrimaryGeneratorAction* fActionBackground;
    G4bool useBackground;
    G4String bkgFileName;
};

#endif
