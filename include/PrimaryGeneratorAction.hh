#ifndef PRIMARY_GENERATOR_ACTION_HH
#define PRIMARY_GENERATOR_ACTION_HH 

#include "G4ThreeVector.hh"
#include <TLorentzVector.h>
#include <G4VUserPrimaryGeneratorAction.hh>
#include <globals.hh>

class G4ParticleGun;
class G4GeneralParticleSource;
class PrimaryGeneratorMessenger;
class GENIEPrimaryGeneratorAction;
class HepMCPrimaryGeneratorAction;
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
    void setBkgTimeWindow(G4double val) { bkgTimeWindow = val; }

    void setUseHepMC(G4bool val) { useHepMC = val; }
    void setHepMCInputFile(G4String val) { HepMCFileName = val; }
    void setHepMCVtxOffset(G4ThreeVector val) { HepMCVtxOffset = val; }
    

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
    G4double bkgTimeWindow;

    HepMCPrimaryGeneratorAction* fActionHepMC;
    G4bool useHepMC;
    G4String HepMCFileName;
    G4ThreeVector HepMCVtxOffset;

};

#endif
