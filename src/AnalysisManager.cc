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
  evt->Branch("Px[nPrimaryParticle]"                     , Px                      , "Px[nPrimaryParticle]/D");
  evt->Branch("Py[nPrimaryParticle]"                     , Py                      , "Py[nPrimaryParticle]/D");
  evt->Branch("Pz[nPrimaryParticle]"                     , Pz                      , "Pz[nPrimaryParticle]/D");
  evt->Branch("Pmass[nPrimaryParticle]"                  , Pmass                   , "Pmass[nPrimaryParticle]/D");
  evt->Branch("primaryTrackID[nPrimaryParticle]"         , primaryTrackID          , "primaryTrackID[nPrimaryParticle]/I");
  evt->Branch("primaryTrackPDG[nPrimaryParticle]"        , primaryTrackPDG         , "primaryTrackPDG[nPrimaryParticle]/I");
  evt->Branch("primaryTrackLength[nPrimaryParticle]"     , primaryTrackLength      , "primaryTrackLength[nPrimaryParticle]/D");
  evt->Branch("primaryTrackLengthInTPC[nPrimaryParticle]", primaryTrackLengthInTPC , "primaryTrackLengthInTPC[nPrimaryParticle]/D");

  evt->Branch("edepInLAr"              , &edepInLAr             , "edepInLAr/D");
  evt->Branch("edepInLArXY2000mm"      , &edepInLArXY2000mm     , "edepInLArXY2000mm/D");
  evt->Branch("edepInLArXY2500mm"      , &edepInLArXY2500mm     , "edepInLArXY2500mm/D");
  evt->Branch("edepInHadCalX"          , &edepInHadCalX         , "edepInHadCalX/D");
  evt->Branch("edepInHadCalY"          , &edepInHadCalY         , "edepInHadCalY/D");
  evt->Branch("edepInMuonFinderX"      , &edepInMuonFinderX     , "edepInMuonFinderX/D");
  evt->Branch("edepInMuonFinderY"      , &edepInMuonFinderY     , "edepInMuonFinderY/D");
  evt->Branch("edepInHadAborb"         , &edepInHadAborb        , "edepInHadAborb/D");
  evt->Branch("edepInMuonFinderAbsorb" , &edepInMuonFinderAbsorb, "edepInMuonFinderAbsorb/D");
  evt->Branch("edepInCryGap"           , &edepInCryGap          , "edepInCryGap/D");

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
  nuPDG                  = 0;
  nuE                    = -999;
  nuX                    = -999;
  nuY                    = -999;
  nuZ                    = -999;
  nuIntType              = -1;
  nuScatteringType       = -1;
  nuFSLPDG               = 0;
  nuFSLPx                = -999;
  nuFSLPy                = -999;
  nuFSLPz                = -999;
  nuFSLE                 = -999;
  edepInLAr              = 0;
  edepInLArXY2000mm      = 0;
  edepInLArXY2500mm      = 0;
  edepInHadCalX          = 0;
  edepInHadCalY          = 0;
  edepInMuonFinderX      = 0;
  edepInMuonFinderY      = 0;
  edepInHadAborb         = 0;
  edepInMuonFinderAbsorb = 0;
  edepInCryGap           = 0;
  nPrimaryParticle       = 0;
  nFromFSLParticles      = 0;
  nSecondaryTracks       = 0;
  nStepsIn25cm           = 0;
  for (G4int i= 0; i< 2000000; ++i) {
    PDG[i]   = 0;
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
  allTracksPTPair.clear();
  tracksFromFSL.clear();
  tracksFromFSLSecondary.clear();
}

