#include <vector>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <iomanip>
#include <random>

#include "AnalysisManager.hh"
#include "PixelMap3D.hh"
#include "geometry/GeometricalParameters.hh"
#include "LArBoxSD.hh"
#include "LArBoxHit.hh"
#include "PrimaryParticleInformation.hh"
#include "reco/PCAAnalysis3D.hh"
#include "reco/Cluster3D.hh"
#include "reco/LinearFit.hh"
#include "reco/ShowerLID.hh"
#include "reco/CircleFit.hh"

#include <G4Event.hh>
#include <G4SDManager.hh>
#include <G4SystemOfUnits.hh>
#include <Randomize.hh>
#include <G4Poisson.hh>

#include <TFile.h>
#include <TTree.h>
#include <TH2F.h>
#include <THnSparse.h>
#include <TString.h>
#include <Math/ProbFunc.h>
//#include <Math/Point3Dfwd.h>
//#include <Math/Point3D.h>

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
  m_addDiffusion = "false";
  m_saveHit = false;
  m_save3DEvd = false;
  m_save2DEvd = false;
  m_circularFit = false;
}

AnalysisManager::~AnalysisManager() {;}

void AnalysisManager::bookEvtTree() {
  evt = new TTree("evt", "evtTreeInfo");
  evt->Branch("evtID"                     , &evtID                     , "evtID/I");
  evt->Branch("nuIdx"                     , &nuIdx                     , "nuIdx/I");
  evt->Branch("nuPDG"                     , &nuPDG                     , "nuPDG/I");
  evt->Branch("nuE"                       , &nuE                       , "nuE/D");
  evt->Branch("nuX"                       , &nuX                       , "nuX/D");
  evt->Branch("nuY"                       , &nuY                       , "nuY/D");
  evt->Branch("nuZ"                       , &nuZ                       , "nuZ/D");
  evt->Branch("nuIntType"                 , &nuIntType                 , "nuIntType/I");
  evt->Branch("nuScatteringType"          , &nuScatteringType          , "nuScatteringType/I");
  evt->Branch("W"                         , &nuW                       , "W/D");
  evt->Branch("nuFSLPDG"                  , &nuFSLPDG                  , "nuFSLPDG/I");
  evt->Branch("nuFSLPx"                   , &nuFSLPx                   , "nuFSLPx/D");
  evt->Branch("nuFSLPy"                   , &nuFSLPy                   , "nuFSLPy/D");
  evt->Branch("nuFSLPz"                   , &nuFSLPz                   , "nuFSLPz/D");
  evt->Branch("nuFSLE"                    , &nuFSLE                    , "nuFSLE/D");
  evt->Branch("TotalDedxLongitudinal"     , TotalDedxLongitudinal      , "TotalDedxLongitudinal[3000]/D");
  evt->Branch("TrueTotalDedxLongitudinal" , TrueTotalDedxLongitudinal  , "TrueTotalDedxLongitudinal[3000]/D");
  evt->Branch("nPrimaryParticle"          , &nPrimaryParticle          , "nPrimaryParticle/I");
  evt->Branch("Px"                        , Px                         , "Px[nPrimaryParticle]/D");
  evt->Branch("Py"                        , Py                         , "Py[nPrimaryParticle]/D");
  evt->Branch("Pz"                        , Pz                         , "Pz[nPrimaryParticle]/D");
  evt->Branch("VtxX"                      , VtxX                       , "VtxX[nPrimaryParticle]/D");
  evt->Branch("VtxY"                      , VtxY                       , "VtxY[nPrimaryParticle]/D");
  evt->Branch("VtxZ"                      , VtxZ                       , "VtxZ[nPrimaryParticle]/D");
  evt->Branch("Pmass"                     , Pmass                      , "Pmass[nPrimaryParticle]/D");
  evt->Branch("primaryParentID"           , primaryParentID            , "primaryParentID[nPrimaryParticle]/I");
  evt->Branch("primaryParentPDG"          , primaryParentPDG           , "primaryParentPDG[nPrimaryParticle]/I");
  evt->Branch("primaryTrackID"            , primaryTrackID             , "primaryTrackID[nPrimaryParticle]/I");
  evt->Branch("primaryTrackPDG"           , primaryTrackPDG            , "primaryTrackPDG[nPrimaryParticle]/I");
  evt->Branch("primaryTrackLength"        , primaryTrackLength         , "primaryTrackLength[nPrimaryParticle]/D");
  evt->Branch("primaryTrackLengthInTPC"   , primaryTrackLengthInTPC    , "primaryTrackLengthInTPC[nPrimaryParticle]/D");
  evt->Branch("prongIndex"                , prongIndex                 , "prongIndex[nPrimaryParticle]/I");
  evt->Branch("prongType"                 , prongType                  , "prongType[nPrimaryParticle]/I");
  evt->Branch("EInDetector"               , EInDetector                , "EInDetector[nPrimaryParticle]/D");
  evt->Branch("EInLAr"                    , EInLAr                     , "EInLAr[nPrimaryParticle]/D");
  evt->Branch("EInHadCal"                 , EInHadCal                  , "EInHadCal[nPrimaryParticle]/D");
  evt->Branch("EInMuonFinder"             , EInMuonFinder              , "EInMuonFinder[nPrimaryParticle]/D");
  evt->Branch("EInMuonFinderLayer1X"      , EInMuonFinderLayer1X       , "EInMuonFinderLayer1X[nPrimaryParticle]/D");
  evt->Branch("EInMuonFinderLayer1Y"      , EInMuonFinderLayer1Y       , "EInMuonFinderLayer1Y[nPrimaryParticle]/D");
  evt->Branch("EInMuonFinderLayer2X"      , EInMuonFinderLayer2X       , "EInMuonFinderLayer2X[nPrimaryParticle]/D");
  evt->Branch("EInMuonFinderLayer2Y"      , EInMuonFinderLayer2Y       , "EInMuonFinderLayer2Y[nPrimaryParticle]/D");
  evt->Branch("AngleToBeamDir"            , AngleToBeamDir             , "AngleToBeamDir[nPrimaryParticle]/D");
  evt->Branch("ShowerLength"              , ShowerLength               , "ShowerLength[nPrimaryParticle]/D");
  evt->Branch("ShowerLengthInLAr"         , ShowerLengthInLAr          , "ShowerLengthInLAr[nPrimaryParticle]/D");
  evt->Branch("ShowerWidth"               , ShowerWidth                , "ShowerWidth[nPrimaryParticle]/D");
  evt->Branch("ShowerWidthInLAr"          , ShowerWidthInLAr           , "ShowerWidthInLAr[nPrimaryParticle]/D");
  evt->Branch("dEdx"                      , dEdx                       , "dEdx[nPrimaryParticle]/D");
  evt->Branch("dEdxInLAr"                 , dEdxInLAr                  , "dEdxInLAr[nPrimaryParticle]/D");
  evt->Branch("dEdxAlongTrack"            , dEdxAlongTrack             , "dEdxAlongTrack[nPrimaryParticle][100]/D");
  evt->Branch("dEdxTrackLength"           , dEdxTrackLength            , "dEdxTrackLength[nPrimaryParticle][100]/I");
  evt->Branch("dir_pol_x"                 , dir_pol_x                  , "dir_pol_x[nPrimaryParticle]/D");
  evt->Branch("dir_pol_y"                 , dir_pol_y                  , "dir_pol_y[nPrimaryParticle]/D");
  evt->Branch("dir_pol_z"                 , dir_pol_z                  , "dir_pol_z[nPrimaryParticle]/D");
  evt->Branch("dir_coc_x"                 , dir_coc_x                  , "dir_coc_x[nPrimaryParticle]/D");
  evt->Branch("dir_coc_y"                 , dir_coc_y                  , "dir_coc_y[nPrimaryParticle]/D");
  evt->Branch("dir_coc_z"                 , dir_coc_z                  , "dir_coc_z[nPrimaryParticle]/D");

  evt->Branch("nHits"                     , &nHits                     , "nHits/I");
  evt->Branch("sparseFractionMem"         , &sparseFractionMem         , "sparseFractionMem/D");
  evt->Branch("sparseFractionBins"        , &sparseFractionBins        , "sparseFractionBins/D");
  if (m_saveHit) {
    evt->Branch("HitTID"                  , HitTID                     , "HitTID[nHits]/I");
    evt->Branch("HitPID"                  , HitPID                     , "HitPID[nHits]/I");
    evt->Branch("HitPDG"                  , HitPDG                     , "HitPDG[nHits]/I");
    evt->Branch("HitTrackStatus"          , HitTrackStatus             , "HitTrackStatus[nHits]/I");
    evt->Branch("HitPrePositionX"         , HitPrePositionX            , "HitPrePositionX[nHits]/D");
    evt->Branch("HitPrePositionY"         , HitPrePositionY            , "HitPrePositionY[nHits]/D");
    evt->Branch("HitPrePositionZ"         , HitPrePositionZ            , "HitPrePositionZ[nHits]/D");
    evt->Branch("HitPosPositionX"         , HitPosPositionX            , "HitPosPositionX[nHits]/D");
    evt->Branch("HitPosPositionY"         , HitPosPositionY            , "HitPosPositionY[nHits]/D");
    evt->Branch("HitPosPositionZ"         , HitPosPositionZ            , "HitPosPositionZ[nHits]/D");
    evt->Branch("HitEdep"                 , HitEdep                    , "HitEdep[nHits]/D");
  }

  evt->Branch("edepInLAr"                 , &edepInLAr                 , "edepInLAr/D");
  evt->Branch("edepInLArXY2500mm"         , &edepInLArXY2500mm         , "edepInLArXY2500mm/D");
  evt->Branch("edepInLArXY2000mm"         , &edepInLArXY2000mm         , "edepInLArXY2000mm/D");
  evt->Branch("edepInLArXY1500mm"         , &edepInLArXY1500mm         , "edepInLArXY1500mm/D");
  evt->Branch("edepInLArXY1000mm"         , &edepInLArXY1000mm         , "edepInLArXY1000mm/D");
  evt->Branch("edepInHadCalX"             , &edepInHadCalX             , "edepInHadCalX/D");
  evt->Branch("edepInHadCalY"             , &edepInHadCalY             , "edepInHadCalY/D");
  evt->Branch("edepInMuonFinderX"         , &edepInMuonFinderX         , "edepInMuonFinderX/D");
  evt->Branch("edepInMuonFinderY"         , &edepInMuonFinderY         , "edepInMuonFinderY/D");
  evt->Branch("edepInHadAborb"            , &edepInHadAborb            , "edepInHadAborb/D");
  evt->Branch("edepInMuonFinderAbsorb"    , &edepInMuonFinderAbsorb    , "edepInMuonFinderAbsorb/D");
  evt->Branch("missCountedEnergy"         , &missCountedEnergy         , "missCountedEnergy/D");
  
  evt->Branch("nFromFSLParticles"         , &nFromFSLParticles         , "nFromFSLParticles/I");

  if(m_circularFit){
    evt->Branch("circNhits"               , &circNhits                 , "circNhits/I");
    evt->Branch("preTrkNhits"             , &preTrkNhits               , "preTrkNhits/I");
    evt->Branch("postTrkNhits"            , &postTrkNhits              , "postTrkNhits/I");
    evt->Branch("circStatus"              , &circStatus                , "circStatus/I"); 
    evt->Branch("lineStatus"              , &lineStatus                , "lineStatus/I"); 
    evt->Branch("circXc"                  , &xc                        , "circXc/D"); 
    evt->Branch("circZc"                  , &zc                        , "circZc/D");
    evt->Branch("circRc"                  , &rc                        , "circRc/D");
    evt->Branch("circChi2"                , &chi2c                     , "circChi2/D"); 
    evt->Branch("circp0"                  , &p0                        , "circp0/D"); 
    evt->Branch("circp1"                  , &p1                        , "circp1/D");
    evt->Branch("circcosDip"              , &cosDip                    , "circcosDip/D");
    evt->Branch("lineChi2"                , &chi2l                     , "lineChi2/D"); 
    evt->Branch("trkXc"                   , &trkxc                     , "trkXc/D"); 
    evt->Branch("trkZc"                   , &trkzc                     , "trkZc/D");
    evt->Branch("trkRc"                   , &trkrc                     , "trkRc/D");
    evt->Branch("trkp0"                   , &trkp0                     , "trkp0/D"); 
    evt->Branch("trkp1"                   , &trkp1                     , "trkp1/D");
    evt->Branch("trkcosDip"               , &trkcosDip                 , "trkcosDip/D");
    evt->Branch("trkqIn"                  , &trkqIn                    , "trkqIn/D");
    evt->Branch("trkmIn"                  , &trkmIn                    , "trkmIn/D");
    evt->Branch("trkqOut"                 , &trkqOut                   , "trkqOut/D");
    evt->Branch("trkmOut"                 , &trkmOut                   , "trkmOut/D");
    evt->Branch("circHitXFSL"             , &hitXFSL);
    evt->Branch("circHitYFSL"             , &hitYFSL);
    evt->Branch("circHitZFSL"             , &hitZFSL);        
    evt->Branch("circHitPFSL"             , &hitPFSL);        
    evt->Branch("preHitTrkXFSL"           , &preTrkXFSL);
    evt->Branch("preHitTrkYFSL"           , &preTrkYFSL);
    evt->Branch("preHitTrkZFSL"           , &preTrkZFSL);        
    evt->Branch("preHitTrkPFSL"           , &preTrkPFSL);        
    evt->Branch("postHitTrkXFSL"          , &postTrkXFSL);
    evt->Branch("postHitTrkYFSL"          , &postTrkYFSL);
    evt->Branch("postHitTrkZFSL"          , &postTrkZFSL);        
    evt->Branch("postHitTrkPFSL"          , &postTrkPFSL);        
  }
}

