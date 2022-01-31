#include "GENIEPrimaryGeneratorAction.hh"
#include "PrimaryGeneratorAction.hh"

#include <G4GeneralParticleSource.hh>
#include <G4ParticleTable.hh>
#include <G4IonTable.hh>
//#include <G4Event.hh>
#include <G4SystemOfUnits.hh>
//#include <G4ParticleGun.hh>
//#include <Randomize.hh>
//#include <G4GeneralParticleSource.hh>

#include <TMath.h>
#include <TLeaf.h>
#include <TBranchElement.h>
#include <TBranchObject.h>

#include "Framework/Conventions/GBuild.h"
#include "Framework/EventGen/EventRecord.h"
#include "Framework/Ntuple/NtpMCFormat.h"
#include "Framework/Ntuple/NtpMCTreeHeader.h"
#include "Framework/Ntuple/NtpMCEventRecord.h"
#include "Framework/GHEP/GHepParticle.h"
#include "Framework/Messenger/Messenger.h"
#include "Framework/ParticleData/PDGLibrary.h"
#include "Framework/Utils/CmdLnArgParser.h"
#include "Framework/Utils/RunOpt.h"
#include "Framework/Interaction/ProcessInfo.h"

using namespace genie;

GENIEPrimaryGeneratorAction::GENIEPrimaryGeneratorAction(G4GeneralParticleSource* gps)
  : fGPS(gps)
{
}

GENIEPrimaryGeneratorAction::~GENIEPrimaryGeneratorAction()
{
}

void GENIEPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent,
    G4String filename, G4int startIdx) {
  ghepFileName    = filename;
  ghepEvtStartIdx = startIdx;

  int evtID = anEvent->GetEventID();
  std::cout<<"oooOOOooo Event # "<<evtID<<" oooOOOooo"<<std::endl;
  G4cout<<"GeneratePrimaries from file "<<ghepFileName
    <<", evtID starts from "<<ghepEvtStartIdx
    <<", now at "<<ghepEvtStartIdx+evtID<<G4endl;

  ghep_file = new TFile(ghepFileName, "read");
  if (!ghep_file->IsOpen()) {
    std::cout<<"Cannot open ghep file : "<<ghepFileName<<std::endl;
    exit(1);
  }
  ghep_tree = (TTree*)ghep_file->Get("gtree");
  if (!ghep_tree) {
    std::cout<<"No GHEP event tree in input file : "<<ghepFileName<<std::endl;
    exit(1);
  }
  int nev = ghep_tree->GetEntries();
  std::cout<<"Input GHEP tree has "<<nev<<((nev==1)? " entry." : " entries.")<<std::endl;
  //thdr = (NtpMCTreeHeader*)ghep_file->Get("header");
  //std::cout<<*thdr;

  mcrec = new NtpMCEventRecord(); 
  // main event record branch, always present
  ghep_tree->SetBranchAddress("gmcrec", &mcrec);
  mcrec->Clear();               // don't leak previously fetched info

  if (ghepEvtStartIdx + evtID >= nev) {
    std::cout<<"** event index beyond range !! **"<<std::endl;
  }
  ghep_tree->GetEntry(ghepEvtStartIdx + evtID);   // fetch a single entry from GENIE input file
  // retrieve GHEP event record abd print it out.
  EventRecord* event = mcrec->event;

  const ProcessInfo procInfo = event->Summary()->ProcInfo();
  int_type        = procInfo.InteractionTypeId();
  scattering_type = procInfo.ScatteringTypeId();
  InteractionType _int_type;
  ScatteringType _scattering_type;
  std::cout<<_int_type.AsString(int_type)<<" "
           <<_scattering_type.AsString(scattering_type)<<std::endl;

  for (int ipar=0; ipar<event->GetEntries(); ++ipar) {
    GHepParticle* p = event->Particle(ipar);

    // ref: https://internal.dunescience.org/doxygen/ConvertMCTruthToG4_8cxx_source.html
    // unknown pgd codes in GENIE
    // This has been a known issue with GENIE
    const int genieLo = 2000000001;
    const int genieHi = 2000000202;
    if (p->Pdg() >= genieLo && p->Pdg() <= genieHi) {
      std::cout<<"This unknown PDG code ["<<p->Name()<<" ("<<p->Pdg()<<")] was present in the GENIE input, "
               <<"but not processed by Geant4."
               <<std::endl;
      continue;
    }
    //std::cout<<p->Status()<<" "<<p->Name()<<" "<<p->Pdg()<<" ("<<p->Vx()<<", "<<p->Vy()<<", "<<p->Vz()<<") ("
    //  <<p->Px()<<", "<<p->Py()<<", "<<p->Pz()<<")"<<std::endl;
    if (p->Status()==1) {
      G4ParticleDefinition* particleDefinition;
      if (p->Pdg() == 0) {
        particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle("opticalphoton");
      } else {
        particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle(p->Pdg());
      }
      if (p->Pdg() > 1000000000) { // If the particle is a nucleus
        // If the particle table doesn't have a definition yet, ask the ion
        // table for one. This will create a new ion definition as needed.
        if (!particleDefinition) {
          int Z = (p->Pdg() % 10000000) / 10000; // atomic number
          int A = (p->Pdg() % 10000) / 10; // mass number
          particleDefinition = G4ParticleTable::GetParticleTable()->GetIonTable()->GetIon(Z, A, 0.);
        }
      }
      fGPS->SetParticleDefinition(particleDefinition);
      fGPS->GetCurrentSource()->GetEneDist()->SetMonoEnergy((p->Energy() - p->Mass()) * GeV);  // kinetic energy
      fGPS->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(G4ThreeVector(p->Px(), p->Py(), p->Pz()));
      fGPS->GetCurrentSource()->GetPosDist()->SetPosDisType("Point");
      fGPS->GetCurrentSource()->GetPosDist()->SetCentreCoords(G4ThreeVector(p->Vx() * mm, p->Vy() * mm, p->Vz() * mm - 3.0 * m));
      fGPS->GeneratePrimaryVertex(anEvent);
    }
  }
  NtpMCRecHeader rec_header = mcrec->hdr;
  LOG("gevdump", pNOTICE) 
     << " ** Event: " << rec_header.ievent 
     << *event;

  mcrec->Clear();               // don't leak previously fetched info
  ghep_file->Close();
}
