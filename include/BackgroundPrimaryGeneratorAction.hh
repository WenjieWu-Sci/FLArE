#ifndef BackgroundPrimaryGeneratorAction_HH
#define BackgroundPrimaryGeneratorAction_HH

#include "TFile.h"
#include "TLorentzVector.h"
#include "TH3D.h"
#include "globals.hh"

class G4GeneralParticleSource;
class G4Event;

class BackgroundPrimaryGeneratorAction {
  public:
    BackgroundPrimaryGeneratorAction(G4GeneralParticleSource*);
    ~BackgroundPrimaryGeneratorAction();

  public:

    void GeneratePrimaries(G4Event* anEvent, G4String filename, G4double time_window);
    void ShootParticle(G4Event* anEvent, G4int pdg, TLorentzVector x4, TLorentzVector p4);
    void SampleDirectionCosines(double& xdircos, double& ydircos, double E);
    int ExtractBackgroundParticles(G4double time_window);

  private:
    
    G4GeneralParticleSource* fGPS;
    G4String bkgFileName;
    TFile* bkgFile;
    TH3D *hxyE;
    TH3D *hdir;

    std::vector<std::string> speciesList = { "mu_plus","mu_minus","neut" }; 
    G4double LHC_orbitPeriod_s = 88.924e-6; // orbit is 88.924 us
    G4double TPC_driftTime_s = 187.5e-6; // 187.5 us for 30cm drift

};

#endif
