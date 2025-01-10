#include "BackgroundPrimaryGeneratorAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "geometry/GeometricalParameters.hh"

#include <G4GeneralParticleSource.hh>
#include <G4ParticleTable.hh>
#include <G4IonTable.hh>
#include <G4SystemOfUnits.hh>
#include <G4Poisson.hh>
#include <Randomize.hh>

#include <TLorentzVector.h>
#include <TMath.h>
#include <TTree.h>
#include <TH3D.h>
#include <TH2D.h>

BackgroundPrimaryGeneratorAction::BackgroundPrimaryGeneratorAction(G4GeneralParticleSource* gps)
  : fGPS(gps)
{}

BackgroundPrimaryGeneratorAction::~BackgroundPrimaryGeneratorAction()
{}

void BackgroundPrimaryGeneratorAction::ShootParticle(G4Event* anEvent, G4int pdg, TLorentzVector x4, TLorentzVector p4)
{
  // prepare a particle with the extracted starting position and momentum
  // once ready, shoot it with the gun!
  G4ParticleDefinition* particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle(pdg);
  
  G4cout << "Particle PDG " << pdg << " mass " << particleDefinition->GetPDGMass()*MeV << G4endl;
  G4cout << "  x4  " << x4.X() << " " << x4.Y() << " " << x4.Z() << " " << x4.T() << G4endl;
  G4cout << "  p4  " << p4.X() << " " << p4.Y() << " " << p4.Z() << " " << p4.E() << G4endl;
  G4cout << "  kinE " << (p4.E() - particleDefinition->GetPDGMass()*MeV)  << G4endl;

  // load the gun...
  fGPS->SetParticleDefinition(particleDefinition);
  fGPS->GetCurrentSource()->GetEneDist()->SetMonoEnergy(( p4.E() - particleDefinition->GetPDGMass()*MeV));  // kinetic energy
  fGPS->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(G4ThreeVector(p4.X(), p4.Y(), p4.Z()));
  fGPS->GetCurrentSource()->GetPosDist()->SetPosDisType("Point");
  fGPS->GetCurrentSource()->GetPosDist()->SetCentreCoords( G4ThreeVector(x4.X(), x4.Y(), x4.Z()) );
  fGPS->GeneratePrimaryVertex(anEvent); // ...and shoot!
}

int BackgroundPrimaryGeneratorAction::ExtractBackgroundParticlesPerOrbit()
{
  // the (x,y,E) is normalized to the instantaneous luminosity
  // it represents the number of particles / GeV / cm2 / s
  // get the total particles by summing up all bins
  double summed_bins = hxyE->Integral(); // in GeV-1 cm-2 s-1
  double Ebinsize = hxyE->GetZaxis()->GetBinWidth(1); // GeV
  double xbinsize = hxyE->GetXaxis()->GetBinWidth(1); // cm
  double ybinsize = hxyE->GetXaxis()->GetBinWidth(1); // cm
  summed_bins *= Ebinsize * xbinsize * ybinsize; // in s-1

  // now convert from s-1 to per full LHC orbit (88.9us)
  // this way we get the expected background particles in a full orbit period
  double orbits_per_sec = 1./LHC_orbitPeriod_s; // number of orbits per second
  int lambda = summed_bins/orbits_per_sec; 

  // now this value is the expectation of a Poisson distribution (or Gaussian)
  // use it to extract a realization...
  int Nparticles = 0;
  if(lambda < 100) Nparticles = int(G4Poisson(lambda) + 0.5);
  else Nparticles = int(G4RandGauss::shoot(Nparticles, TMath::Sqrt(Nparticles))+0.5);

  return Nparticles;
}

void BackgroundPrimaryGeneratorAction::SampleDirectionCosines(double& xdircos, double& ydircos, double E)
{
  // first, extract the 2D profile at energy E
  int energyBin = hdir->GetZaxis()->FindBin(E);

  // limit the 3D histogram only on that energy bin then take the profile
  // reset the limits afterwards to restore it back
  hdir->GetZaxis()->SetRange(energyBin, energyBin);
  TH2D* h2Ddir = (TH2D*)hdir->Project3D("xy");
  hdir->GetZaxis()->SetRange(0, 0);

  // let's normalize the new 2D histogram, then sample
  h2Ddir->Scale(1.0 / h2Ddir->Integral());
  h2Ddir->GetRandom2(xdircos, ydircos);
}

void BackgroundPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent, G4String filename)
{
  bkgFileName = filename;
  int evtID = anEvent->GetEventID();
  G4cout << "oooOOOooo Event # " << evtID << " oooOOOooo" << G4endl;
  G4cout << "GeneratePrimaries from source " << bkgFileName << G4endl;

  // open the file and find the source histograms
  // for each background species (mu+, mu-, n)
  bkgFile = new TFile(bkgFileName, "read");
  if (!bkgFile->IsOpen() || bkgFile->IsZombie()) {
    G4cout << "Cannot open file : " << bkgFileName << G4endl;
    exit(1);
  }

  // for each background species available...
  for(auto const name : speciesList) {
    
    std::string hxyE_path = name + "/hxyE_" + name;
    std::string hdir_path = name + "/hdir_" + name;

    // get the input histograms
    // 3D histo in (x, y, E): normalized, to be used for the main extraction
    // 3D histo in (xdircos, ydircos, E): one energy is extracted, 

    hxyE = (TH3D*)bkgFile->Get(hxyE_path.c_str());
    hdir = (TH3D*)bkgFile->Get(hdir_path.c_str());

    // get total number of particles to shoot
    // TODO: currently launching the equivalent of a "spill" = full LHC orbit
    // This should be probably rephrased in terms of luminosity
    
    int nparticles = ExtractBackgroundParticlesPerOrbit();
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
      double t = 0.; // TODO: how to imprint the bunch-crossing timing structure?
      double x, y, E;
      hxyE->GetRandom3(x, y, E); //pos in cm, E in GeV

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
  
      // now we shoot all the final state particles into the Geant4 event
      ShootParticle( anEvent, pdg, x4, p4);
    }
  
    G4cout << "*** " << name << " background done!" << G4endl;
  }

  bkgFile->Close();
}

