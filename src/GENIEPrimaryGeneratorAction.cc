#include "GENIEPrimaryGeneratorAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "geometry/GeometricalParameters.hh"

#include <G4GeneralParticleSource.hh>
#include <G4ParticleTable.hh>
#include <G4IonTable.hh>
#include <G4SystemOfUnits.hh>
#include <Randomize.hh>

#include <TMath.h>
#include <TTree.h>

GENIEPrimaryGeneratorAction::GENIEPrimaryGeneratorAction(G4GeneralParticleSource* gps)
  : fGPS(gps)
{
}

GENIEPrimaryGeneratorAction::~GENIEPrimaryGeneratorAction()
{
}

void GENIEPrimaryGeneratorAction::ShootParticle(G4Event* anEvent, G4int pdg, TLorentzVector p4)
{
  // unknown pgd codes in GENIE --> skip it!
  // ref: https://internal.dunescience.org/doxygen/ConvertMCTruthToG4_8cxx_source.html
  // This has been a known issue with GENIE
  const int genieLo = 2000000001;
  const int genieHi = 2000000202;
  if ( pdg >= genieLo && pdg <= genieHi) {
    std::cout<<"This unknown PDG code ["<<pdg<<"] was present in the GENIE input, "
             <<"but will not be processed by Geant4."
             <<std::endl;
    return;
  }
  
  G4ParticleDefinition* particleDefinition;
  if ( pdg == 0) {
    particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle("opticalphoton");
  } else {
    particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle(pdg);
  }
  
  if ( pdg > 1000000000) { // If the particle is a nucleus
    // If the particle table doesn't have a definition yet, ask the ion
    // table for one. This will create a new ion definition as needed.
    if (!particleDefinition) {
      int Z = (pdg % 10000000) / 10000; // atomic number
      int A = (pdg % 10000) / 10;       // mass number
      particleDefinition = G4ParticleTable::GetParticleTable()->GetIonTable()->GetIon(Z, A, 0.);
    }
  }
 
  /*G4cout << "Particle PDG " << pdg << " mass " << particleDefinition->GetPDGMass()*MeV << G4endl;
  G4cout << "p4  " << p4.X() << " " << p4.Y() << " " << p4.Z() << " " << p4.E() << G4endl;
  G4cout << "kinE " << (p4.E()*GeV - particleDefinition->GetPDGMass()*MeV)  << G4endl;*/

  // load the gun...
  fGPS->SetParticleDefinition(particleDefinition);
  fGPS->GetCurrentSource()->GetEneDist()->SetMonoEnergy(( p4.E()*GeV - particleDefinition->GetPDGMass()*MeV));  // kinetic energy
  fGPS->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(G4ThreeVector(p4.X(), p4.Y(), p4.Z()));
  fGPS->GetCurrentSource()->GetPosDist()->SetPosDisType("Point");
  fGPS->GetCurrentSource()->GetPosDist()->SetCentreCoords( G4ThreeVector(neuX4.X(), neuX4.Y(), neuX4.Z()) );
  fGPS->GeneratePrimaryVertex(anEvent); // ...and shoot!

}

void GENIEPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent,
    G4String filename, G4int startIdx, G4int nuVtxOpt)
{
  gstFileName    = filename;
  gstEvtStartIdx = startIdx;

  int evtID = anEvent->GetEventID();
  std::cout << "oooOOOooo Event # " << evtID << " oooOOOooo" <<std::endl;
  G4cout << "GeneratePrimaries from file " << gstFileName
    << ", evtID starts from "<< gstEvtStartIdx
    << ", now at " << gstEvtStartIdx+evtID<<G4endl;

  gst_file = new TFile(gstFileName, "read");
  if (!gst_file->IsOpen()) {
    std::cout<<"Cannot open gst file : "<<gstFileName<<std::endl;
    exit(1);
  }
  gst_tree = (TTree*)gst_file->Get("gst");
  if (!gst_tree) {
    std::cout<<"No GST event tree in input file : "<<gstFileName<<std::endl;
    exit(1);
  }
  int nev = gst_tree->GetEntries();
  std::cout<<"Input GST tree has "<<nev<<((nev==1)? " entry." : " entries.")<<std::endl;
  
  // define the branches we are interested in from the GST tree
  const int kNPmax = 250;

  double Ev, pxv, pyv, pzv;
  double El, pxl, pyl, pzl;
  int nf; int pdgf[250];
  double Ef[250], pxf[250], pyf[250], pzf[250];
  bool qel, mec, res, dis, coh, dfr, imd; 
  bool imdanh, nuel, em, cc, nc, charm; 
  bool singlek, amnugamma; 

  gst_tree->SetBranchAddress("qel",&qel); // is QEL?   
  gst_tree->SetBranchAddress("mec",&mec); // is MEC?
  gst_tree->SetBranchAddress("res",&res); // is RES?
  gst_tree->SetBranchAddress("dis",&dis); // is DIS?
  gst_tree->SetBranchAddress("coh",&coh); // is Coherent?
  gst_tree->SetBranchAddress("dfr",&dfr); // id Diffractive?
  gst_tree->SetBranchAddress("imd",&imd); // is IMD?
  gst_tree->SetBranchAddress("imdanh",&imdanh); // is IMD annihilation?
  gst_tree->SetBranchAddress("singlek",&singlek); // is single Kaon?
  gst_tree->SetBranchAddress("nuel",&nuel);  // is ve elastic?
  gst_tree->SetBranchAddress("em",&em); // is EM process?
  gst_tree->SetBranchAddress("cc",&cc); // is Weak CC?
  gst_tree->SetBranchAddress("nc",&nc); // is Weak NC?
  gst_tree->SetBranchAddress("charm",&charm); // produces charm?
  gst_tree->SetBranchAddress("amnugamma",&amnugamma); // is anomaly mediated nu gamma?

  gst_tree->SetBranchAddress("neu",&neuPDG); //neutrino PDG
  gst_tree->SetBranchAddress("Ev",&Ev); // neutrino energy (GeV)
  gst_tree->SetBranchAddress("pxv",&pxv); // neutrino px (GeV)
  gst_tree->SetBranchAddress("pyv",&pyv); // neutrino py (Gev)
  gst_tree->SetBranchAddress("pzv",&pzv); // neutrino pz (GeV)
 
  gst_tree->SetBranchAddress("fspl",&fslPDG); // primary letpton PDG
  gst_tree->SetBranchAddress("El",&El); // primary lepton energy (GeV)
  gst_tree->SetBranchAddress("pxl",&pxl); // primary lepton px (GeV)
  gst_tree->SetBranchAddress("pyl",&pyl); // primary lepton py (Gev)
  gst_tree->SetBranchAddress("pzl",&pzl); // primary lepton pz (GeV)
  
  gst_tree->SetBranchAddress("nf",&nf); // number of final state hadrons
  gst_tree->SetBranchAddress("pdgf",&pdgf); // hadrons PDG
  gst_tree->SetBranchAddress("Ef",&Ef); // hadrons energy (GeV)
  gst_tree->SetBranchAddress("pxf",&pxf); // hadrons px (GeV)
  gst_tree->SetBranchAddress("pyf",&pyf); // hadrons py (Gev)
  gst_tree->SetBranchAddress("pzf",&pzf); // hadrons pz (GeV)
  
  gst_tree->SetBranchAddress("W",&W); // invariant hadronic mass (GeV)

  if (gstEvtStartIdx + evtID >= nev) {
    std::cout<<"** event index beyond range !! **"<<std::endl;
  }
  
  // fetch a single entry from GENIE input file
  gst_tree->GetEntry(gstEvtStartIdx + evtID); 

  // compute/repackage what is not directly available from the tree
  // position is randomly extracted in the detector fiducial volume
  // or set set to the center according to config parameter
 
  neuIdx = gstEvtStartIdx + evtID;
  neuP4.SetPxPyPzE(pxv,pyv,pzv,Ev);
  fslP4.SetPxPyPzE(pxl,pyl,pzl,El);
  
  int_type = DecodeInteractionType(cc,nc,em);
  scattering_type = DecodeScatteringType(qel,res,dis,coh,dfr,imd,imdanh,mec,nuel,singlek,amnugamma);

  G4Random::setTheSeed(gstEvtStartIdx+evtID+1);
  switch(nuVtxOpt) {
    case 0:
      neuX4.SetX(0.*m);
      neuX4.SetY(0.*m);
      neuX4.SetZ(GeometricalParameters::Get()->GetFLArEPosition().z() -
                GeometricalParameters::Get()->GetFLArEFidVolSize().z()/2);
      neuX4.SetT(0.);
      break;
    case 1:
      neuX4.SetX(GeometricalParameters::Get()->GetFLArEPosition().x() +
                 (G4UniformRand()-0.5) * GeometricalParameters::Get()->GetFLArEFidVolSize().x());
      neuX4.SetY(GeometricalParameters::Get()->GetFLArEPosition().y() +
                 (G4UniformRand()-0.5) * GeometricalParameters::Get()->GetFLArEFidVolSize().y());
      neuX4.SetZ(GeometricalParameters::Get()->GetFLArEPosition().z() +
                 (G4UniformRand()-0.5) * GeometricalParameters::Get()->GetFLArEFidVolSize().z());
      neuX4.SetT(0.);
      break;
    case 2:
      neuX4.SetX(GeometricalParameters::Get()->GetSamplingCaloPosition().x() +
                 (G4UniformRand()-0.5) * GeometricalParameters::Get()->GetSamplingCaloFidVolSize().x());
      neuX4.SetY(GeometricalParameters::Get()->GetSamplingCaloPosition().y() +
                 (G4UniformRand()-0.5) * GeometricalParameters::Get()->GetSamplingCaloFidVolSize().y());
      neuX4.SetZ(GeometricalParameters::Get()->GetSamplingCaloPosition().z() +
                 (G4UniformRand()-0.5) * GeometricalParameters::Get()->GetSamplingCaloFidVolSize().z());
      neuX4.SetT(0.);
      break;
  }

  // now we shoot all the final state particles into the Geant4 event
  // - final state lepton (if NC, it's the neutrino!)
  // - all final state hadrons
  
  ShootParticle( anEvent, fslPDG, fslP4);
  for (int ipar=0; ipar<nf; ++ipar) {
    TLorentzVector p( pxf[ipar], pyf[ipar], pzf[ipar], Ef[ipar] );
    ShootParticle( anEvent, pdgf[ipar], p );
  }

  gst_file->Close();
}

