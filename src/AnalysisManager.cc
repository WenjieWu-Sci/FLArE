#include <vector>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

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
  evt->Branch("evtID"                                    , &evtID                  , "evtID/I");
  evt->Branch("nuPDG"                                    , &nuPDG                  , "nuPDG/I");
  evt->Branch("nuE"                                      , &nuE                    , "nuE/D");
  evt->Branch("nuX"                                      , &nuX                    , "nuX/D");
  evt->Branch("nuY"                                      , &nuY                    , "nuY/D");
  evt->Branch("nuZ"                                      , &nuZ                    , "nuZ/D");
  evt->Branch("nuIntType"                                , &nuIntType              , "nuIntType/I");
  evt->Branch("nuScatteringType"                         , &nuScatteringType       , "nuScatteringType/I");
  evt->Branch("nuFSLPDG"                                 , &nuFSLPDG               , "nuFSLPDG/I");
  evt->Branch("nuFSLPx"                                  , &nuFSLPx                , "nuFSLPx/D");
  evt->Branch("nuFSLPy"                                  , &nuFSLPy                , "nuFSLPy/D");
  evt->Branch("nuFSLPz"                                  , &nuFSLPz                , "nuFSLPz/D");
  evt->Branch("nuFSLE"                                   , &nuFSLE                 , "nuFSLE/D");
  evt->Branch("nPrimaryParticle"                         , &nPrimaryParticle       , "nPrimaryParticle/I");
  evt->Branch("PDG[nPrimaryParticle]"                    , PDG                     , "PDG[nPrimaryParticle]/I");
  evt->Branch("vtxX[nPrimaryParticle]"                   , vtxX                    , "vtxX[nPrimaryParticle]/D");
  evt->Branch("vtxY[nPrimaryParticle]"                   , vtxY                    , "vtxY[nPrimaryParticle]/D");
  evt->Branch("vtxZ[nPrimaryParticle]"                   , vtxZ                    , "vtxZ[nPrimaryParticle]/D");
  evt->Branch("Px[nPrimaryParticle]"                     , Px                      , "Px[nPrimaryParticle]/D");
  evt->Branch("Py[nPrimaryParticle]"                     , Py                      , "Py[nPrimaryParticle]/D");
  evt->Branch("Pz[nPrimaryParticle]"                     , Pz                      , "Pz[nPrimaryParticle]/D");
  evt->Branch("Pmass[nPrimaryParticle]"                  , Pmass                   , "Pmass[nPrimaryParticle]/D");
  evt->Branch("primaryTrackID[nPrimaryParticle]"         , primaryTrackID          , "primaryTrackID[nPrimaryParticle]/I");
  evt->Branch("primaryTrackPDG[nPrimaryParticle]"        , primaryTrackPDG         , "primaryTrackPDG[nPrimaryParticle]/I");
  evt->Branch("primaryTrackLength[nPrimaryParticle]"     , primaryTrackLength      , "primaryTrackLength[nPrimaryParticle]/D");
  evt->Branch("primaryTrackLengthInTPC[nPrimaryParticle]", primaryTrackLengthInTPC , "primaryTrackLengthInTPC[nPrimaryParticle]/D");

  evt->Branch("edepInLAr"              , &edepInLAr         , "edepInLAr/D");
  evt->Branch("edepInHadCalX"          , &edepInHadCalX     , "edepInHadCalX/D");
  evt->Branch("edepInHadCalY"          , &edepInHadCalY     , "edepInHadCalY/D");
  evt->Branch("edepInMuonFinderX"      , &edepInMuonFinderX , "edepInMuonFinderX/D");
  evt->Branch("edepInMuonFinderY"      , &edepInMuonFinderY , "edepInMuonFinderY/D");

  evt->Branch("nFromFSLParticles"      , &nFromFSLParticles      , "nFromFSLParticles/I");
  evt->Branch("fromFSLParticlePDG"     , fromFSLParticlePDG      , "fromFSLParticlePDG[nFromFSLParticles]/I");
  evt->Branch("fromFSLParticleKinE"    , fromFSLParticleKinE     , "fromFSLParticleKinE[nFromFSLParticles]/D");
  evt->Branch("fromFSLParticlePx"      , fromFSLParticlePx       , "fromFSLParticlePx[nFromFSLParticles]/D");
  evt->Branch("fromFSLParticlePy"      , fromFSLParticlePy       , "fromFSLParticlePy[nFromFSLParticles]/D");
  evt->Branch("fromFSLParticlePz"      , fromFSLParticlePz       , "fromFSLParticlePz[nFromFSLParticles]/D");
  evt->Branch("fromFSLTrackLength"     , fromFSLTrackLength      , "fromFSLTrackLength[nFromFSLParticles]/D");
  evt->Branch("fromFSLTrackLengthInTPC", fromFSLTrackLengthInTPC , "fromFSLTrackLengthInTPC[nFromFSLParticles]/D");

  evt->Branch("nStepsIn25cm"      , &nStepsIn25cm     , "nStepsIn25cm/I");
  evt->Branch("StepPIDIn25cm"     , StepPIDIn25cm     , "StepPIDIn25cm[nStepsIn25cm]/I");
  evt->Branch("StepTIDIn25cm"     , StepTIDIn25cm     , "StepTIDIn25cm[nStepsIn25cm]/I");
  evt->Branch("StepPDGCodeIn25cm" , StepPDGCodeIn25cm , "StepPDGCodeIn25cm[nStepsIn25cm]/I");
  evt->Branch("StepnoIn25cm"      , StepnoIn25cm      , "StepnoIn25cm[nStepsIn25cm]/I");
  evt->Branch("StepPrePosIn25cmX" , StepPrePosIn25cmX , "StepPrePosIn25cmX[nStepsIn25cm]/D");
  evt->Branch("StepPrePosIn25cmY" , StepPrePosIn25cmY , "StepPrePosIn25cmY[nStepsIn25cm]/D");
  evt->Branch("StepPrePosIn25cmZ" , StepPrePosIn25cmZ , "StepPrePosIn25cmZ[nStepsIn25cm]/D");
  evt->Branch("StepPostPosIn25cmX", StepPostPosIn25cmX, "StepPostPosIn25cmX[nStepsIn25cm]/D");
  evt->Branch("StepPostPosIn25cmY", StepPostPosIn25cmY, "StepPostPosIn25cmY[nStepsIn25cm]/D");
  evt->Branch("StepPostPosIn25cmZ", StepPostPosIn25cmZ, "StepPostPosIn25cmZ[nStepsIn25cm]/D");
  evt->Branch("StepEdepIn25cm"    , StepEdepIn25cm    , "StepEdepIn25cm[nStepsIn25cm]/D");

  evt->Branch("nSecondaryTracks"       , &nSecondaryTracks       , "nSecondaryTracks/I");
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
  if (m_saveEvd) thefile->mkdir("edep");
}