void AnalysisManager::EndOfEvent(const G4Event* event) {
  /// Branch: evtID
  evtID = event->GetEventID();
  if (m_saveEvd) {
    std::string histname = "evt_"+std::to_string(evtID)+"_EdepXY";
    hEdepXY = new TH2D(histname.c_str(), histname.c_str(), 600, -1500, 1500, 600, -1500, 1500);
    hEdepXY->GetXaxis()->SetTitle("X [mm]");
    hEdepXY->GetYaxis()->SetTitle("Y [mm]");
    histname = "evt_"+std::to_string(evtID)+"_EdepZX";
    hEdepZX = new TH2D(histname.c_str(), histname.c_str(), 1740, 0, 8700, 600, -1500, 1500);
    hEdepZX->GetXaxis()->SetTitle("Z [mm]");
    hEdepZX->GetYaxis()->SetTitle("X [mm]");
    histname = "evt_"+std::to_string(evtID)+"_EdepZY";
    hEdepZY = new TH2D(histname.c_str(), histname.c_str(), 1740, 0, 8700, 600, -1500, 1500);
    hEdepZY->GetXaxis()->SetTitle("Z [mm]");
    hEdepZY->GetYaxis()->SetTitle("Y [mm]");
    histname = "evt_"+std::to_string(evtID)+"_EdepXY_FSL";
    hEdepXYFSL = new TH2D(histname.c_str(), histname.c_str(), 600, -1500, 1500, 600, -1500, 1500);
    hEdepXYFSL->GetXaxis()->SetTitle("X [mm]");
    hEdepXYFSL->GetYaxis()->SetTitle("Y [mm]");
    histname = "evt_"+std::to_string(evtID)+"_EdepZX_FSL";
    hEdepZXFSL = new TH2D(histname.c_str(), histname.c_str(), 1740, -0, 8700, 600, -1500, 1500);
    hEdepZXFSL->GetXaxis()->SetTitle("Z [mm]");
    hEdepZXFSL->GetYaxis()->SetTitle("X [mm]");
    histname = "evt_"+std::to_string(evtID)+"_EdepZY_FSL";
    hEdepZYFSL = new TH2D(histname.c_str(), histname.c_str(), 1740, 0, 8700, 600, -1500, 1500);
    hEdepZYFSL->GetXaxis()->SetTitle("Z [mm]");
    hEdepZYFSL->GetYaxis()->SetTitle("Y [mm]");
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

  // loop over all sensitive detectors
  const G4int nsds = 8;
  G4int sdids[nsds] = {-1, -1, -1, -1, -1, -1, -1, -1};
  std::string sds[nsds] = {"lArBoxSD/lar_box",
                           "HadCalXSD/lar_box",
                           "HadCalYSD/lar_box",
                           "MuonFinderXSD/lar_box",
                           "MuonFinderYSD/lar_box",
                           "HadAbsorbSD/lar_box",
                           "MuonFinderAbsorbSD/lar_box",
                           "CryGapSD/lar_box"};
  for (int i= 0; i< nsds; ++i) {
    if (sdids[i]<0) {
      sdids[i] = sdm->GetCollectionID(sds[i]);
      G4cout<<"AnalysisManager : "<<sds[i]<<" (ID="<<sdids[i]<<")"<<G4endl;
    }
    if (sdids[i]>=0) {
      FillTree(sdids[i], sds[i]);
    }
  }

  // find all the tracks originate from the final state lepton, include FSL itself (TID=1)
  tracksFromFSL.insert(1);
  for (auto x : allTracksPTPair) {
    if (tracksFromFSL.find(x.first) != tracksFromFSL.end()) {
      tracksFromFSL.insert(x.second);
    }
  }
  std::cout<<"Recorded tracks       : "<<allTracksPTPair.size()<<std::endl;
  std::cout<<"Tracks from FSL       : "<<tracksFromFSL.size()<<std::endl;
  std::cout<<"Tracks from FSL (2nd) : "<<tracksFromFSLSecondary.size()<<std::endl;

  // must run after FillTree, otherwise tracksFromFSL and tracksFromFSLSecondary are invalid
  for (int i= 0; i< nsds; ++i) {
    if (sdids[i]>=0) {
      FillTrueEdep(sdids[i], sds[i]);
    }
  }

  evt->Fill();

  if (m_saveEvd) {
    thefile->cd("/edep/");
    hEdepXY->Write();
    hEdepZX->Write();
    hEdepZY->Write();
    hEdepXYFSL->Write();
    hEdepZXFSL->Write();
    hEdepZYFSL->Write();
  }
}

void AnalysisManager::FillTree(G4int sdId, std::string sdName) {
  int detID = 0;
  const std::unordered_map<std::string,std::function<void()>> detIDs{
          {"lArBoxSD/lar_box",           [&](){ detID = 1; }},
          {"HadCalXSD/lar_box",          [&](){ detID = 2; }},
          {"HadCalYSD/lar_box",          [&](){ detID = 3; }},
          {"MuonFinderXSD/lar_box",      [&](){ detID = 4; }},
          {"MuonFinderYSD/lar_box",      [&](){ detID = 5; }},
          {"HadAbsorbSD/lar_box",        [&](){ detID = 6; }},
          {"MuonFinderAbsorbSD/lar_box", [&](){ detID = 7; }},
          {"CryGapSD/lar_box",           [&](){ detID = 8; }},
  };
  detIDs.find(sdName)->second();

  // Get and cast hit collection with LArBoxHits
  LArBoxHitsCollection* hitCollection = dynamic_cast<LArBoxHitsCollection*>(hcofEvent->GetHC(sdId));
  if (hitCollection) {
    for (auto hit: *hitCollection->GetVector()) {
      double pre_x  = hit->GetPreStepPosition().x();
      double pre_y  = hit->GetPreStepPosition().y();
      double pre_z  = hit->GetPreStepPosition().z();
      double post_x = hit->GetPostStepPosition().x();
      double post_y = hit->GetPostStepPosition().y();
      double post_z = hit->GetPostStepPosition().z();

      // energy deposition in different volumes of the detector
      switch(detID) {
        case 1: {
          double thre1 = 1250; // mm
          double thre2 = 1000; // mm
          edepInLAr += hit->GetEdep();
          if ((abs(pre_x)<=thre1) & (abs(pre_y)<=thre1) & (abs(post_x)<=thre1) & (abs(post_y)<=thre1)) {
            edepInLArXY2500mm += hit->GetEdep();
          } else if ((abs(pre_x)<=thre1) & (abs(pre_y)<=thre1) & ((abs(post_x)>thre1) | (abs(post_y)>thre1))) {
            //double d = TMath::Sqrt(TMath::Power(pre_x-post_x,2)+TMath::Power(pre_y-post_y,2));
            if (abs(post_x)>abs(post_y)) {
              double w = (thre1 - abs(pre_x))/abs(post_x-pre_x);
              edepInLArXY2500mm += hit->GetEdep()*w;
            } else {
              double w = (thre1 - abs(pre_y))/abs(post_y-pre_y);
              edepInLArXY2500mm += hit->GetEdep()*w;
            }
          }
          if ((abs(pre_x)<=thre2) & (abs(pre_y)<=thre2) & (abs(post_x)<=thre2) & (abs(post_y)<=thre2)) {
            edepInLArXY2000mm += hit->GetEdep();
          } else if ((abs(pre_x)<=thre2) & (abs(pre_y)<=thre2) & ((abs(post_x)>thre2) | (abs(post_y)>thre2))) {
            //double d = TMath::Sqrt(TMath::Power(pre_x-post_x,2)+TMath::Power(pre_y-post_y,2));
            if (abs(post_x)>abs(post_y)) {
              double w = (thre2 - abs(pre_x))/abs(post_x-pre_x);
              edepInLArXY2000mm += hit->GetEdep()*w;
            } else {
              double w = (thre2 - abs(pre_y))/abs(post_y-pre_y);
              edepInLArXY2000mm += hit->GetEdep()*w;
            }
          }
          break; }
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
        case 6:
          edepInHadAborb += hit->GetEdep();
          break;
        case 7:
          edepInMuonFinderAbsorb += hit->GetEdep();
          break;
        case 8:
          edepInCryGap += hit->GetEdep();
          break;
      }
      // stable final state particles in GENIE, primary particles in Geant4
      if (hit->GetCreatorProcess()=="PrimaryParticle") { // i.e. PID==0
        if (hit->GetStepNo()==1) {
          primaryTrackID[hit->GetTID()-1] = hit->GetTID();
          primaryTrackPDG[hit->GetTID()-1] = hit->GetParticle();
        }
        primaryTrackLength[hit->GetTID()-1] += hit->GetStepLength();
        if (detID==1) {
          primaryTrackLengthInTPC[hit->GetTID()-1] += hit->GetStepLength();
        } 
      }

      if (hit->GetStepNo()==1 && hit->GetPID()>nPrimaryParticle && hit->GetParticle()!=22) {
        // record secondary particles, but exclude gamma
        nSecondaryTracks++;
        if (nSecondaryTracks<=2000000) 
          secondaryTrackPDG[nSecondaryTracks-1] = hit->GetParticle();
      }

      // save step information around the vertex to study vertex activities
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

      allTracksPTPair.insert(std::make_pair(hit->GetPID(), hit->GetTID()));
      if (hit->GetPID()==1 && hit->GetCreatorProcess()=="Decay") {
        tracksFromFSLSecondary.insert(hit->GetTID());
      }
    } // end of hit loop
  }
}

void AnalysisManager::FillTrueEdep(G4int sdId, std::string sdName) {
  int detID = 0;
  const std::unordered_map<std::string,std::function<void()>> detIDs{
          {"lArBoxSD/lar_box",           [&](){ detID = 1; }},
          {"HadCalXSD/lar_box",          [&](){ detID = 2; }},
          {"HadCalYSD/lar_box",          [&](){ detID = 3; }},
          {"MuonFinderXSD/lar_box",      [&](){ detID = 4; }},
          {"MuonFinderYSD/lar_box",      [&](){ detID = 5; }},
          {"HadAbsorbSD/lar_box",        [&](){ detID = 6; }},
          {"MuonFinderAbsorbSD/lar_box", [&](){ detID = 7; }},
          {"CryGapSD/lar_box",           [&](){ detID = 8; }},
  };
  detIDs.find(sdName)->second();

  nFromFSLParticles = tracksFromFSLSecondary.size();
  std::map<int, int> map_tracksFromFSLSecondary;
  int _idx = 0;
  for (auto _tid : tracksFromFSLSecondary) {
    map_tracksFromFSLSecondary[_tid] = _idx;
    _idx++;
  }

  LArBoxHitsCollection* hitCollection = dynamic_cast<LArBoxHitsCollection*>(hcofEvent->GetHC(sdId));
  if (hitCollection) {
    for (auto hit: *hitCollection->GetVector()) {
      // Particles decay from the final state lepton in GENIE, or decay from the primary particles in G4
      if (tracksFromFSLSecondary.find(hit->GetTID()) != tracksFromFSLSecondary.end()) {
        int whichTrackFromFSL = map_tracksFromFSLSecondary[hit->GetTID()];
        if (hit->GetStepNo()==1) {
          std::cout<<"TID : "<<hit->GetTID()     <<", PID : "           <<hit->GetPID()
            <<", PDG : "<<hit->GetParticle()     <<", CreatorProcess : "<<hit->GetCreatorProcess()
            <<", Ek : " <<hit->GetInitKinEnergy()<<" MeV"              <<std::endl;
          fromFSLParticlePDG[whichTrackFromFSL]  = hit->GetParticle();
          fromFSLParticleKinE[whichTrackFromFSL] = hit->GetInitKinEnergy();
          fromFSLParticlePx[whichTrackFromFSL]   = hit->GetInitMomentum().getX();
          fromFSLParticlePy[whichTrackFromFSL]   = hit->GetInitMomentum().getY();
          fromFSLParticlePz[whichTrackFromFSL]   = hit->GetInitMomentum().getZ();
        }
        fromFSLTrackLength[whichTrackFromFSL] += hit->GetStepLength();
        if (detID==1) {
          fromFSLTrackLengthInTPC[whichTrackFromFSL] += hit->GetStepLength();
        }
      }

      if (m_saveEvd) {
        hEdepXY->Fill(hit->GetEdepPosition().x(), hit->GetEdepPosition().y(), hit->GetEdep());
        hEdepZX->Fill(hit->GetEdepPosition().z()+3500, hit->GetEdepPosition().x(), hit->GetEdep());
        hEdepZY->Fill(hit->GetEdepPosition().z()+3500, hit->GetEdepPosition().y(), hit->GetEdep());
        if (tracksFromFSL.find(hit->GetTID()) != tracksFromFSL.end()) {
          hEdepXYFSL->Fill(hit->GetEdepPosition().x(), hit->GetEdepPosition().y(), hit->GetEdep());
          hEdepZXFSL->Fill(hit->GetEdepPosition().z()+3500, hit->GetEdepPosition().x(), hit->GetEdep());
          hEdepZYFSL->Fill(hit->GetEdepPosition().z()+3500, hit->GetEdepPosition().y(), hit->GetEdep());
        }
      }
    } // end of hit loop
  }
}
