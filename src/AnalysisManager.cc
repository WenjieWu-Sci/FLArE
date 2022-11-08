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
  m_saveHit = false;
  m_saveEvd = false;
}

AnalysisManager::~AnalysisManager() {;}

void AnalysisManager::bookEvtTree() {
  evt = new TTree("evt", "evtTreeInfo");
  evt->Branch("evtID"                  , &evtID                  , "evtID/I");
  evt->Branch("nuIdx"                  , &nuIdx                  , "nuIdx/I");
  evt->Branch("nuPDG"                  , &nuPDG                  , "nuPDG/I");
  evt->Branch("nuE"                    , &nuE                    , "nuE/D");
  evt->Branch("nuX"                    , &nuX                    , "nuX/D");
  evt->Branch("nuY"                    , &nuY                    , "nuY/D");
  evt->Branch("nuZ"                    , &nuZ                    , "nuZ/D");
  evt->Branch("nuIntType"              , &nuIntType              , "nuIntType/I");
  evt->Branch("nuScatteringType"       , &nuScatteringType       , "nuScatteringType/I");
  evt->Branch("nuFSLPDG"               , &nuFSLPDG               , "nuFSLPDG/I");
  evt->Branch("nuFSLPx"                , &nuFSLPx                , "nuFSLPx/D");
  evt->Branch("nuFSLPy"                , &nuFSLPy                , "nuFSLPy/D");
  evt->Branch("nuFSLPz"                , &nuFSLPz                , "nuFSLPz/D");
  evt->Branch("nuFSLE"                 , &nuFSLE                 , "nuFSLE/D");
  evt->Branch("nPrimaryParticle"       , &nPrimaryParticle       , "nPrimaryParticle/I");
  evt->Branch("Px"                     , Px                      , "Px[nPrimaryParticle]/D");
  evt->Branch("Py"                     , Py                      , "Py[nPrimaryParticle]/D");
  evt->Branch("Pz"                     , Pz                      , "Pz[nPrimaryParticle]/D");
  evt->Branch("VtxX"                   , VtxX                    , "VtxX[nPrimaryParticle]/D");
  evt->Branch("VtxY"                   , VtxY                    , "VtxY[nPrimaryParticle]/D");
  evt->Branch("VtxZ"                   , VtxZ                    , "VtxZ[nPrimaryParticle]/D");
  evt->Branch("Pmass"                  , Pmass                   , "Pmass[nPrimaryParticle]/D");
  evt->Branch("primaryParentID"        , primaryParentID         , "primaryParentID[nPrimaryParticle]/I");
  evt->Branch("primaryTrackID"         , primaryTrackID          , "primaryTrackID[nPrimaryParticle]/I");
  evt->Branch("primaryTrackPDG"        , primaryTrackPDG         , "primaryTrackPDG[nPrimaryParticle]/I");
  evt->Branch("primaryTrackLength"     , primaryTrackLength      , "primaryTrackLength[nPrimaryParticle]/D");
  evt->Branch("primaryTrackLengthInTPC", primaryTrackLengthInTPC , "primaryTrackLengthInTPC[nPrimaryParticle]/D");
  evt->Branch("prongType"              , prongType               , "prongType[nPrimaryParticle]/I");
  evt->Branch("EInLAr"                 , EInLAr                  , "EInLAr[nPrimaryParticle]/D");
  evt->Branch("EInHadCal"              , EInHadCal               , "EInHadCal[nPrimaryParticle]/D");
  evt->Branch("EInMuonFinder"          , EInMuonFinder           , "EInMuonFinder[nPrimaryParticle]/D");
  evt->Branch("AngleToBeamDir"         , AngleToBeamDir          , "AngleToBeamDir[nPrimaryParticle]/D");
  evt->Branch("ShowerLength"           , ShowerLength            , "ShowerLength[nPrimaryParticle]/D");
  evt->Branch("ShowerLengthInLAr"      , ShowerLengthInLAr       , "ShowerLengthInLAr[nPrimaryParticle]/D");
  evt->Branch("ShowerWidth"            , ShowerWidth             , "ShowerWidth[nPrimaryParticle]/D");
  evt->Branch("ShowerWidthInLAr"       , ShowerWidthInLAr        , "ShowerWidthInLAr[nPrimaryParticle]/D");
  evt->Branch("dEdx"                   , dEdx                    , "dEdx[nPrimaryParticle]/D");
  evt->Branch("dEdxInLAr"              , dEdxInLAr               , "dEdxInLAr[nPrimaryParticle]/D");

  evt->Branch("nHits"                  , &nHits                 , "nHits/I");
  if (m_saveHit) {
    evt->Branch("HitTID"               , HitTID                 , "HitTID[nHits]/I");
    evt->Branch("HitPID"               , HitPID                 , "HitPID[nHits]/I");
    evt->Branch("HitPDG"               , HitPDG                 , "HitPDG[nHits]/I");
    evt->Branch("HitTrackStatus"       , HitTrackStatus         , "HitTrackStatus[nHits]/I");
    evt->Branch("HitPrePositionX"      , HitPrePositionX        , "HitPrePositionX[nHits]/D");
    evt->Branch("HitPrePositionY"      , HitPrePositionY        , "HitPrePositionY[nHits]/D");
    evt->Branch("HitPrePositionZ"      , HitPrePositionZ        , "HitPrePositionZ[nHits]/D");
    evt->Branch("HitPosPositionX"      , HitPosPositionX        , "HitPosPositionX[nHits]/D");
    evt->Branch("HitPosPositionY"      , HitPosPositionY        , "HitPosPositionY[nHits]/D");
    evt->Branch("HitPosPositionZ"      , HitPosPositionZ        , "HitPosPositionZ[nHits]/D");
    evt->Branch("HitEdep"              , HitEdep                , "HitEdep[nHits]/D");
  }

  evt->Branch("edepInLAr"              , &edepInLAr             , "edepInLAr/D");
  evt->Branch("edepInLArXY2500mm"      , &edepInLArXY2500mm     , "edepInLArXY2500mm/D");
  evt->Branch("edepInLArXY2000mm"      , &edepInLArXY2000mm     , "edepInLArXY2000mm/D");
  evt->Branch("edepInLArXY1500mm"      , &edepInLArXY1500mm     , "edepInLArXY1500mm/D");
  evt->Branch("edepInLArXY1000mm"      , &edepInLArXY1000mm     , "edepInLArXY1000mm/D");
  evt->Branch("edepInHadCalX"          , &edepInHadCalX         , "edepInHadCalX/D");
  evt->Branch("edepInHadCalY"          , &edepInHadCalY         , "edepInHadCalY/D");
  evt->Branch("edepInMuonFinderX"      , &edepInMuonFinderX     , "edepInMuonFinderX/D");
  evt->Branch("edepInMuonFinderY"      , &edepInMuonFinderY     , "edepInMuonFinderY/D");
  evt->Branch("edepInHadAborb"         , &edepInHadAborb        , "edepInHadAborb/D");
  evt->Branch("edepInMuonFinderAbsorb" , &edepInMuonFinderAbsorb, "edepInMuonFinderAbsorb/D");
  evt->Branch("edepInCryGap"           , &edepInCryGap          , "edepInCryGap/D");
  evt->Branch("missCountedEnergy"      , &missCountedEnergy     , "missCountedEnergy/D");

  evt->Branch("nFromFSLParticles"      , &nFromFSLParticles      , "nFromFSLParticles/I");
}

