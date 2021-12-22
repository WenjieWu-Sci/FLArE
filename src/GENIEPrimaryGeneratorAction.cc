#include "GENIEPrimaryGeneratorAction.hh"
#include "PrimaryGeneratorAction.hh"

#include <G4GeneralParticleSource.hh>
#include <G4ParticleTable.hh>
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
  for (int ipar=0; ipar<event->GetEntries(); ++ipar) {
    GHepParticle* p = event->Particle(ipar);
    if (p->Status()==1) {
      //std::cout<<p->Name()<<" "<<p->Pdg()<<" ("<<p->Vx()<<", "<<p->Vy()<<", "<<p->Vz()<<") ("
      //  <<p->Px()<<", "<<p->Py()<<", "<<p->Pz()<<")"<<std::endl;

      fGPS->SetParticleDefinition(G4ParticleTable::GetParticleTable()->FindParticle(p->Pdg()));
      fGPS->GetCurrentSource()->GetEneDist()->SetMonoEnergy(p->Energy() * GeV);
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
