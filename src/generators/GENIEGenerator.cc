#include "generators/GeneratorBase.hh"
#include "generators/GENIEGenerator.hh"
#include "generators/GENIEGeneratorMessenger.hh"

#include "geometry/GeometricalParameters.hh"

#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4Exception.hh"
#include "Randomize.hh"

#include "TMath.h"
#include "TFile.h"
#include "TTree.h"

GENIEGenerator::GENIEGenerator()
{
  fGeneratorName = "genie";
  fMessenger = new GENIEGeneratorMessenger(this);

  fGSTFile = nullptr;
  fGSTTree = nullptr;
  fRandomVtx = true;
  fEventCounter = 0;
}

GENIEGenerator::~GENIEGenerator()
{
  delete fGSTTree;
  if(fGSTFile) fGSTFile->Close();
  delete fMessenger;
}

void GENIEGenerator::LoadData()
{

  fGSTFile = new TFile(fGSTFilename, "read");
  if (!fGSTFile->IsOpen()) {
    G4String err = "Cannot open GST file : " + fGSTFilename;
    G4Exception("GENIEGenerator", "FileError", FatalErrorInArgument, err.c_str());
  }

  fGSTTree = (TTree*)fGSTFile->Get("gst");
  if (!fGSTTree) {
    G4String err = "No GST event tree in input file : " + fGSTFilename;
    G4Exception("GENIEGenerator", "FileError", FatalErrorInArgument, err.c_str());
  }

  fNEntries = fGSTTree->GetEntries();
  G4cout << "Input GST tree has " << fNEntries << ((fNEntries==1)? " entry." : " entries.") << G4endl;

  fGSTTree->SetBranchAddress("qel",&m_qel); // is QEL?   
  fGSTTree->SetBranchAddress("mec",&m_mec); // is MEC?
  fGSTTree->SetBranchAddress("res",&m_res); // is RES?
  fGSTTree->SetBranchAddress("dis",&m_dis); // is DIS?
  fGSTTree->SetBranchAddress("coh",&m_coh); // is Coherent?
  fGSTTree->SetBranchAddress("dfr",&m_dfr); // id Diffractive?
  fGSTTree->SetBranchAddress("imd",&m_imd); // is IMD?
  fGSTTree->SetBranchAddress("imdanh",&m_imdanh); // is IMD annihilation?
  fGSTTree->SetBranchAddress("singlek",&m_singlek); // is single Kaon?
  fGSTTree->SetBranchAddress("nuel",&m_nuel);  // is ve elastic?
  fGSTTree->SetBranchAddress("em",&m_em); // is EM process?
  fGSTTree->SetBranchAddress("cc",&m_cc); // is Weak CC?
  fGSTTree->SetBranchAddress("nc",&m_nc); // is Weak NC?
  fGSTTree->SetBranchAddress("charm",&m_charm); // produces charm?
  fGSTTree->SetBranchAddress("amnugamma",&m_amnugamma); // is anomaly mediated nu gamma?

  fGSTTree->SetBranchAddress("neu",&m_neuPDG); //neutrino PDG
  fGSTTree->SetBranchAddress("Ev",&m_Ev); // neutrino energy (GeV)
  fGSTTree->SetBranchAddress("pxv",&m_pxv); // neutrino px (GeV)
  fGSTTree->SetBranchAddress("pyv",&m_pyv); // neutrino py (Gev)
  fGSTTree->SetBranchAddress("pzv",&m_pzv); // neutrino pz (GeV)
 
  fGSTTree->SetBranchAddress("fspl",&m_fslPDG); // primary letpton PDG
  fGSTTree->SetBranchAddress("El",&m_El); // primary lepton energy (GeV)
  fGSTTree->SetBranchAddress("pxl",&m_pxl); // primary lepton px (GeV)
  fGSTTree->SetBranchAddress("pyl",&m_pyl); // primary lepton py (Gev)
  fGSTTree->SetBranchAddress("pzl",&m_pzl); // primary lepton pz (GeV)
  
  fGSTTree->SetBranchAddress("nf",&m_nf); // number of final state hadrons
  fGSTTree->SetBranchAddress("pdgf",&m_pdgf); // hadrons PDG
  fGSTTree->SetBranchAddress("Ef",&m_Ef); // hadrons energy (GeV)
  fGSTTree->SetBranchAddress("pxf",&m_pxf); // hadrons px (GeV)
  fGSTTree->SetBranchAddress("pyf",&m_pyf); // hadrons py (Gev)
  fGSTTree->SetBranchAddress("pzf",&m_pzf); // hadrons pz (GeV)
  
  fGSTTree->SetBranchAddress("W",&m_W); // invariant hadronic mass (GeV)

}

G4bool GENIEGenerator::FindParticleDefinition(G4int const pdg, G4ParticleDefinition* &particleDefinition) const
{
  // unknown pgd codes in GENIE --> skip it!
  // ref: https://internal.dunescience.org/doxygen/ConvertMCTruthToG4_8cxx_source.html
  // This has been a known issue with GENIE
  const int genieLo = 2000000001;
  const int genieHi = 2000000202;
  if ( pdg >= genieLo && pdg <= genieHi) {
    G4cout<< "This unknown PDG code [" << pdg << "] was present in the GENIE input, "
             << "but will not be processed by Geant4."
             << G4endl;
    return false; // return bad
  }
  
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

  return true; //return good
}