void AnalysisManager::BeginOfRun() {
  G4cout<<"TTree is booked and the run has been started"<<G4endl;
  if (thefile) {
    delete thefile;
  }
  thefile = new TFile(m_filename, "RECREATE");
  bookEvtTree();

  NumberOfSDs = GeometricalParameters::Get()->GetSDNamelist().size();
  G4cout<<"Number of SDs : "<<NumberOfSDs<<G4endl;
  for (auto sdname : GeometricalParameters::Get()->GetSDNamelist()) {
    SDNamelist.insert(sdname);
    G4cout<<sdname.first<<" "<<sdname.second<<G4endl;
  }
}

void AnalysisManager::EndOfRun() {
  thefile->cd();
  evt->Write();
  thefile->Close();
}

void AnalysisManager::BeginOfEvent() {
  nuIdx                        = -1;
  nuPDG                        = 0;
  nuE                          = -999;
  nuX                          = -999;
  nuY                          = -999;
  nuZ                          = -999;
  nuIntType                    = -1;
  nuScatteringType             = -1;
  nuW                          = -1;
  nuFSLPDG                     = 0;
  nuFSLPx                      = -999;
  nuFSLPy                      = -999;
  nuFSLPz                      = -999;
  nuFSLE                       = -999;
  nHits                        = 0;
  sparseFractionMem            = -1;
  sparseFractionBins           = -1;
  edepInLAr                    = 0;
  edepInLArXY2500mm            = 0;
  edepInLArXY2000mm            = 0;
  edepInLArXY1500mm            = 0;
  edepInLArXY1000mm            = 0;
  edepInHadCalX                = 0;
  edepInHadCalY                = 0;
  edepInMuonFinderX            = 0;
  edepInMuonFinderY            = 0;
  edepInHadAborb               = 0;
  edepInMuonFinderAbsorb       = 0;
  missCountedEnergy            = 0;
  nPrimaryParticle             = 0;
  nFromFSLParticles            = 0;
  nFromFSPizeroParticles       = 0;
  nFromFSLDecayPizeroParticles = 0;
  countPrimaryParticle         = 0;
  for (G4int j= 0; j< 3000; ++j) {
    TotalDedxLongitudinal[j] = 0;
    TrueTotalDedxLongitudinal[j] = 0;
  }
  for (G4int i= 0; i< 1000; ++i) {
    Px[i]    = -999;
    Py[i]    = -999;
    Pz[i]    = -999;
    VtxX[i]  = -999;
    VtxY[i]  = -999;
    VtxZ[i]  = -999;
    Pmass[i] = -999;
    primaryParentID[i]         = -1;
    primaryParentPDG[i]        = 0;
    primaryTrackID[i]          = -1;
    primaryTrackPDG[i]         = 0;
    prongIndex[i]              = -1;
    prongType[i]               = -1;
    primaryTrackLength[i]      = 0;
    primaryTrackLengthInTPC[i] = 0;
    EInDetector[i]             = 0;
    EInLAr[i]                  = 0;
    EInHadCal[i]               = 0;
    EInMuonFinder[i]           = 0;
    EInMuonFinderLayer1X[i]    = 0;
    EInMuonFinderLayer1Y[i]    = 0;
    EInMuonFinderLayer2X[i]    = 0;
    EInMuonFinderLayer2Y[i]    = 0;
    AngleToBeamDir[i]          = -1;
    ShowerLength[i]            = -1;
    ShowerLengthInLAr[i]       = -1;
    ShowerWidth[i]             = 0;
    ShowerWidthInLAr[i]        = 0;
    dEdx[i]                    = -1;
    dEdxInLAr[i]               = -1;
    for (G4int j= 0; j< 100; ++j) {
      dEdxAlongTrack[i][j] = 0;
      dEdxTrackLength[i][j] = -1;
    }
    dir_pol_x[i]               = -999;
    dir_pol_y[i]               = -999;
    dir_pol_z[i]               = -999;
    dir_coc_x[i]               = -999;
    dir_coc_y[i]               = -999;
    dir_coc_z[i]               = -999;
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
  tracksFromFSPizeroSecondary.clear();
  tracksFromFSLDecayPizeroSecondary.clear();
  fPrimIdxFSL = -1;

  hitXFSL.clear();
  hitYFSL.clear();
  hitZFSL.clear();
  hitPFSL.clear();
  preTrkXFSL.clear();
  preTrkYFSL.clear();
  preTrkZFSL.clear();
  preTrkPFSL.clear();
  postTrkXFSL.clear();
  postTrkYFSL.clear();
  postTrkZFSL.clear();
  postTrkPFSL.clear();
}

void AnalysisManager::EndOfEvent(const G4Event* event) {
  /// Branch: evtID
  evtID = event->GetEventID();

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
          nuW              = primary_particle_info->GetW();
          nuFSLPDG         = primary_particle_info->GetFSLPDG();
          nuFSLPx          = primary_particle_info->GetFSLP4().X();
          nuFSLPy          = primary_particle_info->GetFSLP4().Y();
          nuFSLPz          = primary_particle_info->GetFSLP4().Z();
          nuFSLE           = primary_particle_info->GetFSLP4().T();
          continue;
        }
      }
    }
  }
  nPrimaryVertex   = event->GetNumberOfPrimaryVertex();
  std::cout<<"\nnumber of primary vertices  : "<<nPrimaryVertex<<std::endl;

  G4SDManager* sdm = G4SDManager::GetSDMpointer();
  // Get the hit collections
  hcofEvent = event->GetHCofThisEvent();
  // If there is no hit collection, there is nothing to be done
  if (!hcofEvent) return;

  // loop over all sensitive detectors
  const G4int nsds = 12;
  G4int sdids[nsds] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
  std::string sds[nsds] = {"lArBoxSD/lar_box",
                           "HadCalXSD/lar_box",
                           "HadCalYSD/lar_box",
                           "MuonFinderXSD/lar_box",
                           "MuonFinderYSD/lar_box",
                           "HadAbsorbSD/lar_box",
                           "MuonFinderAbsorbSD/lar_box",
                           "FORMOSAScinBarSD/lar_box",
                           "FASERnu2EmulsionSD/lar_box",
                           "FASERnu2VetoInterfaceSD/lar_box",
                           "TrkHorScinSD/lar_box",
                           "TrkVerScinSD/lar_box"};

  for (int i= 0; i< nsds; ++i) {
    if (sdids[i]<0) {
      sdids[i] = sdm->GetCollectionID(sds[i]);
    }
    if (sdids[i]>=0) {
      G4cout<<sdids[i]<<" "<<sds[i]<<G4endl;
      FillPrimaryTruthTree(sdids[i], sds[i]);
    }
  }
  // update number of primary particles
  // including decay products from tau and pizero
  nPrimaryParticle = countPrimaryParticle;
  nFromFSLParticles = tracksFromFSLSecondary.size();
  nFromFSPizeroParticles = tracksFromFSPizeroSecondary.size();
  nFromFSLDecayPizeroParticles = tracksFromFSLDecayPizeroSecondary.size();

  // in case this is not a neutrino event
  //if (nuPDG==0) {
  //  nuX = VtxX[0];
  //  nuY = VtxY[0];
  //  nuZ = VtxZ[0];
  //}

  // find all the tracks originate from the final state lepton, include FSL itself (TID=1)
  // should only work with neutrino interaction generator
  // exception to single particle generator: tau, mu
  if (nuPDG!=0 || abs(primaryTrackPDG[0])==15 || abs(primaryTrackPDG[0])==13) {
    tracksFromFSL.insert(1);
    for (auto x : allTracksPTPair) {
      if (tracksFromFSL.find(x.first) != tracksFromFSL.end()) {
        tracksFromFSL.insert(x.second);
      }
    }
  }
  // tracksFromFSL includes all the tracks orginating from the fsl
  // tracksFromFSLSecondary only inclues the tracks directly decayed from the fsl
  std::cout<<"Recorded tracks       : "<<allTracksPTPair.size()<<std::endl;
  std::cout<<"Tracks from FSL       : "<<tracksFromFSL.size()<<std::endl;
  std::cout<<"Tracks from FSL (2nd) : "<<tracksFromFSLSecondary.size()<<std::endl;
  std::cout<<"number of primary particles : "<<nPrimaryParticle
    <<" , in which number of particles from fsl : "<<nFromFSLParticles<<std::endl;

  //- cluster all tracks to primary particles
  //- mark the index of the final state lepton from the neutrino interaction
  trackClusters.resize(nPrimaryParticle);
  for (int iPrim= 0; iPrim< nPrimaryParticle; ++iPrim) {
    trackClusters[iPrim].insert(primaryTrackID[iPrim]);
    if (primaryTrackID[iPrim]==1 && nuPDG!=0) {
      fPrimIdxFSL = iPrim;
    }
  }
  if (fPrimIdxFSL>=0) prongType[fPrimIdxFSL] = 0;
  
  for (auto x : allTracksPTPair) {
    // if this track is the fsl (TID=1) and it decays (nFromFSLParticles>0),
    // then it forms a single cluster by itself, this is mainly for studying the tau decay.
    if ((x.second==1) && (nFromFSLParticles>0)) continue;
    // if this track is the decay product of the fsl, it should already been added to the trackClusters
    if ((x.first==1) && (nFromFSLParticles>0) && (tracksFromFSLSecondary.find(x.second) != tracksFromFSLSecondary.end())) continue;
    // if this is the decay product of the final state pizero, it should already been added to the trackClusters
    if ((tracksFromFSPizeroSecondary.find(x.second) != tracksFromFSPizeroSecondary.end())) continue;
    // if this is the decay product of the tau decay pizero, it should already been added to the trackClusters
    if ((tracksFromFSLDecayPizeroSecondary.find(x.second) != tracksFromFSLDecayPizeroSecondary.end())) continue;
    // add the track to the corresponding cluster if its parent is in the cluster.
    // one track can have only one parent, break the loop once its parent is found.
    for (int iPrim= 0; iPrim< nPrimaryParticle; ++iPrim) {
      if (trackClusters[iPrim].find(x.first) != trackClusters[iPrim].end()) {
        trackClusters[iPrim].insert(x.second);
        break;
      }
    }
  }

  InitializeEvd();
  /// FillTrueEdep must run after FillPrimaryTruthTree, otherwise tracksFromFSL and tracksFromFSLSecondary are invalid
  for (int i= 0; i< nsds; ++i) {
    if (sdids[i]>=0) {
      FillTrueEdep(sdids[i], sds[i]);
    }
  }

  sparseFractionMem = hist3D->GetSparseFractionMem();
  sparseFractionBins = hist3D->GetSparseFractionBins();


  //slid::ShowerLID* shwlid = new slid::ShowerLID(hist3DEdep, nuX, nuY, nuZ, 0., 0., 1.); 
  //Double_t* ptr_dedx = shwlid->GetTotalDedxLongitudinal();
  //std::copy(ptr_dedx, ptr_dedx+3000, TotalDedxLongitudinal);

  /*
  for(int iPrim= 0; iPrim< nPrimaryParticle; ++iPrim) {
    directionfitter::LinearFit* linFit = new directionfitter::LinearFit(hitClusterZX[iPrim+1], hitClusterZY[iPrim+1],
        vtxHitClusterZX[iPrim+1], vtxHitClusterZY[iPrim+1], nuX, nuY, nuZ, VtxX[iPrim], VtxY[iPrim], VtxZ[iPrim]);
    dir_pol_x[iPrim] = linFit->GetDir().X();
    dir_pol_y[iPrim] = linFit->GetDir().Y();
    dir_pol_z[iPrim] = linFit->GetDir().Z();
    dir_coc_x[iPrim] = linFit->GetCOCDir().X();
    dir_coc_y[iPrim] = linFit->GetCOCDir().Y();
    dir_coc_z[iPrim] = linFit->GetCOCDir().Z();
    delete linFit;
  }
  */

  if (m_circularFit){
    
    circNhits = hitXFSL.size();
    preTrkNhits = preTrkXFSL.size();
    postTrkNhits = postTrkXFSL.size();
    
    if ( circNhits > 0 ){
      circularfitter::CircleFit* circFit = new circularfitter::CircleFit(hitXFSL,hitZFSL);
      circularfitter::LineFit* lineFit = new circularfitter::LineFit(hitZFSL,hitYFSL);
      circStatus = circFit->GetStatus();
      lineStatus = lineFit->GetStatus();
      xc = circFit->GetXc();
      zc = circFit->GetZc();
      rc = circFit->GetR();
      p0 = lineFit->GetP0();
      p1 = lineFit->GetP1();
      cosDip = lineFit->GetCosDip();
      chi2c = circFit->GetChi2();
      chi2l = lineFit->GetChi2();
    }

    if( preTrkNhits > 0 && postTrkNhits > 0){
      circularfitter::CircleExtractor* circExtract = new circularfitter::CircleExtractor(preTrkZFSL,preTrkXFSL,postTrkZFSL,postTrkXFSL);
      trkxc = circExtract->GetXc();
      trkzc = circExtract->GetZc();
      trkrc = circExtract->GetR();
      circularfitter::line prel = circExtract->GetPreLine();
      circularfitter::line postl = circExtract->GetPostLine();
      trkqIn = prel.q;
      trkmIn = prel.m;
      trkqOut = postl.q;
      trkmOut = postl.m;
      
      std::vector<double> allZ = preTrkZFSL;
      allZ.insert( allZ.end(), postTrkZFSL.begin(), postTrkZFSL.end());
      std::vector<double> allY = preTrkYFSL;
      allY.insert( allY.end(), postTrkYFSL.begin(), postTrkYFSL.end());
      circularfitter::LineFit* lineFit2 = new circularfitter::LineFit(allZ,allY);
      trkp0 = lineFit2->GetP0();
      trkp1 = lineFit2->GetP1();
      trkcosDip = lineFit2->GetCosDip();
    }
  }
  
  // FillPseudoRecoVar must run after FillTrueEdep, otherwise some of the variables won't be filled
  FillPseudoRecoVar();

  evt->Fill();

  std::cout<<"Total number of hits : "<<nHits<<std::endl;

  if (m_save3DEvd || m_save2DEvd) {
    hist3D->WriteToFile(thefile, m_save3DEvd, m_save2DEvd);
  }

  for (int iPrim= 0; iPrim< nPrimaryParticle; ++iPrim) {
    trackClusters[iPrim].clear();
  }
  trackClusters.clear();
  trackClusters.shrink_to_fit();

  delete hist3D;
}