void AnalysisManager::BeginOfRun() {
  G4cout<<"TTree is booked and the run has been started"<<G4endl;
  if (thefile) {
    delete thefile;
  }
  thefile = new TFile(m_filename, "RECREATE");
  bookEvtTree();
}

void AnalysisManager::EndOfRun() {
  thefile->cd();
  evt->Write();
  thefile->Close();
}

void AnalysisManager::BeginOfEvent() {
  nuIdx                  = -1;
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
  nHits                  = 0;
  edepInLAr              = 0;
  edepInLArXY2500mm      = 0;
  edepInLArXY2000mm      = 0;
  edepInLArXY1500mm      = 0;
  edepInLArXY1000mm      = 0;
  edepInHadCalX          = 0;
  edepInHadCalY          = 0;
  edepInMuonFinderX      = 0;
  edepInMuonFinderY      = 0;
  edepInHadAborb         = 0;
  edepInMuonFinderAbsorb = 0;
  edepInCryGap           = 0;
  missCountedEnergy      = 0;
  nPrimaryParticle       = 0;
  nFromFSLParticles      = 0;
  countPrimaryParticle   = 0;
  //nSecondaryTracks       = 0;
  //nStepsIn25cm           = 0;
  for (G4int i= 0; i< 2000000; ++i) {
    Px[i]    = -999;
    Py[i]    = -999;
    Pz[i]    = -999;
    VtxX[i]  = -9999;
    VtxY[i]  = -9999;
    VtxZ[i]  = -9999;
    Pmass[i] = -999;
    primaryParentID[i]         = -1;
    primaryTrackID[i]          = -1;
    primaryTrackPDG[i]         = 0;
    prongType[i]               = -1;
    primaryTrackLength[i]      = 0;
    primaryTrackLengthInTPC[i] = 0;
    EInLAr[i]                  = 0;
    EInHadCal[i]               = 0;
    EInMuonFinder[i]           = 0;
    AngleToBeamDir[i]          = -1;
    ShowerLength[i]            = -1;
    ShowerLengthInLAr[i]       = -1;
    ShowerWidth[i]             = -1;
    ShowerWidthInLAr[i]        = -1;
    dEdx[i]                    = -1;
    dEdxInLAr[i]               = -1;
    fromFSLParticlePDG[i]      = 0;
  }
  if (m_saveHit) {
    for (G4int i= 0; i< 40000000; ++i) {
      HitTID[i] = -1;
      HitPID[i] = -1;
      HitPDG[i] = 0;
      HitTrackStatus[i]  = -1;
      HitPrePositionX[i] = -999;
      HitPrePositionY[i] = -999;
      HitPrePositionZ[i] = -999;
      HitPosPositionX[i] = -999;
      HitPosPositionY[i] = -999;
      HitPosPositionZ[i] = -999;
      HitEdep[i]         = 0;
    }
  }

  // vectors that need to be cleared for a new event
  allTracksPTPair.clear();
  trackClusters.clear();
  tracksFromFSL.clear();
  tracksFromFSLSecondary.clear();
  ShowerP.clear();
  hitClusterXY.clear();
  hitClusterZX.clear();
  hitClusterZY.clear();
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
          nuIdx            = primary_particle_info->GetNeuIdx();
          nuPDG            = primary_particle_info->GetNeuPDG();
          nuE              = primary_particle_info->GetNeuP4().T();
          nuX              = primary_particle_info->GetNeuX4().X();
          nuY              = primary_particle_info->GetNeuX4().Y();
          nuZ              = primary_particle_info->GetNeuX4().Z();
          nuIntType        = primary_particle_info->GetInteractionTypeId();
          nuScatteringType = primary_particle_info->GetScatteringTypeId();
          nuFSLPDG         = primary_particle_info->GetFSLPDG();
          nuFSLPx          = primary_particle_info->GetFSLP4().X();
          nuFSLPy          = primary_particle_info->GetFSLP4().Y();
          nuFSLPz          = primary_particle_info->GetFSLP4().Z();
          nuFSLE           = primary_particle_info->GetFSLP4().T();
        }
        count_particles++;
      }
    }
  }
  nPrimaryVertex   = event->GetNumberOfPrimaryVertex();
  nPrimaryParticle = count_particles;
  std::cout<<"\nnumber of primary vertices  : "<<nPrimaryVertex<<std::endl;

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
    }
    if (sdids[i]>=0) {
      FillTree(sdids[i], sds[i]);
    }
  }

  nFromFSLParticles = tracksFromFSLSecondary.size();
  if (nFromFSLParticles>0) {
    // if fsl decays, count decay products as primary particles
    nPrimaryParticle = nPrimaryParticle + nFromFSLParticles;
  }

  /// event displays
  /// 0: deposited energy of all hits
  /// non-0: deposited energy of each prong (primary particle)
  if (m_saveEvd) {
    int res = 5;
    int len_x = 1800;
    int len_y = 1800;
    //int len_z = 8700; // 16cm steel for muon finder
    int len_z = 9400;   // 50cm steel for muon finder
    hitClusterXY.resize(nPrimaryParticle+1);
    hitClusterZX.resize(nPrimaryParticle+1);
    hitClusterZY.resize(nPrimaryParticle+1);
    std::string histname = "evt_"+std::to_string(evtID)+"_tot_EdepXY";
    hitClusterXY[0] = new TH2D(histname.c_str(), histname.c_str(), len_x/res, -len_x/2, len_x/2, len_y/res, -len_y/2, len_y/2);
    hitClusterXY[0]->GetXaxis()->SetTitle("X [mm]");
    hitClusterXY[0]->GetYaxis()->SetTitle("Y [mm]");
    histname = "evt_"+std::to_string(evtID)+"_tot_EdepZX";
    hitClusterZX[0] = new TH2D(histname.c_str(), histname.c_str(), len_z/res, 0, len_z, len_x/res, -len_x/2, len_x/2);
    hitClusterZX[0]->GetXaxis()->SetTitle("Z [mm]");
    hitClusterZX[0]->GetYaxis()->SetTitle("X [mm]");
    histname = "evt_"+std::to_string(evtID)+"_tot_EdepZY";
    hitClusterZY[0] = new TH2D(histname.c_str(), histname.c_str(), len_z/res, 0, len_z, len_y/res, -len_y/2, len_y/2);
    hitClusterZY[0]->GetXaxis()->SetTitle("Z [mm]");
    hitClusterZY[0]->GetYaxis()->SetTitle("Y [mm]");
    for (int iPrim=0; iPrim< nPrimaryParticle; ++iPrim) {
      histname = "evt_"+std::to_string(evtID)+"_Prong_"+std::to_string(iPrim)+"_EdepXY";
      hitClusterXY[iPrim+1] = new TH2D(histname.c_str(), histname.c_str(), len_x/res, -len_x/2, len_x/2, len_y/res, -len_y/2, len_y/2);
      hitClusterXY[iPrim+1]->GetXaxis()->SetTitle("X [mm]");
      hitClusterXY[iPrim+1]->GetYaxis()->SetTitle("Y [mm]");
      histname = "evt_"+std::to_string(evtID)+"_Prong_"+std::to_string(iPrim)+"_EdepZX";
      hitClusterZX[iPrim+1] = new TH2D(histname.c_str(), histname.c_str(), len_z/res, 0, len_z, len_x/res, -len_x/2, len_x/2);
      hitClusterZX[iPrim+1]->GetXaxis()->SetTitle("Z [mm]");
      hitClusterZX[iPrim+1]->GetYaxis()->SetTitle("X [mm]");
      histname = "evt_"+std::to_string(evtID)+"_Prong_"+std::to_string(iPrim)+"_EdepZY";
      hitClusterZY[iPrim+1] = new TH2D(histname.c_str(), histname.c_str(), len_z/res, 0, len_z, len_y/res, -len_y/2, len_y/2);
      hitClusterZY[iPrim+1]->GetXaxis()->SetTitle("Z [mm]");
      hitClusterZY[iPrim+1]->GetYaxis()->SetTitle("Y [mm]");
    }
  }

  // find all the tracks originate from the final state lepton, include FSL itself (TID=1)
  tracksFromFSL.insert(1);
  for (auto x : allTracksPTPair) {
    if (tracksFromFSL.find(x.first) != tracksFromFSL.end()) {
      tracksFromFSL.insert(x.second);
    }
  }
  // tracksFromFSL includes all the tracks orginating from the fsl
  // tracksFromFSLSeconday only inclues the tracks directly decayed from the fsl
  std::cout<<"Recorded tracks       : "<<allTracksPTPair.size()<<std::endl;
  std::cout<<"Tracks from FSL       : "<<tracksFromFSL.size()<<std::endl;
  std::cout<<"Tracks from FSL (2nd) : "<<tracksFromFSLSecondary.size()<<std::endl;
  std::cout<<"number of primary particles : "<<nPrimaryParticle
    <<" , in which number of particles from fsl : "<<nFromFSLParticles<<std::endl;

  // cluster all tracks to primary particles
  trackClusters.resize(nPrimaryParticle);
  for (int iPrim= 0; iPrim< nPrimaryParticle; ++iPrim) {
    trackClusters[iPrim].insert(primaryTrackID[iPrim]);
  }
  for (auto x : allTracksPTPair) {
    // if this track is the fsl (TID=1) and it decays (nFromFSLParticles>0),
    // then it forms a single cluster by itself, this is mainly for studying the tau decay.
    if ((x.second==1) && (nFromFSLParticles>0)) continue;
    // if this track is the decay product of the fsl, it should already been added to the trackClusters
    if ((x.first==1) && (nFromFSLParticles>0)) continue;
    // add the track to the corresponding cluster if its parent is in the cluster.
    // one track can have only one parent, break the loop once its parent is found.
    for (int iPrim= 0; iPrim< nPrimaryParticle; ++iPrim) {
      if (trackClusters[iPrim].find(x.first) != trackClusters[iPrim].end()) {
        trackClusters[iPrim].insert(x.second);
        break;
      }
    }
  }

  ShowerP.resize(nPrimaryParticle);
  for (int iPrim= 0; iPrim< nPrimaryParticle; ++iPrim) {
    ShowerP[iPrim] = TMath::Sqrt(Px[iPrim]*Px[iPrim]+Py[iPrim]*Py[iPrim]+Pz[iPrim]*Pz[iPrim]);
    double costheta = Pz[iPrim]/ShowerP[iPrim];
    AngleToBeamDir[iPrim] = TMath::ACos(costheta);
  }

  // FillTrueEdep must run after FillTree, otherwise tracksFromFSL and tracksFromFSLSecondary are invalid
  for (int i= 0; i< nsds; ++i) {
    if (sdids[i]>=0) {
      FillTrueEdep(sdids[i], sds[i]);
    }
  }

  std::cout<<"\nPDG Angle ShowerLength EInLAr EInHadCal EInMuonFinder dEdx ShowerLengthInLAr dEdxInLAr ProngType Pz"<<std::endl;
  for (int iPrim= 0; iPrim< nPrimaryParticle; ++iPrim) { 
    if (abs(nuPDG)==16 && abs(nuFSLPDG)==16) {
      prongType[iPrim] = 0;
    } else if (abs(nuPDG)==16 && abs(nuFSLPDG)==15) {
      bool tau2e = false;
      bool tau2mu = false;
      for (int iFromFSL= 0;iFromFSL< nFromFSLParticles; ++iFromFSL) {
        if (fromFSLParticlePDG[iFromFSL]==13) {
          tau2mu = true;
        } else if (fromFSLParticlePDG[iFromFSL]==11) {
          tau2e = true;
        }
      }
      if (tau2e) {
        if (primaryParentID[iPrim]>0) {
          prongType[iPrim] = 1;
        } else {
          prongType[iPrim] = 2;
        }
      } else if (tau2mu) {
        if (primaryParentID[iPrim]>0) {
          prongType[iPrim] = 3;
        } else {
          prongType[iPrim] = 4;
        } 
      } else {
        if (primaryParentID[iPrim]>0) {
          prongType[iPrim] = 5;
        } else {
          prongType[iPrim] = 6;
        } 
      }
    }
    if (abs(nuPDG)==14 && abs(nuFSLPDG)==14) {
      prongType[iPrim] = 7;
    } else if (abs(nuPDG)==14 && abs(nuFSLPDG)==13) {
      if (abs(primaryTrackPDG[iPrim])==13) {
        prongType[iPrim] = 8;
      } else {
        prongType[iPrim] = 9;
      }
    }
    if (abs(nuPDG)==12 && abs(nuFSLPDG)==12) {
      prongType[iPrim] = 10;
    } else if (abs(nuPDG)==12 && abs(nuFSLPDG)==11) {
      if (abs(primaryTrackPDG[iPrim])==11) {
        prongType[iPrim] = 11;
      } else {
        prongType[iPrim] = 12;
      }
    }
    dEdx[iPrim] = (EInLAr[iPrim] + EInHadCal[iPrim] + EInMuonFinder[iPrim])/ShowerLength[iPrim];
    dEdxInLAr[iPrim] = EInLAr[iPrim]/ShowerLengthInLAr[iPrim];

    std::cout<<std::setw(8)<<primaryTrackPDG[iPrim]<<" "
             <<std::setw(8)<<AngleToBeamDir[iPrim]<<" "
             <<std::setw(8)<<ShowerLength[iPrim]<<" "
             <<std::setw(8)<<EInLAr[iPrim]<<" " 
             <<std::setw(8)<<EInHadCal[iPrim]<<" "
             <<std::setw(8)<<EInMuonFinder[iPrim]<<" "
             <<std::setw(8)<<dEdx[iPrim]<<" "
             <<std::setw(8)<<ShowerLengthInLAr[iPrim]<<" "
             <<std::setw(8)<<dEdxInLAr[iPrim]<<" "
             <<std::setw(8)<<prongType[iPrim]<<" "
             <<std::setw(8)<<Pz[iPrim]<<std::endl;
  }

  evt->Fill();

  std::cout<<"Total number of hits : "<<nHits<<std::endl;

  if (m_saveEvd) {
    std::string dirname = "edep/evt_"+std::to_string(evtID)+"/";
    thefile->mkdir(dirname.c_str());
    thefile->cd(dirname.c_str());
    for (int i=0; i<nPrimaryParticle+1; ++i) {
      hitClusterXY[i]->Write();
      hitClusterZX[i]->Write();
      hitClusterZY[i]->Write();
    }
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
      nHits++;

      double pre_x  = hit->GetPreStepPosition().x();
      double pre_y  = hit->GetPreStepPosition().y();
      double pre_z  = hit->GetPreStepPosition().z();
      double post_x = hit->GetPostStepPosition().x();
      double post_y = hit->GetPostStepPosition().y();
      double post_z = hit->GetPostStepPosition().z();
      
      if (m_saveHit) {
        if (nHits<=40000000) {
          HitTID[nHits-1] = hit->GetTID();
          HitPID[nHits-1] = hit->GetPID();
          HitPDG[nHits-1] = hit->GetParticle();
          HitTrackStatus[nHits-1]  = hit->GetTrackStatus();
          HitPrePositionX[nHits-1] = pre_x;
          HitPrePositionY[nHits-1] = pre_y;
          HitPrePositionZ[nHits-1] = pre_z;
          HitPosPositionX[nHits-1] = post_x;
          HitPosPositionY[nHits-1] = post_y;
          HitPosPositionZ[nHits-1] = post_z;
          HitEdep[nHits-1] = hit->GetEdep();
        }
      }

      // energy deposition in different volumes of the detector
      switch(detID) {
        case 1: {
          double thre1 = 1250; // mm
          double thre2 = 1000; // mm
          double thre3 = 750; // mm
          double thre4 = 500; // mm
          edepInLAr += hit->GetEdep();
          if ((abs(pre_x)<=thre1) & (abs(pre_y)<=thre1) & (abs(post_x)<=thre1) & (abs(post_y)<=thre1)) {
            edepInLArXY2500mm += hit->GetEdep();
          } else if ((abs(pre_x)<=thre1) & (abs(pre_y)<=thre1) & ((abs(post_x)>thre1) | (abs(post_y)>thre1))) {
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
            if (abs(post_x)>abs(post_y)) {
              double w = (thre2 - abs(pre_x))/abs(post_x-pre_x);
              edepInLArXY2000mm += hit->GetEdep()*w;
            } else {
              double w = (thre2 - abs(pre_y))/abs(post_y-pre_y);
              edepInLArXY2000mm += hit->GetEdep()*w;
            }
          }
          if ((abs(pre_x)<=thre3) & (abs(pre_y)<=thre3) & (abs(post_x)<=thre3) & (abs(post_y)<=thre3)) {
            edepInLArXY1500mm += hit->GetEdep();
          } else if ((abs(pre_x)<=thre3) & (abs(pre_y)<=thre3) & ((abs(post_x)>thre3) | (abs(post_y)>thre3))) {
            if (abs(post_x)>abs(post_y)) {
              double w = (thre3 - abs(pre_x))/abs(post_x-pre_x);
              edepInLArXY1500mm += hit->GetEdep()*w;
            } else {
              double w = (thre3 - abs(pre_y))/abs(post_y-pre_y);
              edepInLArXY1500mm += hit->GetEdep()*w;
            }
          }
          if ((abs(pre_x)<=thre4) & (abs(pre_y)<=thre4) & (abs(post_x)<=thre4) & (abs(post_y)<=thre4)) {
            edepInLArXY1000mm += hit->GetEdep();
          } else if ((abs(pre_x)<=thre4) & (abs(pre_y)<=thre4) & ((abs(post_x)>thre4) | (abs(post_y)>thre4))) {
            if (abs(post_x)>abs(post_y)) {
              double w = (thre4 - abs(pre_x))/abs(post_x-pre_x);
              edepInLArXY1000mm += hit->GetEdep()*w;
            } else {
              double w = (thre4 - abs(pre_y))/abs(post_y-pre_y);
              edepInLArXY1000mm += hit->GetEdep()*w;
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

      allTracksPTPair.insert(std::make_pair(hit->GetPID(), hit->GetTID()));

      // stable final state particles in GENIE, primary particles in Geant4
      if (hit->GetCreatorProcess()=="PrimaryParticle") { // i.e. PID==0
        if (hit->GetStepNo()==1) {
          // the following line excludes final state lepton tau from the primary particle list
          //if (abs(nuPDG)==16 && abs(nuFSLPDG)==15 && abs(hit->GetParticle()==15)) continue;
          countPrimaryParticle++;
          primaryParentID[countPrimaryParticle-1] = hit->GetPID();
          primaryTrackID[countPrimaryParticle-1]  = hit->GetTID();
          primaryTrackPDG[countPrimaryParticle-1] = hit->GetParticle();
          Px[countPrimaryParticle-1]              = hit->GetInitMomentum().x();
          Py[countPrimaryParticle-1]              = hit->GetInitMomentum().y();
          Pz[countPrimaryParticle-1]              = hit->GetInitMomentum().z();
          VtxX[countPrimaryParticle-1]            = hit->GetTrackVertex().x();
          VtxY[countPrimaryParticle-1]            = hit->GetTrackVertex().y();
          VtxZ[countPrimaryParticle-1]            = hit->GetTrackVertex().z();
          Pmass[countPrimaryParticle-1]           = hit->GetParticleMass();
        }
      }
      // in case of the fsl decay, the decay products are counted as primary particle
      if (hit->GetPID()==1 && hit->GetCreatorProcess()=="Decay") {
        tracksFromFSLSecondary.insert(hit->GetTID());
        if (hit->GetStepNo()==1) {
          countPrimaryParticle++;
          primaryParentID[countPrimaryParticle-1] = hit->GetPID();
          primaryTrackID[countPrimaryParticle-1]  = hit->GetTID();
          primaryTrackPDG[countPrimaryParticle-1] = hit->GetParticle();
          Px[countPrimaryParticle-1]              = hit->GetInitMomentum().x();
          Py[countPrimaryParticle-1]              = hit->GetInitMomentum().y();
          Pz[countPrimaryParticle-1]              = hit->GetInitMomentum().z();
          VtxX[countPrimaryParticle-1]            = hit->GetTrackVertex().x();
          VtxY[countPrimaryParticle-1]            = hit->GetTrackVertex().y();
          VtxZ[countPrimaryParticle-1]            = hit->GetTrackVertex().z();
          Pmass[countPrimaryParticle-1]           = hit->GetParticleMass();
        }
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
        }
      }


      int whichPrim = -1;
      for (int iPrim= 0; iPrim< nPrimaryParticle; ++iPrim) {
        if (trackClusters[iPrim].find(hit->GetTID()) != trackClusters[iPrim].end()) {
          whichPrim = iPrim;
          break;
        }
      }
      if (whichPrim< 0) { 
        std::cout<<"Can't find the primary particle of the hit, something is wrong "<<hit->GetParticle()<<" "<<hit->GetEdep()<<std::endl; 
        missCountedEnergy += hit->GetEdep();
        continue;
      } 

      if ((hit->GetPID()==0) |
          (tracksFromFSLSecondary.find(hit->GetTID()) != tracksFromFSLSecondary.end())) {
        primaryTrackLength[whichPrim] += hit->GetStepLength();
        if (detID==1) {
          primaryTrackLengthInTPC[whichPrim] += hit->GetStepLength();
        }
      }

      if (m_saveEvd) {
        double pos_x = hit->GetEdepPosition().x();
        double pos_y = hit->GetEdepPosition().y();
        double pos_z = hit->GetEdepPosition().z() + 3500;
        hitClusterXY[0]->Fill(pos_x, pos_y, hit->GetEdep());
        hitClusterZX[0]->Fill(pos_z, pos_x, hit->GetEdep());
        hitClusterZY[0]->Fill(pos_z, pos_y, hit->GetEdep());
        hitClusterXY[whichPrim+1]->Fill(pos_x, pos_y, hit->GetEdep());
        hitClusterZX[whichPrim+1]->Fill(pos_z, pos_x, hit->GetEdep());
        hitClusterZY[whichPrim+1]->Fill(pos_z, pos_y, hit->GetEdep());
      }

//      if (hit->GetEdep()>1e-6) {
        double pre_x  = hit->GetPreStepPosition().x();
        double pre_y  = hit->GetPreStepPosition().y();
        double pre_z  = hit->GetPreStepPosition().z();
        double post_x = hit->GetPostStepPosition().x();
        double post_y = hit->GetPostStepPosition().y();
        double post_z = hit->GetPostStepPosition().z();
        double mod_Pre = TMath::Sqrt((pre_x-VtxX[whichPrim])*(pre_x-VtxX[whichPrim])+
                                     (pre_y-VtxY[whichPrim])*(pre_y-VtxY[whichPrim])+
                                     (pre_z-VtxZ[whichPrim])*(pre_z-VtxZ[whichPrim]));
        double mod_Pos = TMath::Sqrt((post_x-VtxX[whichPrim])*(post_x-VtxX[whichPrim])+
                                     (post_y-VtxY[whichPrim])*(post_y-VtxY[whichPrim])+
                                     (post_z-VtxZ[whichPrim])*(post_z-VtxZ[whichPrim]));
        double product_Pre = (pre_x-VtxX[whichPrim])*Px[whichPrim]+
                             (pre_y-VtxY[whichPrim])*Py[whichPrim]+
                             (pre_z-VtxZ[whichPrim])*Pz[whichPrim];
        double product_Pos = (post_x-VtxX[whichPrim])*Px[whichPrim]+
                             (post_y-VtxY[whichPrim])*Py[whichPrim]+
                             (post_z-VtxZ[whichPrim])*Pz[whichPrim];
        double len_Pre = TMath::Abs(product_Pre)/ShowerP[whichPrim];
        double len_Pos = TMath::Abs(product_Pos)/ShowerP[whichPrim]; 
        double width_Pre = TMath::Sqrt(TMath::Power(ShowerP[whichPrim]*mod_Pre,2)-TMath::Power(product_Pre,2))/ShowerP[whichPrim];
        double width_Pos = TMath::Sqrt(TMath::Power(ShowerP[whichPrim]*mod_Pos,2)-TMath::Power(product_Pos,2))/ShowerP[whichPrim];
        // exclude non-zero hit and hits from neutron when calculating showerlength of the primary particle
        // hits deposited little energy are difficult to be detected
        // simply ignoring the hits is not the most correct but easiest way
        if (hit->GetEdep()>0 && hit->GetParticle()!=2112) {
          ShowerLength[whichPrim] = std::max({ShowerLength[whichPrim], len_Pre, len_Pos});
          ShowerWidth[whichPrim] = std::max({ShowerWidth[whichPrim], width_Pre, width_Pos});
        }
        if (detID==1) { 
          EInLAr[whichPrim] += hit->GetEdep();
          if (hit->GetEdep()>0.1 && hit->GetParticle()!=2112) {
            ShowerLengthInLAr[whichPrim] = std::max({ShowerLengthInLAr[whichPrim], len_Pre, len_Pos});
            ShowerWidthInLAr[whichPrim] = std::max({ShowerWidthInLAr[whichPrim], width_Pre, width_Pos});
          }
        }
        if (detID==2 || detID==3 || detID==6) {
          EInHadCal[whichPrim] += hit->GetEdep();
        }
        if (detID==4 || detID==5 || detID==7) {
          EInMuonFinder[whichPrim] += hit->GetEdep();
        }
//      }
    } // end of hit loop
  }
}
