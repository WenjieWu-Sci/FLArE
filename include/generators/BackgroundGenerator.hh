#ifndef BackgroundGenerator_HH
#define BackgroundGenerator_HH

#include "generators/GeneratorBase.hh"

#include "TFile.h"
#include "TH3D.h"
#include "TLorentzVector.h"
#include "globals.hh"

class G4Event;
class G4GeneralParticleSource;

class BackgroundGenerator : public GeneratorBase
{
  public:
    BackgroundGenerator();
    ~BackgroundGenerator();

  public:

    // override methods from common base class
    void LoadData() override;
    void GeneratePrimaries(G4Event *anEvent) override;

    // setter methods for messenger
    void SetBkgFilename(G4String val) { fBkgFilename = val; }
    void SetBkgTimeWindow(G4double val) { fBkgTimeWindow = val;}

  private:
    
    G4GeneralParticleSource *fGPS;

    G4String fBkgFilename;
    G4double fBkgTimeWindow;
    TFile* fBkgFile;
    TH3D *fhxyE;
    TH3D *fhdir;

    std::vector<std::string> fSpeciesList = { "mu_plus","mu_minus","neut" };
    G4double LHC_orbitPeriod_s = 88.924e-6; // orbit is 88.924 us
    G4double TPC_driftTime_s = 187.5e-6; // 187.5 us for 30cm drift

    // specific internal functions
    void ShootParticle(G4Event* anEvent, G4int pdg, TLorentzVector x4, TLorentzVector p4) const;
    void SampleDirectionCosines(double& xdircos, double& ydircos, double E) const;
    int ExtractBackgroundParticles() const;

};

#endif