void AnalysisManager::EndOfRun() {
  thefile->cd();
  evt->Write();
  thefile->Close();
}

void AnalysisManager::BeginOfEvent() {
  nuPDG              = 0;
  nuE                = -999;
  nuX                = -999;
  nuY                = -999;
  nuZ                = -999;
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
  edepInMuonFinderX  = 0;
  edepInMuonFinderY  = 0;
  nPrimaryParticle   = 0;
  nFromFSLParticles  = 0;
  nSecondaryTracks   = 0;
  nStepsIn25cm       = 0;
  for (G4int i= 0; i< 2000000; ++i) {
    PDG[i]   = 0;
    vtxX[i]  = -999;
    vtxY[i]  = -999;
    vtxZ[i]  = -999;
    Px[i]    = -999;
    Py[i]    = -999;
    Pz[i]    = -999;
    Pmass[i] = -999;
    primaryTrackID[i]          = -1;
    primaryTrackPDG[i]         = 0;
    primaryTrackLength[i]      = 0;
    primaryTrackLengthInTPC[i] = 0;
    fromFSLParticlePDG[i]      = 0;
    fromFSLParticleKinE[i]     = -999;
    fromFSLParticlePx[i]       = 0;
    fromFSLParticlePy[i]       = 0;
    fromFSLParticlePz[i]       = 0;
    fromFSLTrackLength[i]      = 0;
    fromFSLTrackLengthInTPC[i] = 0;
    StepPIDIn25cm[i]           = -1;
    StepTIDIn25cm[i]           = -1;
    StepPDGCodeIn25cm[i]       = 0;
    StepnoIn25cm[i]            = 0;
    StepPrePosIn25cmX[i]       = -999;
    StepPrePosIn25cmY[i]       = -999;
    StepPrePosIn25cmZ[i]       = -999;
    StepPostPosIn25cmX[i]      = -999;
    StepPostPosIn25cmY[i]      = -999;
    StepPostPosIn25cmZ[i]      = -999;
    StepEdepIn25cm[i]          = 0;
    secondaryTrackPDG[i]       = 0;
  }
}

