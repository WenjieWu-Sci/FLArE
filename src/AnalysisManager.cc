#include "AnalysisManager.hh"
#include "LArBoxSD.hh"
#include "LArBoxHit.hh"
#include "PrimaryParticleInformation.hh"

#include <G4Event.hh>
#include <G4SDManager.hh>
#include <G4SystemOfUnits.hh>
#include <TFile.h>
#include <TTree.h>
#include <TH2D.h>
#include <vector>

AnalysisManager* AnalysisManager::instance = 0;

AnalysisManager* AnalysisManager::GetInstance() {
  if (!instance) {
    std::cout<<"AnalysisManager: Re-initialization"<<std::endl;
    instance = new AnalysisManager();
  }
  return instance;
}

AnalysisManager::AnalysisManager() {
  evt = 0;
  messenger = new AnalysisManagerMessenger(this);
  m_filename = "test.root";
  m_saveSecondary = false;
  m_saveEvd = false;
}

AnalysisManager::~AnalysisManager() {;}

void AnalysisManager::bookEvtTree() {
  evt = new TTree("evt", "evtTreeInfo");
  evt->Branch("evtID"         , &evtID         , "evtID/I");
  evt->Branch("nPrimaryVertex", &nPrimaryVertex, "nPrimaryVertex/I");
  evt->Branch("vtxX"     , &vtxX     , "vtxX/D");
  evt->Branch("vtxY"     , &vtxY     , "vtxY/D");
  evt->Branch("vtxZ"     , &vtxZ     , "vtxZ/D");
  //evt->Branch("vtxX2"    , &vtxX2    , "vtxX2/D");
  //evt->Branch("vtxY2"    , &vtxY2    , "vtxY2/D");
  //evt->Branch("vtxZ2"    , &vtxZ2    , "vtxZ2/D");
  evt->Branch("edepInLAr", &edepInLAr, "edepInLAr/D");
  evt->Branch("primaryTrackLength", &primaryTrackLength, "primaryTrackLength/D");
  evt->Branch("nFSParticles"      , &nFSParticles      , "nFSParticles/I");
  evt->Branch("fsParticlePDG"     , fsParticlePDG      , "fsParticlePDG[nFSParticles]/I");
  evt->Branch("fsParticleKinE"    , fsParticleKinE     , "fsParticleKinE[nFSParticles]/D");
  evt->Branch("fsParticlePx"      , fsParticlePx       , "fsParticlePx[nFSParticles]/D");
  evt->Branch("fsParticlePy"      , fsParticlePy       , "fsParticlePy[nFSParticles]/D");
  evt->Branch("fsParticlePz"      , fsParticlePz       , "fsParticlePz[nFSParticles]/D");
  evt->Branch("nSecondaryTracks"  , &nSecondaryTracks  , "nSecondaryTracks/I");
  if (m_saveSecondary) {
    evt->Branch("secondaryTrackPDG", secondaryTrackPDG, "secondaryTrackPDG[nSecondaryTracks]/I");
  }
}

void AnalysisManager::BeginOfRun() {
  G4cout<<"TTree is booked and the run has been started"<<G4endl;
  if (thefile) {
    delete thefile;
  }
  thefile = new TFile(m_filename, "RECREATE");
  bookEvtTree();

  if (m_saveEvd) {
    hEdepXY = new TH2D("EdepXY", "EdepXY", 200, -500, 500, 200, -500, 500);
    hEdepZX = new TH2D("EdepZX", "EdepZX", 1400, 0, 7000, 200, -500, 500);
    hEdepZY = new TH2D("EdepZY", "EdepZY", 1400, 0, 7000, 200, -500, 500);
  }
}

void AnalysisManager::EndOfRun() {
  thefile->cd();
  evt->Write();
  if (m_saveEvd) {
    hEdepXY->Write();
    hEdepZX->Write();
    hEdepZY->Write();
  }
  thefile->Close();
}

void AnalysisManager::BeginOfEvent() {
  vtxX = -999;
  vtxY = -999;
  vtxZ = -999;
  edepInLAr = 0;
  primaryTrackLength = 0;
  nFSParticles = 0;
  nSecondaryTracks = 0;
  for (G4int i= 0; i< 2000000; ++i) {
    fsParticlePDG[i]     = 0;
    fsParticleKinE[i]    = -999;
    fsParticlePx[i]      = 0;
    fsParticlePy[i]      = 0;
    fsParticlePz[i]      = 0;
    secondaryTrackPDG[i] = 0;
  }
}

