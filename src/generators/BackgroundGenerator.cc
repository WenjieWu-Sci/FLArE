#include "generators/GeneratorBase.hh"
#include "generators/BackgroundGenerator.hh"
#include "generators/BackgroundGeneratorMessenger.hh"

#include "geometry/GeometricalParameters.hh"

#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4Poisson.hh"
#include "Randomize.hh"

#include "TLorentzVector.h"
#include "TMath.h"
#include "TFile.h"
#include "TTree.h"
#include "TH3D.h"
#include "TH2D.h"

BackgroundGenerator::BackgroundGenerator()
{
  fGeneratorName = "background";
  fMessenger = new BackgroundGeneratorMessenger(this);
  fGPS = new G4GeneralParticleSource();

  fBkgFile = nullptr;
  fhxyE = nullptr;
  fhdir = nullptr;
  fBkgTimeWindow = TPC_driftTime_s*s;
}

BackgroundGenerator::~BackgroundGenerator()
{
  delete fhxyE;
  delete fhdir;
  if(fBkgFile) fBkgFile->Close();
  delete fMessenger;
}

void BackgroundGenerator::LoadData()
{
  // open the file 
  fBkgFile = new TFile(fBkgFilename, "read");
  if (!fBkgFile->IsOpen() || fBkgFile->IsZombie()) {
    G4String err = "Cannot open background file : " + fBkgFilename;
    G4Exception("BackgroundGenerator", "FileError", FatalErrorInArgument, err.c_str());
  }
  
  // find the source histograms
  // for each background species (mu+, mu-, n)
  for(auto const name : fSpeciesList ){
    
    G4String hxyE_path = name + "/hxyE_" + name;
    G4String hdir_path = name + "/hdir_" + name;

    // get the input histograms
    // 3D histo in (x, y, E): normalized, to be used for the main extraction
    // 3D histo in (xdircos, ydircos, E): one energy is extracted, 
    fhxyE = (TH3D*)fBkgFile->Get(hxyE_path.c_str());
    fhdir = (TH3D*)fBkgFile->Get(hdir_path.c_str());

    if(!fhxyE || !fhdir) {
      G4String err = "Histograms " + hxyE_path + " or " + hdir_path + " unavailable in " + fBkgFilename;
      G4Exception("BackgroundGenerator", "FileError", FatalErrorInArgument, err.c_str());
    }
  }
}


void BackgroundGenerator::ShootParticle(G4Event* anEvent, G4int pdg, TLorentzVector x4, TLorentzVector p4) const
{
  // prepare a particle with the extracted starting position and momentum
  // once ready, shoot it with the gun!
  G4ParticleDefinition* particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle(pdg);
 
  /* uncomment for debugging.. 
  G4cout << "Particle PDG " << pdg << " mass " << particleDefinition->GetPDGMass()*MeV << G4endl;
  G4cout << "  x4  " << x4.X() << " " << x4.Y() << " " << x4.Z() << " " << x4.T() << G4endl;
  G4cout << "  p4  " << p4.X() << " " << p4.Y() << " " << p4.Z() << " " << p4.E() << G4endl;
  G4cout << "  kinE " << (p4.E() - particleDefinition->GetPDGMass()*MeV)  << G4endl;
  */

  // load the gun...
  fGPS->SetParticleDefinition(particleDefinition);
  fGPS->GetCurrentSource()->GetEneDist()->SetMonoEnergy(( p4.E() - particleDefinition->GetPDGMass()*MeV));  // kinetic energy
  fGPS->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(G4ThreeVector(p4.X(), p4.Y(), p4.Z()));
  fGPS->GetCurrentSource()->GetPosDist()->SetPosDisType("Point");
  fGPS->GetCurrentSource()->GetPosDist()->SetCentreCoords( G4ThreeVector(x4.X(), x4.Y(), x4.Z()) );
  fGPS->GeneratePrimaryVertex(anEvent); // ...and shoot!
}