void AnalysisManager::EndOfEvent(const G4Event* event) {
  /// Branch: evtID
  evtID = event->GetEventID();
  if (m_saveEvd) {
    std::string histname = "evt"+std::to_string(evtID)+"_EdepXY";
    hEdepXY = new TH2D(histname.c_str(), histname.c_str(), 600, -1500, 1500, 600, -1500, 1500);
    histname = "evt"+std::to_string(evtID)+"_EdepZX";
    hEdepZX = new TH2D(histname.c_str(), histname.c_str(), 1740, 0, 8700, 600, -1500, 1500);
    histname = "evt"+std::to_string(evtID)+"_EdepZY";
    hEdepZY = new TH2D(histname.c_str(), histname.c_str(), 1740, 0, 8700, 600, -1500, 1500);
  }

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
          nuPDG            = primary_particle_info->GetNeuPDG();
          nuE              = primary_particle_info->GetNeuP4().T();
          nuX              = primary_particle_info->GetNeuX4().X();
          nuY              = primary_particle_info->GetNeuX4().Y();
          nuZ              = primary_particle_info->GetNeuX4().Z();
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
  hcofEvent = event->GetHCofThisEvent();
  // If there is no hit collection, there is nothing to be done
  if (!hcofEvent) return;

  // Main detector
  if (flArBoxSDId< 0) {
    flArBoxSDId = sdm->GetCollectionID("lArBoxSD/lar_box");
    G4cout<<"AnalysisManager: LArBox SD ID: "<<flArBoxSDId<<G4endl;
  }
  if (flArBoxSDId >= 0) {
    FillTree(flArBoxSDId, "LArTPC");
  }

  // HadCalXSD
  if (fHadCalXSDId< 0) {
    fHadCalXSDId = sdm->GetCollectionID("HadCalXSD/lar_box");
    G4cout<<"AnalysisManager: HadCalX SD ID: "<<fHadCalXSDId<<G4endl;
  }
  if (fHadCalXSDId>= 0) {
    FillTree(fHadCalXSDId, "HadCalX");
  }

  // HadCalYSD
  if (fHadCalYSDId< 0) {
    fHadCalYSDId = sdm->GetCollectionID("HadCalYSD/lar_box");
    G4cout<<"AnalysisManager: HadCalY SD ID: "<<fHadCalYSDId<<G4endl;
  }
  if (fHadCalYSDId>= 0) {
    FillTree(fHadCalYSDId, "HadCalY");
  }

  // MuonFinderXSD
  if (fMuonFinderXSDId< 0) {
    fMuonFinderXSDId = sdm->GetCollectionID("MuonFinderXSD/lar_box");
    G4cout<<"AnalysisManager: MuonFinderX SD ID: "<<fMuonFinderXSDId<<G4endl;
  }
  if (fMuonFinderXSDId>= 0) {
    FillTree(fMuonFinderXSDId, "MuonFinderX");
  }

  // MuonFinderYSD
  if (fMuonFinderYSDId< 0) {
    fMuonFinderYSDId = sdm->GetCollectionID("MuonFinderYSD/lar_box");
    G4cout<<"AnalysisManager: MuonFinderY SD ID: "<<fMuonFinderYSDId<<G4endl;
  }
  if (fMuonFinderYSDId>= 0) {
    FillTree(fMuonFinderYSDId, "MuonFinderY");
  }

  evt->Fill();

  if (m_saveEvd) {
    thefile->cd("/edep/");
    hEdepXY->Write();
    hEdepZX->Write();
    hEdepZY->Write();
  }
  std::cout<<"nStepsIn25cm : "<<nStepsIn25cm<<std::endl;
}