void AnalysisManager::FillPrimaryTruthTree(G4int sdId, std::string sdName) {
  int detID = 0;
  const std::unordered_map<std::string,std::function<void()>> detIDs{
          {"lArBoxSD/lar_box",                [&](){ detID = 1; }},
          {"HadCalXSD/lar_box",               [&](){ detID = 2; }},
          {"HadCalYSD/lar_box",               [&](){ detID = 3; }},
          {"MuonFinderXSD/lar_box",           [&](){ detID = 4; }},
          {"MuonFinderYSD/lar_box",           [&](){ detID = 5; }},
          {"HadAbsorbSD/lar_box",             [&](){ detID = 6; }},
          {"MuonFinderAbsorbSD/lar_box",      [&](){ detID = 7; }},
          {"FORMOSAScinBarSD/lar_box",        [&](){ detID = 8; }},
          {"FASERnu2EmulsionSD/lar_box",      [&](){ detID = 9; }},
          {"FASERnu2VetoInterfaceSD/lar_box", [&](){ detID = 10; }},
          {"TrkHorScinSD/lar_box",            [&](){ detID = 11; }},
          {"TrkVerScinSD/lar_box",            [&](){ detID = 12; }},
  };
  detIDs.find(sdName)->second();

  // Get and cast hit collection with LArBoxHits
  LArBoxHitsCollection* hitCollection = dynamic_cast<LArBoxHitsCollection*>(hcofEvent->GetHC(sdId));
  if (hitCollection) {
    for (auto hit: *hitCollection->GetVector()) {
      nHits++;

      double pre_x  = hit->GetPreStepPosition().x();
      double pre_y  = hit->GetPreStepPosition().y();
      double pre_z  = hit->GetPreStepPosition().z() + 3500;
      double post_x = hit->GetPostStepPosition().x();
      double post_y = hit->GetPostStepPosition().y();
      double post_z = hit->GetPostStepPosition().z() + 3500;
      
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
      }

      // save FSL (only muons!) hits for circle fitting
      if( TMath::Abs(hit->GetParticle())==13 && hit->GetPID() == 0 ){
        double px = hit->GetInitMomentum().x();
        double pz = hit->GetInitMomentum().z();
        double p_perp = TMath::Sqrt(px*px+pz*pz);
        if( detID > 1 && detID < 6){
          hitXFSL.push_back(post_x);
          hitYFSL.push_back(post_y);
          hitZFSL.push_back(post_z);
          hitPFSL.push_back(p_perp);
        }
        else if( detID > 10 && post_z < GeometricalParameters::Get()->GetMagnetZPosition()+3500*mm){
          preTrkXFSL.push_back(post_x);
          preTrkYFSL.push_back(post_y);
          preTrkZFSL.push_back(post_z);
          preTrkPFSL.push_back(p_perp);
        }
        else if ( detID > 10 && post_z > GeometricalParameters::Get()->GetMagnetZPosition()+3500*mm){
          postTrkXFSL.push_back(post_x);
          postTrkYFSL.push_back(post_y);
          postTrkZFSL.push_back(post_z);
          postTrkPFSL.push_back(p_perp);
        }
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
          prongType[countPrimaryParticle-1]       = 1;
          prongIndex[countPrimaryParticle-1]      = countPrimaryParticle-1;
        }
      }
      // in case of the fsl decay, the decay products are counted as primary particles
      // * tau- decay (dominant)
      // * mu- decay
      //if (hit->GetPID()==1 && hit->GetCreatorProcess()=="Decay") {
      if (hit->GetIsTrackFromPrimaryLepton()) {
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
          prongType[countPrimaryParticle-1]       = 2;
          prongIndex[countPrimaryParticle-1]      = countPrimaryParticle-1;
        }
      }
      // in case of pizero in the list of primary tracks, its decay products are also counted as primary particles
      if (hit->GetIsTrackFromPrimaryPizero()) {
        tracksFromFSPizeroSecondary.insert(hit->GetTID());
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
          prongType[countPrimaryParticle-1]       = 3;
          prongIndex[countPrimaryParticle-1]      = countPrimaryParticle-1;
        }
      }
      // in case of tau decay pizero
      if (hit->GetIsTrackFromFSLPizero()) {
        tracksFromFSLDecayPizeroSecondary.insert(hit->GetTID());
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
          prongType[countPrimaryParticle-1]       = 4;
          prongIndex[countPrimaryParticle-1]      = countPrimaryParticle-1;
        }
      }
    } // end of hit loop
  }
}