int BackgroundGenerator::ExtractBackgroundParticles() const
{
  // the (x,y,E) is normalized to the instantaneous luminosity
  // it represents the number of particles / GeV / cm2 / s
  // get the total particles by summing up all bins
  double summed_bins = fhxyE->Integral(); // in GeV-1 cm-2 s-1
  double Ebinsize = fhxyE->GetZaxis()->GetBinWidth(1); // GeV
  double xbinsize = fhxyE->GetXaxis()->GetBinWidth(1); // cm
  double ybinsize = fhxyE->GetXaxis()->GetBinWidth(1); // cm
  summed_bins *= Ebinsize * xbinsize * ybinsize; // in s-1

  // now convert from s-1 to the requested time window
  // this way we get the expected background particles in the time window
  double windows_per_sec = 1./(fBkgTimeWindow/s); // number of windows in a second
  int lambda = summed_bins/windows_per_sec; 

  // now this value is the expectation of a Poisson distribution (or Gaussian)
  // use it to extract a realization...
  int Nparticles = 0;
  if(lambda < 100) Nparticles = int(G4Poisson(lambda) + 0.5);
  else Nparticles = int(G4RandGauss::shoot(Nparticles, TMath::Sqrt(Nparticles))+0.5);

  return Nparticles;
}

void BackgroundGenerator::SampleDirectionCosines(double& xdircos, double& ydircos, double E) const
{
  // first, extract the 2D profile at energy E
  int energyBin = fhdir->GetZaxis()->FindBin(E);

  // limit the 3D histogram only on that energy bin then take the profile
  // reset the limits afterwards to restore it back
  fhdir->GetZaxis()->SetRange(energyBin, energyBin);
  TH2D* h2Ddir = (TH2D*)fhdir->Project3D("xy");
  fhdir->GetZaxis()->SetRange(0, 0);

  // let's normalize the new 2D histogram, then sample
  h2Ddir->Scale(1.0 / h2Ddir->Integral());
  h2Ddir->GetRandom2(xdircos, ydircos);
}

void BackgroundGenerator::GeneratePrimaries(G4Event* anEvent)
{

  // complete line from PrimaryGeneratorAction...
  G4cout << ") : Background Generator ===oooOOOooo===" << G4endl;

  int evtID = anEvent->GetEventID();
  G4cout << "oooOOOooo Event # " << evtID << " oooOOOooo" << G4endl;
  G4cout << "GeneratePrimaries from source " << fBkgFilename << G4endl;

  // for each background species available...
  for(auto const name : fSpeciesList) {
    
    // get total number of particles to shoot
    // TODO: currently launching the equivalent of the requested time window
    // This should be probably rephrased in terms of luminosity
    G4cout << "Requested time window for background extraction: " << fBkgTimeWindow/us << " us" << G4endl;
    int nparticles = ExtractBackgroundParticles();
    G4cout << "********" << G4endl;
    G4cout << "*** Shooting " << nparticles << " background " << name << "!" << G4endl;
    
    int pdg=0;
    if( name == "mu_plus") pdg = -13;
    else if( name == "mu_minus") pdg = 13;
    else if( name == "neut") pdg = 2112;

    // for each particle 
    for(int i=0; i<nparticles; i++){

      // extract position at entry wall + energy sampling from the histogram 
      // then extract the direction (directional cosines) from second histogram
      // TODO: you should actually be using a 5D histo for full correlations?
     
      double z = -1.*GeometricalParameters::Get()->GetHallHeadDistance(); //entry wall z in mm
      double t = 0.; // TODO: imprint the bunch-crossing timing structure?
      double x, y, E;
      fhxyE->GetRandom3(x, y, E); //pos in cm, E in GeV

      double xdircos, ydircos;
      SampleDirectionCosines(xdircos, ydircos, E); 
      double zdircos2 = 1 - (xdircos*xdircos) - (ydircos*ydircos);
      double zdircos = TMath::Sqrt(zdircos2); // always positive

      // now prepare inputs to load the gun
      TLorentzVector x4(x*cm, y*cm, z, t); // tell G4 that zy are cm

      double mass = G4ParticleTable::GetParticleTable()->FindParticle(pdg)->GetPDGMass()*MeV; // in MeV
      double totE = E*GeV + mass; //total energy
      double p = TMath::Sqrt( totE*totE - mass*mass );
      TLorentzVector p4( xdircos*p, ydircos*p, zdircos*p, totE);
  
      // you may fire when ready now 
      ShootParticle( anEvent, pdg, x4, p4);
    }

    G4cout << "*** " << name << " background done!" << G4endl;
  }
}