void AnalysisManager::FillTree(G4int sdId, std::string sdName) {
  int detID = 0;
  const std::unordered_map<std::string,std::function<void()>> detIDs{
          {"LArTPC",      [&](){ detID = 1; }},
          {"HadCalX",     [&](){ detID = 2; }},
          {"HadCalY",     [&](){ detID = 3; }},
          {"MuonFinderX", [&](){ detID = 4; }},
          {"MuonFinderY", [&](){ detID = 5; }},
  };
  detIDs.find(sdName)->second();
  // Get and cast hit collection with LArBoxHits
  LArBoxHitsCollection* hitCollection = dynamic_cast<LArBoxHitsCollection*>(hcofEvent->GetHC(sdId));
  if (hitCollection) {
    for (auto hit: *hitCollection->GetVector()) {
      switch(detID) {
        case 1:
          edepInLAr += hit->GetEdep();
          break;
        case 2:
          edepInHadCalX += hit->GetEdep();
          break;
        case 3:
          edepInHadCalY += hit->GetEdep();
          break;
        case 4:
          edepInMuonFinderX += hit->GetEdep();
          break;
        case 5:
          edepInMuonFinderY += hit->GetEdep();
          break;
      }
      // stable final state particles in GENIE, primary particles in Geant4
      if (hit->GetCreatorProcess()=="PrimaryParticle") { // i.e. PID==0
        if (hit->GetStepNo()==1) {
          primaryTrackID[hit->GetTID()-1] = hit->GetTID();
          primaryTrackPDG[hit->GetTID()-1] = hit->GetParticle();
        }
        if (detID==1) {
          primaryTrackLength[hit->GetTID()-1] += hit->GetStepLength();
          primaryTrackLengthInTPC[hit->GetTID()-1] += hit->GetStepLength();
        } else {
          primaryTrackLength[hit->GetTID()-1] += hit->GetStepLength();
        }
      }
      // Particles decay from the final state lepton in GENIE, or decay from the primary particles in G4
      if (hit->GetPID()==1 && hit->GetCreatorProcess()=="Decay") {
        if (hit->GetStepNo()==1) {
          std::cout<<"TID : "<<hit->GetTID()     <<", PID : "           <<hit->GetPID()
            <<", PDG : "<<hit->GetParticle()     <<", CreatorProcess : "<<hit->GetCreatorProcess()
            <<", Ek : " <<hit->GetInitKinEnergy()<<" MeV"              <<std::endl;
          nFromFSLParticles++;
          if (nFromFSLParticles<=2000000) {
            fromFSLParticlePDG[nFromFSLParticles-1]  = hit->GetParticle();
            fromFSLParticleKinE[nFromFSLParticles-1] = hit->GetInitKinEnergy();
            fromFSLParticlePx[nFromFSLParticles-1]   = hit->GetInitMomentum().getX();
            fromFSLParticlePy[nFromFSLParticles-1]   = hit->GetInitMomentum().getY();
            fromFSLParticlePz[nFromFSLParticles-1]   = hit->GetInitMomentum().getZ();
          }
        }
        if (nFromFSLParticles>0) {
          if (detID==1) {
            fromFSLTrackLength[nFromFSLParticles-1] += hit->GetStepLength();
            fromFSLTrackLengthInTPC[nFromFSLParticles-1] += hit->GetStepLength();
          } else {
            fromFSLTrackLength[nFromFSLParticles-1] += hit->GetStepLength();
          }
        }
      }
      if (hit->GetStepNo()==1 && hit->GetPID()>nPrimaryParticle && hit->GetParticle()!=22) {
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
      if (abs(hit->GetPreStepPosition().x() - nuX) < 12.5 * cm &&
          abs(hit->GetPreStepPosition().y() - nuY) < 12.5 * cm &&
          hit->GetPreStepPosition().z() - nuZ < 20.0 * cm &&
          hit->GetPreStepPosition().z() - nuZ > -5.0 * cm) {
        nStepsIn25cm++;
        StepPIDIn25cm[nStepsIn25cm-1]       = hit->GetPID();
        StepTIDIn25cm[nStepsIn25cm-1]       = hit->GetTID();
        StepPDGCodeIn25cm[nStepsIn25cm-1]   = hit->GetParticle();
        StepnoIn25cm[nStepsIn25cm-1]        = hit->GetStepNo();
        StepPrePosIn25cmX[nStepsIn25cm-1]   = hit->GetPreStepPosition().x();
        StepPrePosIn25cmY[nStepsIn25cm-1]   = hit->GetPreStepPosition().y();
        StepPrePosIn25cmZ[nStepsIn25cm-1]   = hit->GetPreStepPosition().z();
        StepPostPosIn25cmX[nStepsIn25cm-1]  = hit->GetPostStepPosition().x();
        StepPostPosIn25cmY[nStepsIn25cm-1]  = hit->GetPostStepPosition().y();
        StepPostPosIn25cmZ[nStepsIn25cm-1]  = hit->GetPostStepPosition().z();
        StepEdepIn25cm[nStepsIn25cm-1]      = hit->GetEdep();
      }
    }
  }
}
