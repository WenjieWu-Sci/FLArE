#include <ostream>

#include "PrimaryGeneratorAction.hh"
#include "PrimaryGeneratorMessenger.hh"
#include <G4ParticleTable.hh>
#include <G4Event.hh>
#include <G4SystemOfUnits.hh>
#include <G4ParticleGun.hh>
#include <Randomize.hh>
#include <G4GeneralParticleSource.hh>

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

//PrimaryGeneratorAction* PrimaryGeneratorAction::GeneratorInstance = 0;
//
//PrimaryGeneratorAction* PrimaryGeneratorAction::GetInstance() {
//  if (!GeneratorInstance) {
//    std::cout<<"PrimaryGeneratorAction: Re-initialization"<<std::endl;
//    GeneratorInstance = new PrimaryGeneratorAction();
//  }
//  return GeneratorInstance;
//}

PrimaryGeneratorAction::PrimaryGeneratorAction() {
  fGPS = new G4GeneralParticleSource();

  // create a messenger for this class
  genMessenger = new PrimaryGeneratorMessenger(this);

//  ghepFileName = "genie/nutau_ar40_100GeV.ghep.root";
//  std::cout<<"PrimaryGeneratorAction constructor : "<<ghepFileName<<" "<<ghepEvtStartIdx<<" "<<ghepEvtStopIdx<<std::endl;

  //std::cout<<"loading GHEP file : "<<ghepFileName<<std::endl;
  //ghep_file = new TFile(ghepFileName, "read");
  //ghep_tree = (TTree*)ghep_file->Get("gtree");
  //if (!ghep_tree) {
  //  std::cout<<"No GHEP event tree in input file: "<<ghepFileName<<std::endl;
  //  exit(1);
  //}
  //int nev = ghep_tree->GetEntries();
  //std::cout<<"Input GHEP tree has "<<nev<<((nev==1)? " entry." : " entries.")<<std::endl;
  //thdr = (NtpMCTreeHeader*)ghep_file->Get("header");
  //std::cout<<*thdr;

  //mcrec = new NtpMCEventRecord(); 
  //// main event record branch, always present
  //ghep_tree->SetBranchAddress("gmcrec", &mcrec);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
  //ghep_file->Close();

//  delete ghep_file;
//  delete mcrec;
  delete fGPS;
  delete genMessenger;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
  int evtID = anEvent->GetEventID();
  std::cout<<"oooOOOooo Event # "<<evtID<<" oooOOOooo"<<std::endl;
  G4cout<<"GeneratePrimaries from file "<<ghepFileName
    <<", evtID range from "<<ghepEvtStartIdx
    <<" to "<<ghepEvtStopIdx
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

  if (ghepEvtStartIdx + evtID > ghepEvtStopIdx || ghepEvtStartIdx + evtID >= nev) {
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

//  G4ParticleDefinition* myParticle;
//  myParticle = G4ParticleTable::GetParticleTable()->FindParticle("e-");
//  fGPS->SetParticleDefinition(myParticle);
//  fGPS->GetCurrentSource()->GetEneDist()->SetMonoEnergy(5*GeV);
//  fGPS->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(G4ThreeVector(0,0,1));
//
//  G4double x0 = 0. * m;
//  G4double y0 = 0. * m;
//  G4double z0 = 0. * m;
//  G4double dz = 7. * m;
//  z0 += dz*(G4UniformRand()-0.5);
//  std::cout<<x0<<" "<<y0<<" "<<z0<<std::endl;
//  fGPS->GetCurrentSource()->GetPosDist()->SetPosDisType("Point");
//  fGPS->GetCurrentSource()->GetPosDist()->SetCentreCoords(G4ThreeVector(x0, y0, z0));
//
//  fGPS->GeneratePrimaryVertex(anEvent);
}