void AnalysisManager::FillTrueEdep(G4int sdId, std::string sdName) {
  int detID = 0;
  const std::unordered_map<std::string,std::function<void()>> detIDs{
          {"lArBoxSD/lar_box",                [&](){ detID = 1; }},
          {"HadCalXSD/lar_box",               [&](){ detID = 2; }},
          {"HadCalYSD/lar_box",               [&](){ detID = 3; }},
          {"MuonFinderXSD/lar_box",           [&](){ detID = 4; }},
          {"MuonFinderYSD/lar_box",           [&](){ detID = 5; }},
          {"HadAbsorbSD/lar_box",             [&](){ detID = 6; }},
          {"MuonFinderAbsorbSD/lar_box",      [&](){ detID = 7; }},
          {"FORMOSAScinBarSD/lar_box",        [&](){ detID = 8; }},
          {"FASERnu2EmulsionSD/lar_box",      [&](){ detID = 9; }},
          {"FASERnu2VetoInterfaceSD/lar_box", [&](){ detID = 10; }},
          {"TrkHorScinSD/lar_box",            [&](){ detID = 11; }},
          {"TrkVerScinSD/lar_box",            [&](){ detID = 12; }},
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
        std::cout<<"Can't find the primary particle of the hit, something is wrong "<<hit->GetParticle()
          <<" edep("<<hit->GetEdep()<<") TID("<<hit->GetTID()<<") PID("<<hit->GetPID()
          <<") creator process ("<<hit->GetCreatorProcess()<<") position"
          <<hit->GetEdepPosition()<<std::endl; 
        missCountedEnergy += hit->GetEdep();
        continue;
      } 

      double pos_x = hit->GetEdepPosition().x();
      double pos_y = hit->GetEdepPosition().y();
      double pos_z = hit->GetEdepPosition().z();
      double ShowerP = TMath::Sqrt(Px[whichPrim]*Px[whichPrim]+Py[whichPrim]*Py[whichPrim]+Pz[whichPrim]*Pz[whichPrim]);
      double hit_position_xyz[3] = {pos_x, pos_y, pos_z};
      double vtx_xyz[3];
      if (nuPDG!=0) {
        vtx_xyz[0] = nuX; 
        vtx_xyz[1] = nuY; 
        vtx_xyz[2] = nuZ;
      }
      else {
        vtx_xyz[0] = VtxX[0]; 
        vtx_xyz[1] = VtxY[0];
        vtx_xyz[2] = VtxZ[0];
      }

      if (detID==1 && m_addDiffusion=="toy") {
        hist3D->FillEntryWithToyElectronTransportation(hit_position_xyz, 
                                                       vtx_xyz, 
                                                       hit->GetEdep(), 
                                                       whichPrim);
      } else if (detID==1 && m_addDiffusion=="single") {
        hist3D->FillEntryWithToySingleElectronTransportation(hit_position_xyz, 
                                                       vtx_xyz, 
                                                       hit->GetEdep(), 
                                                       whichPrim);
      } else {
        hist3D->FillEntry(hit_position_xyz, vtx_xyz, hit->GetEdep(), whichPrim);
      }
      if (detID==1) {
        double longitudinal_distance_to_vtx;  // in mm
        if (nuPDG!=0) {
          longitudinal_distance_to_vtx = (pos_z-vtx_xyz[2]);
        } else {
          longitudinal_distance_to_vtx = ((pos_x-VtxX[0])*Px[0]+
                                          (pos_y-VtxY[0])*Py[0]+
                                          (pos_z-VtxZ[0])*Pz[0])/TMath::Sqrt(Px[0]*Px[0]+Py[0]*Py[0]+Pz[0]*Pz[0]);
        }
        if (Int_t(longitudinal_distance_to_vtx)>=0 && Int_t(longitudinal_distance_to_vtx)<3000) {  // within 3000 mm
          TrueTotalDedxLongitudinal[Int_t(longitudinal_distance_to_vtx)] += hit->GetEdep();
        }
      }
      // calculate dEdx along the track
      // combine the tracks if they come from the final state lepton, namely tau-
      if ((hit->GetPID()==0) |
          (tracksFromFSLSecondary.find(hit->GetTID()) != tracksFromFSLSecondary.end()) |
          (tracksFromFSPizeroSecondary.find(hit->GetTID()) != tracksFromFSPizeroSecondary.end()) |
          (tracksFromFSLDecayPizeroSecondary.find(hit->GetTID()) != tracksFromFSLDecayPizeroSecondary.end())) {
        primaryTrackLength[whichPrim] += hit->GetStepLength();
        if (detID==1) {
          primaryTrackLengthInTPC[whichPrim] += hit->GetStepLength();
          if ((hit->GetPID()==0) |
              (tracksFromFSPizeroSecondary.find(hit->GetTID()) != tracksFromFSPizeroSecondary.end())) {
            double longitudinal_distance_to_vtx = ((pos_x-VtxX[whichPrim])*Px[whichPrim]+
                                                   (pos_y-VtxY[whichPrim])*Py[whichPrim]+
                                                   (pos_z-VtxZ[whichPrim])*Pz[whichPrim])/ShowerP;
            if (int(longitudinal_distance_to_vtx)>=0 && int(longitudinal_distance_to_vtx)<100) {  // within 100 mm
              dEdxAlongTrack[whichPrim][int(longitudinal_distance_to_vtx)] += hit->GetEdep();
              dEdxTrackLength[whichPrim][int(longitudinal_distance_to_vtx)] = int(longitudinal_distance_to_vtx);
            }
          } else {
            if (fPrimIdxFSL>=0) {
              double ShowerP_FSL = TMath::Sqrt(Px[fPrimIdxFSL]*Px[fPrimIdxFSL]+
                                               Py[fPrimIdxFSL]*Py[fPrimIdxFSL]+
                                               Pz[fPrimIdxFSL]*Pz[fPrimIdxFSL]);
              double longitudinal_distance_to_vtx = ((pos_x-VtxX[fPrimIdxFSL])*Px[fPrimIdxFSL]+
                                                     (pos_y-VtxY[fPrimIdxFSL])*Py[fPrimIdxFSL]+
                                                     (pos_z-VtxZ[fPrimIdxFSL])*Pz[fPrimIdxFSL])/ShowerP_FSL;
              if (int(longitudinal_distance_to_vtx)>=0 && int(longitudinal_distance_to_vtx)<100) {  // within 100 mm
                dEdxAlongTrack[fPrimIdxFSL][int(longitudinal_distance_to_vtx)] += hit->GetEdep();
                dEdxTrackLength[fPrimIdxFSL][int(longitudinal_distance_to_vtx)] = int(longitudinal_distance_to_vtx);
              }
            }
          }
        }
      }
      // calculate the shower/track length and width
      // length: defined as the longest projection distance at the true direction between vertex and hits
      //         length = |\vector{hit_position}|\cdot\cos(theta) = \vertor{hit_position}\cdot\vector{P} / |\vector{P}|
      // width: defined as the weighted average of the least distance of the hits to the true direction
      double dsquare_hit_vtx = TMath::Power((pos_x-VtxX[whichPrim]),2)+
                               TMath::Power((pos_y-VtxY[whichPrim]),2)+
                               TMath::Power((pos_z-VtxZ[whichPrim]),2);
      double product_hit_p = (pos_x-VtxX[whichPrim])*Px[whichPrim]+
                             (pos_y-VtxY[whichPrim])*Py[whichPrim]+
                             (pos_z-VtxZ[whichPrim])*Pz[whichPrim];
      double len_hit = TMath::Abs(product_hit_p)/ShowerP;
      double width_hit = TMath::Sqrt((dsquare_hit_vtx - product_hit_p*product_hit_p/ShowerP/ShowerP));
      // exclude zero hit when calculating showerlength of the primary particle
      // exclude hits from the cryo gap (detID=8)
      if (hit->GetEdep()>0 && detID!=8) {
        EInDetector[whichPrim] += hit->GetEdep();
        ShowerLength[whichPrim] = std::max({ShowerLength[whichPrim], len_hit});
        //double square_weighted_width_hit = TMath::Power(width_hit*hit->GetEdep(),2);
        double weighted_width_hit = width_hit*hit->GetEdep();
        if (!std::isnan(weighted_width_hit)) ShowerWidth[whichPrim] += weighted_width_hit;
      }
      if (detID==1) { 
        if (hit->GetEdep()>0) {
          EInLAr[whichPrim] += hit->GetEdep();
          ShowerLengthInLAr[whichPrim] = std::max({ShowerLengthInLAr[whichPrim], len_hit});
          //double square_weighted_width_hit = TMath::Power(width_hit*hit->GetEdep(),2);
          double weighted_width_hit = width_hit*hit->GetEdep();
          if (!std::isnan(weighted_width_hit)) ShowerWidthInLAr[whichPrim] += weighted_width_hit;
        }
      }

      if (detID==2 || detID==3 || detID==6) {
        EInHadCal[whichPrim] += hit->GetEdep();
      }
      if (detID==4 || detID==5 || detID==7) {
        EInMuonFinder[whichPrim] += hit->GetEdep();
        if (detID==4) {
          if (pos_z < 9030) {
            EInMuonFinderLayer1X[whichPrim] += hit->GetEdep();
          } else {
            EInMuonFinderLayer2X[whichPrim] += hit->GetEdep();
          }
        } else if (detID==5) {
          if (pos_z < 9030) {
            EInMuonFinderLayer1Y[whichPrim] += hit->GetEdep();
          } else {
            EInMuonFinderLayer2Y[whichPrim] += hit->GetEdep();
          }
        }
      }
    } // end of hit loop
  }
}