void AnalysisManager::EndOfEvent(const G4Event* event) {
  /// Branch: evtID
  evtID = event->GetEventID();

  G4int count_particles = 0;
  /// loop over the vertices, and then over primary particles,
  /// primary particle MC truth info from event generator.
  for (G4int ivtx = 0; ivtx < event->GetNumberOfPrimaryVertex(); ++ivtx) {
    for (G4int ipp = 0; ipp < event->GetPrimaryVertex(ivtx)->GetNumberOfParticle(); ++ipp) {
      G4PrimaryParticle* primary_particle = 
        event->GetPrimaryVertex(ivtx)->GetPrimary(ipp);
      if (primary_particle) {
        PrimaryParticleInformation* primary_particle_info = 
          dynamic_cast<PrimaryParticleInformation*>(primary_particle->GetUserInformation());
        primary_particle_info->Print();
        count_particles++;
      }
    }
  }
  nPrimaryVertex = event->GetNumberOfPrimaryVertex();
  std::cout<<"number of primary particles : "<<count_particles<<" "<<nPrimaryVertex<<std::endl;

  G4SDManager* sdm = G4SDManager::GetSDMpointer();
  // Get the hit collections
  G4HCofThisEvent* hcofEvent = event->GetHCofThisEvent();
  // If there is no hit collection, there is nothing to be done
  if (!hcofEvent) return;
  if (flArBoxSDId< 0) {
    flArBoxSDId = sdm->GetCollectionID("lArBoxSD/lar_box");
    G4cout<<"EventAction: LArBox SD ID: "<<flArBoxSDId<<G4endl;
  }
  if (flArBoxSDId >= 0) {
    // Get and cast hit collection with LArBoxHits
    LArBoxHitsCollection* hitCollection = dynamic_cast<LArBoxHitsCollection*>(hcofEvent->GetHC(flArBoxSDId));
    if (hitCollection) {
      // Branch: vtxX, vtxY, vtxZ, edepInLAr;
      for (auto hit: *hitCollection->GetVector()) {
        edepInLAr += hit->GetEdep();
        if (hit->GetCreatorProcess()=="PrimaryParticle") {
          primaryTrackLength += hit->GetStepLength();
          if (hit->GetStepNo()==1) {
            vtxX = hit->GetTrackVertex().getX();
            vtxY = hit->GetTrackVertex().getY();
            vtxZ = hit->GetTrackVertex().getZ();
            std::cout<<vtxX<<" "<<vtxY<<" "<<vtxZ<<std::endl;
          }
        }
        if (hit->GetStepNo()==1 && hit->GetPID()==1 && hit->GetCreatorProcess()=="Decay") {
          std::cout<<"TID : "<<hit->GetTID()     <<", PID : "           <<hit->GetPID()
            <<", PDG : "<<hit->GetParticle()     <<", CreatorProcess : "<<hit->GetCreatorProcess()
            <<", Ek : " <<hit->GetInitKinEnergy()<<" MeV"              <<std::endl;
          nFSParticles++;
          if (nFSParticles<=2000000) {
            fsParticlePDG[nFSParticles-1] = hit->GetParticle();
            fsParticleKinE[nFSParticles-1] = hit->GetInitKinEnergy();
            fsParticlePx[nFSParticles-1] = hit->GetInitMomentum().getX();
            fsParticlePy[nFSParticles-1] = hit->GetInitMomentum().getY();
            fsParticlePz[nFSParticles-1] = hit->GetInitMomentum().getZ();
          }
        }
        if (hit->GetStepNo()==1 && hit->GetPID()!=0 && hit->GetParticle()!=22) {
          // record secondary particles, but exclude gamma
          nSecondaryTracks++;
          if (nSecondaryTracks<=2000000) 
            secondaryTrackPDG[nSecondaryTracks-1] = hit->GetParticle();
        }
        if (m_saveEvd) {
          hEdepXY->Fill(hit->GetEdepPosition().x(), hit->GetEdepPosition().y(), hit->GetEdep());
          hEdepZX->Fill(hit->GetEdepPosition().z()+3500, hit->GetEdepPosition().x(), hit->GetEdep());
          hEdepZY->Fill(hit->GetEdepPosition().z()+3500, hit->GetEdepPosition().y(), hit->GetEdep());
        }
      }
      std::cout<<"Primary Track Length : "<<primaryTrackLength<<" mm"<<std::endl;
    }
  }

  evt->Fill();
}