void GENIEGenerator::GeneratePrimaries(G4Event* anEvent)
{

  // complete line from PrimaryGeneratorAction...
  G4cout << ") : GENIE Generator ===oooOOOooo===" << G4endl;
  
  G4cout << "oooOOOooo Event # " << fEventCounter << " oooOOOooo" << G4endl;
  G4cout << "GeneratePrimaries from file " << fGSTFilename << ", evtID starts from "<< fEvtStartIdx << ", now at " << fEvtStartIdx+fEventCounter << G4endl;

  G4int currentIdx = fEvtStartIdx+fEventCounter;
  anEvent->SetEventID(currentIdx);

  if ( currentIdx >= fNEntries ) {
    G4cerr << "** event index beyond range !! **" << G4endl;
  }
  
  // fetch a single entry from GENIE input file
  fGSTTree->GetEntry(currentIdx); 

  // compute/repackage what is not directly available from the tree
  // position is randomly extracted in the detector fiducial volume
  // or set to the center according to config parameter
 
  m_neuIdx = currentIdx;
  m_neuP4.SetPxPyPzE(m_pxv,m_pyv,m_pzv,m_Ev);
  m_fslP4.SetPxPyPzE(m_pxl,m_pyl,m_pzl,m_El);
  
  m_int_type = DecodeInteractionType();
  m_scattering_type = DecodeScatteringType();

  G4Random::setTheSeed(currentIdx+1);
  if(fRandomVtx){
      m_neuX4.SetX(GeometricalParameters::Get()->GetFLArEPosition().x() +
                 (G4UniformRand()-0.5) * GeometricalParameters::Get()->GetFLArEFidVolSize().x());
      m_neuX4.SetY(GeometricalParameters::Get()->GetFLArEPosition().y() +
                 (G4UniformRand()-0.5) * GeometricalParameters::Get()->GetFLArEFidVolSize().y());
      m_neuX4.SetZ(GeometricalParameters::Get()->GetFLArEPosition().z() +
                 (G4UniformRand()-0.5) * GeometricalParameters::Get()->GetFLArEFidVolSize().z());
      m_neuX4.SetT(0.);
  } else {
    m_neuX4.SetX(0.*m);
    m_neuX4.SetY(0.*m);
    m_neuX4.SetZ(GeometricalParameters::Get()->GetFLArEPosition().z() -
                GeometricalParameters::Get()->GetFLArEFidVolSize().z()/2);
    m_neuX4.SetT(0.);
  }

  // create primary vertex (neutrino)
  G4PrimaryVertex* vtx = new G4PrimaryVertex(m_neuX4.X(), m_neuX4.Y(), m_neuX4.Z(), m_neuX4.T()); // in mm

  // now add all the final state particles into the vertex
  // - final state lepton (if NC, it's the neutrino!)
  G4ParticleDefinition* particleDefinition;
  if ( FindParticleDefinition(m_fslPDG, particleDefinition) ){

    G4PrimaryParticle* plepton = new G4PrimaryParticle(particleDefinition, m_fslP4.X()*GeV, m_fslP4.Y()*GeV, m_fslP4.Z()*GeV, m_fslP4.E()*GeV); //in GeV
    /* G4cout << "Lepton PDG " << m_fslPDG << " mass " << particleDefinition->GetPDGMass()*MeV << G4endl;
    G4cout << "p4  " << m_fslP4.X() << " " << m_fslP4.Y() << " " <<  m_fslP4.Z() << " " <<  m_fslP4.E() << G4endl;
    G4cout << "kinE " << ( m_fslP4.E()*GeV - particleDefinition->GetPDGMass()*MeV)  << G4endl; */
    vtx->SetPrimary(plepton);

  }

  // - all final state hadrons
  for (int ipar=0; ipar<m_nf; ++ipar) {

    TLorentzVector p( m_pxf[ipar], m_pyf[ipar], m_pzf[ipar], m_Ef[ipar] );
    if ( !FindParticleDefinition( m_pdgf[ipar], particleDefinition) ) continue; //skip bad pdgs
    G4PrimaryParticle* prim = new G4PrimaryParticle(particleDefinition, p.X()*GeV, p.Y()*GeV, p.Z()*GeV, p.E()*GeV); //in GeV
    /* G4cout << "Particle PDG " << m_pdgf[ipar] << " mass " << particleDefinition->GetPDGMass()*MeV << G4endl;
    G4cout << "p4  " << p.X() << " " << p.Y() << " " << p.Z() << " " << p.E() << G4endl;
    G4cout << "kinE " << (p.E()*GeV - particleDefinition->GetPDGMass()*MeV)  << G4endl; */
    vtx->SetPrimary(prim);

  }

  anEvent->AddPrimaryVertex(vtx);
  fEventCounter++;

}

G4int GENIEGenerator::DecodeInteractionType() const
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
	
  if (m_cc) return kIntWeakCC;
  else if (m_nc) return kIntWeakNC;
  else if (m_em) return kIntEM;

  return kIntNull;

}

G4int GENIEGenerator::DecodeScatteringType() const
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

  if(m_qel) return kScQuasiElastic;
  else if(m_res) return kScResonant;
  else if(m_dis) return kScDeepInelastic;
  else if(m_coh) return kScCoherentProduction;
  else if(m_dfr) return kScDiffractive;
  else if(m_imd) return kScInverseMuDecay;
  else if(m_imdanh) return kScIMDAnnihilation;
  else if(m_mec) return kScMEC;
  else if(m_nuel) return kScNuElectronElastic;
  else if(m_singlek) return kScSingleKaon;
  else if(m_amnugamma) return kScAMNuGamma;

  return kScUnknown;
}