double AnalysisManager::GetTotalEnergy(double px, double py, double pz, double m) {
  return TMath::Sqrt(px*px+py*py+pz*pz+m*m);
}

void AnalysisManager::InitializeEvd() {
  const Double_t res_tpc2[3] = {1, 5, 5}; // mm
  if (nuPDG!=0) {
    hist3D = new PixelMap3D(evtID, nPrimaryParticle, nuPDG, res_tpc2);
  } else {
    hist3D = new PixelMap3D(evtID, nPrimaryParticle, primaryTrackPDG[0], res_tpc2);
  }
  // boundary in global coord.
  hist3D->SetPMBoundary(GeometricalParameters::Get()->GetFLArEPosition()/mm -
                        GeometricalParameters::Get()->GetTPCSizeXYZ()/mm/2,
                        GeometricalParameters::Get()->GetFLArEPosition()/mm +
                        GeometricalParameters::Get()->GetTPCSizeXYZ()/mm/2);
  hist3D->InitializePM();
}

void AnalysisManager::FillPseudoRecoVar() {
  //  AngleToBeamDir, dEdx, dEdxInLAr ProngType
  std::cout<<std::fixed<<std::setw(10)<<"PDG"
    <<std::setw(12)<<"Angle"
    <<std::setw(13)<<"TrackLength"
    <<std::setw(13)<<"ShowerLength"
    <<std::setw(18)<<"ShowerWidthInLAr"
    <<std::setw(12)<<"EInLAr" 
    <<std::setw(12)<<"EInHadCal"
    <<std::setw(12)<<"dEdxInLAr"
    <<std::setw(10)<<"ProngType"
    <<std::setw(12)<<"Pz"<<std::endl;

  for (int iPrim= 0; iPrim< nPrimaryParticle; ++iPrim) { 
    if (EInDetector[iPrim]>0) {
      ShowerWidth[iPrim] = ShowerWidth[iPrim]/EInDetector[iPrim];
    }
    if (EInLAr[iPrim]>0) {
      ShowerWidthInLAr[iPrim] = ShowerWidthInLAr[iPrim]/EInLAr[iPrim];
    }

    double ShowerP = TMath::Sqrt(Px[iPrim]*Px[iPrim]+Py[iPrim]*Py[iPrim]+Pz[iPrim]*Pz[iPrim]);
    double costheta = Pz[iPrim]/ShowerP;
    AngleToBeamDir[iPrim] = TMath::ACos(costheta);

    dEdx[iPrim] = (EInLAr[iPrim] + EInHadCal[iPrim] + EInMuonFinder[iPrim])/ShowerLength[iPrim];
    dEdxInLAr[iPrim] = EInLAr[iPrim]/ShowerLengthInLAr[iPrim];

    std::cout<<std::setiosflags(std::ios::fixed)<<std::setprecision(3);
    std::cout<<std::setw(10)<<primaryTrackPDG[iPrim];
    std::cout<<std::setw(12)<<AngleToBeamDir[iPrim];
    std::cout<<std::setw(13)<<primaryTrackLength[iPrim];
    std::cout<<std::setw(13)<<ShowerLength[iPrim];
    std::cout<<std::setw(18)<<ShowerWidthInLAr[iPrim];
    std::cout<<std::setw(12)<<EInLAr[iPrim] ;
    std::cout<<std::setw(12)<<EInHadCal[iPrim];
    std::cout<<std::setw(12)<<dEdxInLAr[iPrim];
    std::cout<<std::setw(10)<<prongType[iPrim];
    std::cout<<std::setw(12)<<Pz[iPrim]<<std::endl;
  }
}