G4int GENIEPrimaryGeneratorAction::DecodeInteractionType(bool cc, bool nc, bool em)
{
  // using ref: https://internal.dunescience.org/doxygen/Generator_2src_2Framework_2Interaction_2InteractionType_8h_source.html
  // returning the value of the corresponding enum member from the flags that are in output.
  // there are no flags in the gst tree for some of the interaction types...
 
  enum InteractionType {
    kIntNull   = 0,
    kIntEM,          //
    kIntWeakCC,      //
    kIntWeakNC,      //
    kIntWeakMix,     // CC + NC + interference
    kIntDarkMatter,  //
    kIntNDecay,      //
    kIntNOsc,        //
    kIntNHL,         //
    kIntDarkNC       // 
  };
	
  if (cc) return kIntWeakCC;
  else if (nc) return kIntWeakNC;
  else if (em) return kIntEM;

  return kIntNull;

}

G4int GENIEPrimaryGeneratorAction::DecodeScatteringType(bool qel, bool res, bool dis, bool coh, bool dfr, 
						       bool imd, bool imdanh, bool mec, bool nuel,
	       					       bool singlek, bool amnugamma)
{
  // using ref: https://internal.dunescience.org/doxygen/ScatteringType_8h_source.html
  // returning the value of the corresponding enum member from the flags that are in output.
  // there are no flags in the gst tree for some of the scattering types...
  
  enum ScatteringType {
    kScUnknown = -100,
    kScNull = 0,
    kScQuasiElastic,
    kScSingleKaon,
    kScDeepInelastic,
    kScResonant,
    kScCoherentProduction,
    kScDiffractive,
    kScNuElectronElastic,
    kScInverseMuDecay,
    kScAMNuGamma,
    kScMEC,
    kScCoherentElastic,
    kScInverseBetaDecay,
    kScGlashowResonance,
    kScIMDAnnihilation,
    kScPhotonCoherent,
    kScPhotonResonance,
    kScSinglePion,
    kScDarkMatterElastic = 101,
    kScDarkMatterDeepInelastic,
    kScDarkMatterElectron,
    kScNorm
  };

  if(qel) return kScQuasiElastic;
  else if(res) return kScResonant;
  else if(dis) return kScDeepInelastic;
  else if(coh) return kScCoherentProduction;
  else if(dfr) return kScDiffractive;
  else if(imd) return kScInverseMuDecay;
  else if(imdanh) return kScIMDAnnihilation;
  else if(mec) return kScMEC;
  else if(nuel) return kScNuElectronElastic;
  else if(singlek) return kScSingleKaon;
  else if(amnugamma) return kScAMNuGamma;

  return kScUnknown;
}
