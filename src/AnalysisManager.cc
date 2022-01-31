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
  evt->Branch("evtID"                  , &evtID             , "evtID/I");

  evt->Branch("nuIntType"              , &nuIntType         , "nuIntType/I");
  evt->Branch("nuScatteringType"       , &nuScatteringType  , "nuScatteringType/I");
  evt->Branch("nuFSLPDG"               , &nuFSLPDG          , "nuFSLPDG/I");
  evt->Branch("nuFSLPx"                , &nuFSLPx           , "nuFSLPx/D");
  evt->Branch("nuFSLPy"                , &nuFSLPy           , "nuFSLPy/D");
  evt->Branch("nuFSLPz"                , &nuFSLPz           , "nuFSLPz/D");
  evt->Branch("nuFSLE"                 , &nuFSLE            , "nuFSLE/D");

  evt->Branch("edepInLAr"              , &edepInLAr         , "edepInLAr/D");
  evt->Branch("edepInHadCalX"          , &edepInHadCalX     , "edepInHadCalX/D");
  evt->Branch("edepInHadCalY"          , &edepInHadCalY     , "edepInHadCalY/D");

  evt->Branch("nPrimaryParticle"       , &nPrimaryParticle  , "nPrimaryParticle/I");
  evt->Branch("PDG[nPrimaryParticle]"  , PDG                , "PDG[nPrimaryParticle]/I");
  evt->Branch("vtxX[nPrimaryParticle]" , vtxX               , "vtxX[nPrimaryParticle]/D");
  evt->Branch("vtxY[nPrimaryParticle]" , vtxY               , "vtxY[nPrimaryParticle]/D");
  evt->Branch("vtxZ[nPrimaryParticle]" , vtxZ               , "vtxZ[nPrimaryParticle]/D");
  evt->Branch("Px[nPrimaryParticle]"   , Px                 , "Px[nPrimaryParticle]/D");
  evt->Branch("Py[nPrimaryParticle]"   , Py                 , "Py[nPrimaryParticle]/D");
  evt->Branch("Pz[nPrimaryParticle]"   , Pz                 , "Pz[nPrimaryParticle]/D");
  evt->Branch("Pmass[nPrimaryParticle]", Pmass               , "Pmass[nPrimaryParticle]/D");

  evt->Branch("primaryTrackLength"     , &primaryTrackLength, "primaryTrackLength/D");
  evt->Branch("nFSParticles"           , &nFSParticles      , "nFSParticles/I");
  evt->Branch("fsParticlePDG"          , fsParticlePDG      , "fsParticlePDG[nFSParticles]/I");
  evt->Branch("fsParticleKinE"         , fsParticleKinE     , "fsParticleKinE[nFSParticles]/D");
  evt->Branch("fsParticlePx"           , fsParticlePx       , "fsParticlePx[nFSParticles]/D");
  evt->Branch("fsParticlePy"           , fsParticlePy       , "fsParticlePy[nFSParticles]/D");
  evt->Branch("fsParticlePz"           , fsParticlePz       , "fsParticlePz[nFSParticles]/D");
  evt->Branch("nSecondaryTracks"       , &nSecondaryTracks  , "nSecondaryTracks/I");
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
    hEdepZX = new TH2D("EdepZX", "EdepZX", 1700, 0, 8500, 200, -500, 500);
    hEdepZY = new TH2D("EdepZY", "EdepZY", 1700, 0, 8500, 200, -500, 500);
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
  nuIntType          = -1;
  nuScatteringType   = -1;
  nuFSLPDG           = 0;
  nuFSLPx            = -999;
  nuFSLPy            = -999;
  nuFSLPz            = -999;
  nuFSLE             = -999;
  edepInLAr          = 0;
  edepInHadCalX      = 0;
  edepInHadCalY      = 0;
  nPrimaryParticle   = 0;
  nFSParticles       = 0;
  primaryTrackLength = 0;
  nSecondaryTracks   = 0;
  for (G4int i= 0; i< 2000000; ++i) {
    PDG[i]   = 0;
    vtxX[i]  = -999;
    vtxY[i]  = -999;
    vtxZ[i]  = -999;
    Px[i]    = -999;
    Py[i]    = -999;
    Pz[i]    = -999;
    Pmass[i] = -999;
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
        if (primary_particle_info->GetPartID()==0) {
          nuIntType        = primary_particle_info->GetInteractionTypeId();
          nuScatteringType = primary_particle_info->GetScatteringTypeId();
          nuFSLPDG         = primary_particle_info->GetFSLPDG();
          nuFSLPx          = primary_particle_info->GetFSLP4().X();
          nuFSLPy          = primary_particle_info->GetFSLP4().Y();
          nuFSLPz          = primary_particle_info->GetFSLP4().Z();
          nuFSLE           = primary_particle_info->GetFSLP4().T();
        }
        PDG[count_particles]   = primary_particle_info->GetPDG();
        vtxX[count_particles]  = primary_particle_info->GetVertexMC().getX();
        vtxY[count_particles]  = primary_particle_info->GetVertexMC().getY();
        vtxZ[count_particles]  = primary_particle_info->GetVertexMC().getZ();
        Px[count_particles]    = primary_particle_info->GetMomentumMC().getX();
        Py[count_particles]    = primary_particle_info->GetMomentumMC().getY();
        Pz[count_particles]    = primary_particle_info->GetMomentumMC().getZ();
        Pmass[count_particles] = primary_particle_info->GetMass();
        count_particles++;
      }
    }
  }
  nPrimaryVertex   = event->GetNumberOfPrimaryVertex();
  nPrimaryParticle = count_particles;
  std::cout<<"number of primary vertices  : "<<nPrimaryVertex  <<std::endl;
  std::cout<<"number of primary particles : "<<nPrimaryParticle<<std::endl;

  G4SDManager* sdm = G4SDManager::GetSDMpointer();
  // Get the hit collections
  G4HCofThisEvent* hcofEvent = event->GetHCofThisEvent();
  // If there is no hit collection, there is nothing to be done
  if (!hcofEvent) return;

  if (flArBoxSDId< 0) {
    flArBoxSDId = sdm->GetCollectionID("lArBoxSD/lar_box");
    G4cout<<"AnalysisManager: LArBox SD ID: "<<flArBoxSDId<<G4endl;
  }
  if (flArBoxSDId >= 0) {
    // Get and cast hit collection with LArBoxHits
    LArBoxHitsCollection* hitCollection = dynamic_cast<LArBoxHitsCollection*>(hcofEvent->GetHC(flArBoxSDId));
    if (hitCollection) {
      for (auto hit: *hitCollection->GetVector()) {
        edepInLAr += hit->GetEdep();
        if (hit->GetCreatorProcess()=="PrimaryParticle") {
          primaryTrackLength += hit->GetStepLength();
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
    }
  }

  // HadCalXSD
  if (fHadCalXSDId< 0) {
    fHadCalXSDId = sdm->GetCollectionID("HadCalXSD/lar_box");
    G4cout<<"AnalysisManager: HadCalX SD ID: "<<fHadCalXSDId<<G4endl;
  }
  if (fHadCalXSDId>= 0) {
    // Get and cast hit collection with HadCalXHits
    LArBoxHitsCollection* hitCollection = dynamic_cast<LArBoxHitsCollection*>(hcofEvent->GetHC(fHadCalXSDId));
    if (hitCollection) {
      for (auto hit: *hitCollection->GetVector()) {
        edepInHadCalX += hit->GetEdep();
        if (hit->GetCreatorProcess()=="PrimaryParticle") {
          primaryTrackLength += hit->GetStepLength();
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
    }
  }

  // HadCalYSD
  if (fHadCalYSDId< 0) {
    fHadCalYSDId = sdm->GetCollectionID("HadCalYSD/lar_box");
    G4cout<<"AnalysisManager: HadCalY SD ID: "<<fHadCalYSDId<<G4endl;
  }
  if (fHadCalYSDId>= 0) {
    // Get and cast hit collection with HadCalYHits
    LArBoxHitsCollection* hitCollection = dynamic_cast<LArBoxHitsCollection*>(hcofEvent->GetHC(fHadCalYSDId));
    if (hitCollection) {
      for (auto hit: *hitCollection->GetVector()) {
        edepInHadCalY += hit->GetEdep();
        if (hit->GetCreatorProcess()=="PrimaryParticle") {
          primaryTrackLength += hit->GetStepLength();
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
    }
  }

  std::cout<<"Primary Track Length : "<<primaryTrackLength<<" mm"<<std::endl;

  evt->Fill();
}
