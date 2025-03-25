#include <vector>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <iomanip>
#include <random>

#include "AnalysisManager.hh"
#include "geometry/GeometricalParameters.hh"
#include "LArBoxSD.hh"
#include "LArBoxHit.hh"
#include "PrimaryParticleInformation.hh"
#include "reco/PCAAnalysis3D.hh"
#include "reco/Cluster3D.hh"
#include "reco/LinearFit.hh"
#include "reco/ShowerLID.hh"
#include "reco/CircleFit.hh"
#include "FPFParticle.hh"
#include "FPFNeutrino.hh"

#include <G4Event.hh>
#include <G4SDManager.hh>
#include <G4SystemOfUnits.hh>
#include <Randomize.hh>
#include <G4Poisson.hh>
#include <G4Trajectory.hh>

#include <TFile.h>
#include <TTree.h>
#include <TH2F.h>
#include <THnSparse.h>
#include <TString.h>
#include <Math/ProbFunc.h>

using namespace hep_hpc::hdf5;

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
  trk = 0;
  messenger = new AnalysisManagerMessenger(this);
  m_filename = "test.root";
  m_addDiffusion = "false";
  m_saveHit = false;
  m_saveTrack = false;
  m_save3DEvd = false;
  m_save2DEvd = false;
  m_circularFit = false;

  fH5Filename = "test.h5";
}

AnalysisManager::~AnalysisManager() {;}

void AnalysisManager::bookEvtTree() {
  evt = new TTree("evt", "evtTreeInfo");
  evt->Branch("evtID"                     , &evtID                     , "evtID/I");
  evt->Branch("FPFNeutrino"                , &neutrino, 96000, 0);
  evt->Branch("FPFParticle"                , &primaries, 96000, 0);
  evt->Branch("TotalDedxLongitudinal"     , TotalDedxLongitudinal      , "TotalDedxLongitudinal[3000]/D");
  evt->Branch("TrueTotalDedxLongitudinal" , TrueTotalDedxLongitudinal  , "TrueTotalDedxLongitudinal[3000]/D");
  evt->Branch("nPrimaryParticle"          , &nPrimaryParticle          , "nPrimaryParticle/I");
  evt->Branch("primaryParentPDG"          , primaryParentPDG           , "primaryParentPDG[nPrimaryParticle]/I");
  evt->Branch("primaryTrackLength"        , primaryTrackLength         , "primaryTrackLength[nPrimaryParticle]/D");
  evt->Branch("primaryTrackLengthInTPC"   , primaryTrackLengthInTPC    , "primaryTrackLengthInTPC[nPrimaryParticle]/D");
  evt->Branch("ProngEInDetector"          , ProngEInDetector           , "ProngEInDetector[nPrimaryParticle]/D");
  evt->Branch("ProngEInLAr"               , ProngEInLAr                , "ProngEInLAr[nPrimaryParticle]/D");
  evt->Branch("ProngEInHadCal"            , ProngEInHadCal             , "ProngEInHadCal[nPrimaryParticle]/D");
  evt->Branch("ProngEInMuonFinder"        , ProngEInMuonFinder         , "ProngEInMuonFinder[nPrimaryParticle]/D");
  evt->Branch("ProngEInMuonFinderLayer1X" , ProngEInMuonFinderLayer1X  , "ProngEInMuonFinderLayer1X[nPrimaryParticle]/D");
  evt->Branch("ProngEInMuonFinderLayer1Y" , ProngEInMuonFinderLayer1Y  , "ProngEInMuonFinderLayer1Y[nPrimaryParticle]/D");
  evt->Branch("ProngEInMuonFinderLayer2X" , ProngEInMuonFinderLayer2X  , "ProngEInMuonFinderLayer2X[nPrimaryParticle]/D");
  evt->Branch("ProngEInMuonFinderLayer2Y" , ProngEInMuonFinderLayer2Y  , "ProngEInMuonFinderLayer2Y[nPrimaryParticle]/D");
  evt->Branch("ProngAngleToBeamDir"       , ProngAngleToBeamDir        , "ProngAngleToBeamDir[nPrimaryParticle]/D");
  evt->Branch("ShowerLength"              , ShowerLength               , "ShowerLength[nPrimaryParticle]/D");
  evt->Branch("ShowerLengthInLAr"         , ShowerLengthInLAr          , "ShowerLengthInLAr[nPrimaryParticle]/D");
  evt->Branch("ShowerWidth"               , ShowerWidth                , "ShowerWidth[nPrimaryParticle]/D");
  evt->Branch("ShowerWidthInLAr"          , ShowerWidthInLAr           , "ShowerWidthInLAr[nPrimaryParticle]/D");
  evt->Branch("ProngAvgdEdx"              , ProngAvgdEdx               , "ProngAvgdEdx[nPrimaryParticle]/D");
  evt->Branch("ProngAvgdEdxInLAr"         , ProngAvgdEdxInLAr          , "ProngAvgdEdxInLAr[nPrimaryParticle]/D");
  evt->Branch("ProngdEdxAlongTrack"       , ProngdEdxAlongTrack        , "ProngdEdxAlongTrack[nPrimaryParticle][100]/D");
  evt->Branch("ProngdEdxTrackLength"      , ProngdEdxTrackLength       , "ProngdEdxTrackLength[nPrimaryParticle][100]/I");
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
    evt->Branch("trkNhits"                , &trkNhits                  , "trkNhits/I");
    evt->Branch("circXc"                  , &xc                        , "circXc/D"); 
    evt->Branch("circZc"                  , &zc                        , "circZc/D");
    evt->Branch("circRc"                  , &rc                        , "circRc/D");
    evt->Branch("circp0"                  , &p0                        , "circp0/D"); 
    evt->Branch("circp1"                  , &p1                        , "circp1/D");
    evt->Branch("circcosDip"              , &cosDip                    , "circcosDip/D");
    evt->Branch("Nmagnets"                , &Nmagnets                  , "Nmagnets/I");
    evt->Branch("trkp0"                   , &trkp0                     , "trkp0/D"); 
    evt->Branch("trkp1"                   , &trkp1                     , "trkp1/D");
    evt->Branch("trkcosDip"               , &trkcosDip                 , "trkcosDip/D");
    evt->Branch("magnetZs"                , &magzpos);
    evt->Branch("trkXc"                   , &trkxc); 
    evt->Branch("trkZc"                   , &trkzc);
    evt->Branch("trkRc"                   , &trkrc);
    evt->Branch("trkqIn"                  , &trkqIn);
    evt->Branch("trkmIn"                  , &trkmIn);
    evt->Branch("trkqOut"                 , &trkqOut);
    evt->Branch("trkmOut"                 , &trkmOut);
    evt->Branch("circHitXFSL"             , &hitXFSL);
    evt->Branch("circHitYFSL"             , &hitYFSL);
    evt->Branch("circHitZFSL"             , &hitZFSL);        
    evt->Branch("circHitPFSL"             , &hitPFSL);        
    evt->Branch("trkHitXFSL"              , &trkXFSL);
    evt->Branch("trkHitYFSL"              , &trkYFSL);
    evt->Branch("trkHitZFSL"              , &trkZFSL);        
    evt->Branch("trkHitPFSL"              , &trkPFSL);        
  }

}

void AnalysisManager::bookTrkTree() {

  trk = new TTree("trk", "trkTreeInfo");
  trk->Branch("evtID", &evtID, "evtID/I");
  trk->Branch("trackTID", &trackTID, "trackTID/I");                     
  trk->Branch("trackPID", &trackPID, "trackPID/I");                     
  trk->Branch("trackPDG", &trackPDG, "trackPDG/I");
  trk->Branch("trackKinE", &trackKinE, "trackKinE/D");
  trk->Branch("trackNPoints", &trackNPoints, "trackNPoints/I");  
  trk->Branch("trackPointX", &trackPointX);  
  trk->Branch("trackPointY", &trackPointY);  
  trk->Branch("trackPointZ", &trackPointZ);

}

void AnalysisManager::BeginOfRun() {
  
  G4cout<<"TTree is booked and the run has been started"<<G4endl;
  if (thefile) {
    delete thefile;
  }
  thefile = new TFile(m_filename.c_str(), "RECREATE");
  bookEvtTree();
  if(m_saveTrack) bookTrkTree();

  fH5Filename = m_filename;
  if(fH5Filename.find(".root") != std::string::npos) {
    const size_t pos = fH5Filename.find(".root");
    fH5Filename.resize(pos);
  }
  fH5Filename += ".h5";

  fH5file = hep_hpc::hdf5::File(fH5Filename, H5F_ACC_TRUNC);

  SDNamelist = GeometricalParameters::Get()->GetSDNamelist();
  NumberOfSDs = SDNamelist.size();
  G4cout<<"Number of SDs : "<<NumberOfSDs<<G4endl;
  for (auto sdname : SDNamelist) 
    G4cout<<sdname.first<<" "<<sdname.second<<G4endl;
}

void AnalysisManager::EndOfRun() {
  thefile->cd();
  evt->Write();
  if(m_saveTrack) trk->Write();
  thefile->Close();
  fH5file.close();
}

void AnalysisManager::BeginOfEvent() {
  neutrino = FPFNeutrino();
  primaries.clear();
  primaryIDs.clear();
  nHits                        = 0;
  sparseFractionMem            = -1;
  sparseFractionBins           = -1;
  edepInLAr                    = 0;
  edepInHadCalX                = 0;
  edepInHadCalY                = 0;
  edepInMuonFinderX            = 0;
  edepInMuonFinderY            = 0;
  edepInHadAborb               = 0;
  edepInMuonFinderAbsorb       = 0;
  missCountedEnergy            = 0;
  nPrimaryParticle             = 0;
  nTestNPrimaryTrack           = 0;
  nFromFSLParticles            = 0;
  nFromFSPizeroParticles       = 0;
  nFromFSLDecayPizeroParticles = 0;
  countPrimaryParticle         = 0;
  for (G4int j= 0; j< 3000; ++j) {
    TotalDedxLongitudinal[j] = 0;
    TrueTotalDedxLongitudinal[j] = 0;
  }
  for (G4int i= 0; i< 1000; ++i) {
    primaryParentPDG[i]          = 0;
    primaryTrackLength[i]        = 0;
    primaryTrackLengthInTPC[i]   = 0;
    ProngEInDetector[i]          = 0;
    ProngEInLAr[i]               = 0;
    ProngEInHadCal[i]            = 0;
    ProngEInMuonFinder[i]        = 0;
    ProngEInMuonFinderLayer1X[i] = 0;
    ProngEInMuonFinderLayer1Y[i] = 0;
    ProngEInMuonFinderLayer2X[i] = 0;
    ProngEInMuonFinderLayer2Y[i] = 0;
    ProngAngleToBeamDir[i]       = -1;
    ShowerLength[i]              = -1;
    ShowerLengthInLAr[i]         = -1;
    ShowerWidth[i]               = 0;
    ShowerWidthInLAr[i]          = 0;
    ProngAvgdEdx[i]              = -1;
    ProngAvgdEdxInLAr[i]         = -1;
    for (G4int j= 0; j< 100; ++j) {
      ProngdEdxAlongTrack[i][j] = 0;
      ProngdEdxTrackLength[i][j] = -1;
    }
    dir_pol_x[i]                 = -999;
    dir_pol_y[i]                 = -999;
    dir_pol_z[i]                 = -999;
    dir_coc_x[i]                 = -999;
    dir_coc_y[i]                 = -999;
    dir_coc_z[i]                 = -999;
    fromFSLParticlePDG[i]        = 0;
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

  magzpos.clear();
  trkxc.clear(); 
  trkzc.clear();
  trkrc.clear();
  trkqIn.clear();
  trkmIn.clear();
  trkqOut.clear();
  trkmOut.clear();

  hitXFSL.clear();
  hitYFSL.clear();
  hitZFSL.clear();
  hitPFSL.clear();
  trkXFSL.clear();
  trkYFSL.clear();
  trkZFSL.clear();
  trkPFSL.clear();

  trackTID = -1;                     
  trackPID = -1;     
  trackPDG = -1;
  trackKinE = -1;
  trackNPoints = -1;  
  trackPointX.clear();  
  trackPointY.clear();  
  trackPointZ.clear();

}

void AnalysisManager::EndOfEvent(const G4Event* event) {
  /// evtID
  evtID = event->GetEventID();

  /// loop over the vertices, and then over primary particles,
  /// neutrino truth info from event generator.
  for (G4int ivtx = 0; ivtx < event->GetNumberOfPrimaryVertex(); ++ivtx) {
    for (G4int ipp = 0; ipp < event->GetPrimaryVertex(ivtx)->GetNumberOfParticle(); ++ipp) {
      G4PrimaryParticle* primary_particle = 
        event->GetPrimaryVertex(ivtx)->GetPrimary(ipp);
      if (primary_particle) {
        PrimaryParticleInformation* primary_particle_info = 
          dynamic_cast<PrimaryParticleInformation*>(primary_particle->GetUserInformation());
        primary_particle_info->Print();

	// add to list of primary particles
	// this list is then appended further if necessary
        countPrimaryParticle++;
	primaryIDs.push_back(primary_particle_info->GetPartID()); //store to avoid duplicates
        primaries.push_back(FPFParticle(primary_particle_info->GetPDG(), 0, 
		primary_particle_info->GetPartID(),
	       	countPrimaryParticle-1, 1,
		primary_particle_info->GetMass(),
                primary_particle_info->GetVertexMC().x(),
                primary_particle_info->GetVertexMC().y(),
                primary_particle_info->GetVertexMC().z(),
		0.,
                primary_particle_info->GetMomentumMC().x(), 
		primary_particle_info->GetMomentumMC().y(),
	        primary_particle_info->GetMomentumMC().z(), 
                GetTotalEnergy(primary_particle_info->GetMomentumMC().x(),primary_particle_info->GetMomentumMC().y(),
                  primary_particle_info->GetMomentumMC().z(), primary_particle_info->GetMass())
		));

	// if it's the first, also store the nuetrino info (if available)
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
          neutrino = FPFNeutrino(nuPDG, nuX, nuY, nuZ, 0, 0, 0, 0, nuE, 
              nuIdx, nuIntType, nuScatteringType, nuW, nuFSLPDG, nuFSLPx, nuFSLPy, nuFSLPz, nuFSLE);
          continue;
        }
      }
    }
  }
  nPrimaryVertex   = event->GetNumberOfPrimaryVertex();
  std::cout<<"\nnumber of primary vertices  : "<<nPrimaryVertex<<std::endl;
  std::cout<<"=============>"<<neutrino.NuPDG()<<" "<<neutrino.NuFSLPDG()<<std::endl;
  
  // Get the hit collections
  // If there is no hit collection, there is nothing to be done
  hcofEvent = event->GetHCofThisEvent();
  if (!hcofEvent) return;

  // loop over all sensitive detectors to get all the hit collections
  // fill the truth tree for primary particles
  for (auto sdname : SDNamelist) {
    std::cout << "FillPrimaryTruthTree(" << sdname.first << ", " << sdname.second << ")" << std::endl;
    FillPrimaryTruthTree(sdname.first, sdname.second);
  }
  // update number of primary particles after FillPrimaryTruthTree
  // including decay products from primary tau and pizero
  nPrimaryParticle = countPrimaryParticle; // TODO: they're equivalent, should only store one of them
  nFromFSLParticles = tracksFromFSLSecondary.size();
  nFromFSPizeroParticles = tracksFromFSPizeroSecondary.size();
  nFromFSLDecayPizeroParticles = tracksFromFSLDecayPizeroSecondary.size();

  // find all the tracks originate from the final state lepton, include FSL itself (TID=1)
  // should only work with neutrino interaction generator
  // exception to single particle generator: tau, mu
  if (neutrino.NuPDG()!=0 || abs(primaries[0].PDG())==15 || abs(primaries[0].PDG())==13) {
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
  //std::cout<<"Test nTestNPrimaryTrack : "<<nTestNPrimaryTrack<<std::endl;

  //- cluster all tracks to primary particles
  //- mark the index of the final state lepton from the neutrino interaction
  trackClusters.resize(nPrimaryParticle);
  for (int iPrim= 0; iPrim< nPrimaryParticle; ++iPrim) {
    trackClusters[iPrim].insert(primaries[iPrim].TID());
    if (primaries[iPrim].TID()==1 && neutrino.NuPDG()!=0) {
      fPrimIdxFSL = iPrim;
    }
  }
  if (fPrimIdxFSL>=0) primaries[fPrimIdxFSL].SetProngType(0);
  
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

  const Double_t res_tpc[3] = {1, 5, 5}; // mm
  if (neutrino.NuPDG()!=0) {
    pm3D = new PixelMap3D(evtID, nPrimaryParticle, neutrino.NuPDG(), res_tpc);
  } else {
    pm3D = new PixelMap3D(evtID, nPrimaryParticle, primaries[0].PDG(), res_tpc);
  }
  // boundary in global coord.
  pm3D->SetPMBoundary(GeometricalParameters::Get()->GetFLArEPosition()/mm -
                        GeometricalParameters::Get()->GetTPCSizeXYZ()/mm/2,
                        GeometricalParameters::Get()->GetFLArEPosition()/mm +
                        GeometricalParameters::Get()->GetTPCSizeXYZ()/mm/2);
  pm3D->InitializePM();

  /// FillTrueEdep must run after FillPrimaryTruthTree, 
  /// otherwise tracksFromFSL and tracksFromFSLSecondary are invalid
  /// Pixel map is also filled here
  for (auto sdname : SDNamelist) {
    FillTrueEdep(sdname.first, sdname.second);
  }

  if (m_save2DEvd) pm3D->Write2DPMToFile(thefile);

  pm3D->Process3DPM(fH5file, neutrino, m_save3DEvd);
  sparseFractionMem = pm3D->GetSparseFractionMem();
  sparseFractionBins = pm3D->GetSparseFractionBins();

  if (m_circularFit){
    
    circNhits = hitXFSL.size();
    trkNhits = trkXFSL.size();
    
    // apply circular fitting to FLArE hadCath/muonFinder
    if ( circNhits > 0 ){
      circularfitter::CircleFit* circFit = new circularfitter::CircleFit(hitXFSL,hitZFSL);
      circularfitter::LineFit* lineFit = new circularfitter::LineFit(hitZFSL,hitYFSL);
      xc = circFit->GetXc();
      zc = circFit->GetZc();
      rc = circFit->GetR();
      p0 = lineFit->GetP0();
      p1 = lineFit->GetP1();
      cosDip = lineFit->GetCosDip();
    }

    // apply circular fitting for FASER2 spectrometer magnet
    if( trkNhits > 0 ){
      
      Nmagnets = (GeometricalParameters::Get()->GetFASER2MagnetOption() == GeometricalParameters::magnetOption::SAMURAI) ? 1 :
                  GeometricalParameters::Get()->GetNFASER2Magnets();
      G4cout << "Number of FASER2 magnets: " << Nmagnets << G4endl;

      circularfitter::CircleExtractor* circExtract = new circularfitter::CircleExtractor(trkXFSL,trkYFSL,trkZFSL);
      magzpos = circExtract->GetMagnetZs();
      trkxc = circExtract->GetXc();
      trkzc = circExtract->GetZc();
      trkrc = circExtract->GetR();
      
      std::vector<circularfitter::line> pre  = circExtract->GetPreLine();
      std::vector<circularfitter::line> post = circExtract->GetPostLine();
      for(int i=0; i<Nmagnets; i++){
        //G4cout << "circle: " << trkzc[i] << " " << trkxc[i] << " " << trkrc[i] << G4endl;
        trkqIn.push_back( pre[i].q );
        trkmIn.push_back( pre[i].m );
        trkqOut.push_back( post[i].q );
        trkmOut.push_back( post[i].m );
      }

      circularfitter::LineFit* lineFit2 = new circularfitter::LineFit(trkZFSL,trkYFSL);
      trkp0 = lineFit2->GetP0();
      trkp1 = lineFit2->GetP1();
      trkcosDip = lineFit2->GetCosDip();
    }
  }
  
  // FillPseudoRecoVar must run after FillTrueEdep, otherwise some of the variables won't be filled
  FillPseudoRecoVar();

  int count_tracks = 0;
  if(m_saveTrack){
    G4cout << "---> Saving track information to tree..." << G4endl;
    auto trajectoryContainer = event->GetTrajectoryContainer();
    if (trajectoryContainer) {
      for (size_t i = 0; i < trajectoryContainer->entries(); ++i) {
        auto trajectory = static_cast<G4Trajectory*>((*trajectoryContainer)[i]);
        trackTID = trajectory->GetTrackID();
        trackPID = trajectory->GetParentID();
        trackPDG = trajectory->GetPDGEncoding();
	trackKinE = trajectory->GetInitialKineticEnergy();
        trackNPoints = trajectory->GetPointEntries();
        count_tracks++;
	for (size_t j = 0; j < trackNPoints; ++j) {
          G4ThreeVector pos = trajectory->GetPoint(j)->GetPosition();
	  trackPointX.push_back( pos.x() );
	  trackPointY.push_back( pos.y() );
	  trackPointZ.push_back( pos.z() );
        }
        trk->Fill();
	trackPointX.clear();
	trackPointY.clear();
	trackPointZ.clear();
      }
    } else G4cout << "No tracks found: did you enable their storage with '/tracking/storeTrajectory 1'?" << G4endl;
    G4cout << "---> Done!" << G4endl;
  }
  
  evt->Fill();

  G4cout << "Total number of recorded hits : " << nHits << std::endl;
  if(m_saveTrack) G4cout << "Total number of recorded track: " << count_tracks << std::endl;

  for (int iPrim= 0; iPrim< nPrimaryParticle; ++iPrim) {
    trackClusters[iPrim].clear();
  }
  trackClusters.clear();
  trackClusters.shrink_to_fit();

  delete pm3D;
}

void AnalysisManager::FillPrimaryTruthTree(G4int sdId, std::string sdName) 
{
  // Get and cast hit collection with LArBoxHits
  LArBoxHitsCollection* hitCollection = dynamic_cast<LArBoxHitsCollection*>(hcofEvent->GetHC(sdId));

  std::cout << "hitCollection->size() = " << hitCollection->GetSize() << std::endl;

  if (hitCollection->GetSize() == 0) return;

  if (hitCollection && hitCollection->GetSize() != 0) {
    
    std::cout << "hitCollection->size()" << hitCollection->GetSize() << std::endl;

    for (auto hit: *hitCollection->GetVector()) {
      std::cout << "HERE" << std::endl;
      nHits++;

      double pre_x  = hit->GetPreStepPosition().x();
      double pre_y  = hit->GetPreStepPosition().y();
      double pre_z  = hit->GetPreStepPosition().z();
      double post_x = hit->GetPostStepPosition().x();
      double post_y = hit->GetPostStepPosition().y();
      double post_z = hit->GetPostStepPosition().z();
      double px = hit->GetInitMomentum().x();
      double pz = hit->GetInitMomentum().z();
      double p_perp = TMath::Sqrt(px*px+pz*pz);

      trkXFSL.push_back(post_x);
      trkYFSL.push_back(post_y);
      trkZFSL.push_back(post_z);
      trkPFSL.push_back(p_perp);
      
      std::cout << "Hit(" << post_x << ", " << post_y << ", " << post_z << ")" << std::endl; 
      

      // if (m_saveHit) {
      //   if (nHits<=40000000) {
      //     HitTID[nHits-1] = hit->GetTID();
      //     HitPID[nHits-1] = hit->GetPID();
      //     HitPDG[nHits-1] = hit->GetParticle();
      //     HitTrackStatus[nHits-1]  = hit->GetTrackStatus();
      //     HitPrePositionX[nHits-1] = pre_x;
      //     HitPrePositionY[nHits-1] = pre_y;
      //     HitPrePositionZ[nHits-1] = pre_z;
      //     HitPosPositionX[nHits-1] = post_x;
      //     HitPosPositionY[nHits-1] = post_y;
      //     HitPosPositionZ[nHits-1] = post_z;
      //     HitEdep[nHits-1] = hit->GetEdep();
      //   }
      // }
      
      // // energy deposition in different volumes of the detector
      // if (sdName == "lArBoxSD/lar_box") 
      //   edepInLAr += hit->GetEdep();
      // else if (sdName == "HadCalXSD/lar_box") 
      //   edepInHadCalX += hit->GetEdep();
      // else if (sdName == "HadCalYSD/lar_box") 
      //   edepInHadCalY += hit->GetEdep();
      // else if (sdName == "MuonFinderXSD/lar_box") 
      //   edepInMuonFinderX += hit->GetEdep();
      // else if (sdName == "MuonFinderYSD/lar_box") 
      //   edepInMuonFinderY += hit->GetEdep();
      // else if (sdName == "HadAbsorbSD/lar_box") 
      //   edepInHadAborb += hit->GetEdep();
      // else if (sdName == "MuonFinderAbsorbSD/lar_box") 
      //   edepInMuonFinderAbsorb += hit->GetEdep();

      // // save FSL (only muons!) hits for circle fitting
      // // check that particle is a primary muon
      
      // double px = hit->GetInitMomentum().x();
      // double pz = hit->GetInitMomentum().z();
      // double p_perp = TMath::Sqrt(px*px+pz*pz);
      // // save hits in FLArE hadCather+muonFinder
      // if( (sdName == "HadCalXSD/lar_box") || (sdName == "HadCalYSD/lar_box") ||
      //     (sdName == "MuonFinderXSD/lar_box") || (sdName == "MuonFinderYSD/lar_box") ||
      //     (sdName == "HadAbsorbSD/lar_box") || (sdName == "MuonFinderAbsorbSD/lar_box") ||
      //     (sdName == "BabyMINDHorBarSD/lar_box") || (sdName == "BabyMINDVerBarSD/lar_box")){
      //   hitXFSL.push_back(post_x);
      //   hitYFSL.push_back(post_y);
      //   hitZFSL.push_back(post_z);
      //   hitPFSL.push_back(p_perp);
      // }
      // else if (sdName == "FASER2TrackerSD/lar_box"){
      //   std::cout << "Pushing back hits in FASER2 Tracker" << std::endl;
      //   trkXFSL.push_back(post_x);
      //   trkYFSL.push_back(post_y);
      //   trkZFSL.push_back(post_z);
      //   trkPFSL.push_back(p_perp);
      // }
      

      //allTracksPTPair.insert(std::make_pair(hit->GetPID(), hit->GetTID()));

      // stable final state particles in GENIE, primary particles in Geant4
    //   if (hit->GetCreatorProcess()=="PrimaryParticle") { // i.e. PID==0
    //     if ( std::find(primaryIDs.begin(), primaryIDs.end(), hit->GetTID()) == primaryIDs.end() ) { 
    //       // the following line excludes final state lepton tau from the primary particle list
    //       //if (abs(nuPDG)==16 && abs(nuFSLPDG)==15 && abs(hit->GetParticle()==15)) continue;
    //       countPrimaryParticle++;
	  // primaryIDs.push_back(hit->GetTID());
    //       primaries.push_back(FPFParticle(hit->GetParticle(), 
    //             hit->GetPID(), hit->GetTID(), countPrimaryParticle-1, 1, hit->GetParticleMass(),
    //             hit->GetTrackVertex().x(), hit->GetTrackVertex().y(), hit->GetTrackVertex().z(), 0, 
    //             hit->GetInitMomentum().x(), hit->GetInitMomentum().y(), hit->GetInitMomentum().z(), 
    //             GetTotalEnergy(hit->GetInitMomentum().x(), hit->GetInitMomentum().y(),
    //               hit->GetInitMomentum().z(), hit->GetParticleMass())));
    //     }
    //   }
    //   // in case of the fsl decay, the decay products are counted as primary particles
    //   // * tau- decay (dominant)
    //   // * mu- decay
    //   //if (hit->GetPID()==1 && hit->GetCreatorProcess()=="Decay") {
    //   if (hit->GetIsTrackFromPrimaryLepton()) {
    //     tracksFromFSLSecondary.insert(hit->GetTID());
    //     if (std::find(primaryIDs.begin(), primaryIDs.end(), hit->GetTID()) == primaryIDs.end()) {
    //       countPrimaryParticle++;
	  // primaryIDs.push_back(hit->GetTID());
    //       primaries.push_back(FPFParticle(hit->GetParticle(), 
    //             hit->GetPID(), hit->GetTID(), countPrimaryParticle-1, 2, hit->GetParticleMass(),
    //             hit->GetTrackVertex().x(), hit->GetTrackVertex().y(), hit->GetTrackVertex().z(), 0, 
    //             hit->GetInitMomentum().x(), hit->GetInitMomentum().y(), hit->GetInitMomentum().z(), 
    //             GetTotalEnergy(hit->GetInitMomentum().x(), hit->GetInitMomentum().y(),
    //               hit->GetInitMomentum().z(), hit->GetParticleMass())));
    //     }
    //   }
    //   // in case of pizero in the list of primary track
    //   // its decay products are also counted as primary particles, mostly 2 gammas
    //   if (hit->GetIsTrackFromPrimaryPizero()) {
    //     tracksFromFSPizeroSecondary.insert(hit->GetTID());
    //     if (std::find(primaryIDs.begin(), primaryIDs.end(), hit->GetTID()) == primaryIDs.end()) {
    //       countPrimaryParticle++;
	  // primaryIDs.push_back(hit->GetTID());
    //       primaries.push_back(FPFParticle(hit->GetParticle(), 
    //             hit->GetPID(), hit->GetTID(), countPrimaryParticle-1, 3, hit->GetParticleMass(),
    //             hit->GetTrackVertex().x(), hit->GetTrackVertex().y(), hit->GetTrackVertex().z(), 0, 
    //             hit->GetInitMomentum().x(), hit->GetInitMomentum().y(), hit->GetInitMomentum().z(), 
    //             GetTotalEnergy(hit->GetInitMomentum().x(), hit->GetInitMomentum().y(),
    //               hit->GetInitMomentum().z(), hit->GetParticleMass())));
    //     }
    //   }
    //   // in case of tau decay pizero
    //   // decay products of this pizero are also counted as primary particles, mostly 2 gammas
    //   if (hit->GetIsTrackFromFSLPizero()) {
    //     tracksFromFSLDecayPizeroSecondary.insert(hit->GetTID());
    //     if (std::find(primaryIDs.begin(), primaryIDs.end(), hit->GetTID()) == primaryIDs.end()) {
    //       countPrimaryParticle++;
	  // primaryIDs.push_back(hit->GetTID());
    //       primaries.push_back(FPFParticle(hit->GetParticle(), 
    //             hit->GetPID(), hit->GetTID(), countPrimaryParticle-1, 4, hit->GetParticleMass(),
    //             hit->GetTrackVertex().x(), hit->GetTrackVertex().y(), hit->GetTrackVertex().z(), 0, 
    //             hit->GetInitMomentum().x(), hit->GetInitMomentum().y(), hit->GetInitMomentum().z(), 
    //             GetTotalEnergy(hit->GetInitMomentum().x(), hit->GetInitMomentum().y(),
    //               hit->GetInitMomentum().z(), hit->GetParticleMass())));
    //     }
    //   }
    } // end of hit loop
  }
}

void AnalysisManager::FillTrueEdep(G4int sdId, std::string sdName) 
{
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
          G4cout<<"TID : "<<hit->GetTID()     <<", PID : "           <<hit->GetPID()
            <<", PDG : "<<hit->GetParticle()     <<", CreatorProcess : "<<hit->GetCreatorProcess()
            <<", Ek : " <<hit->GetInitKinEnergy()<<" MeV"              <<G4endl;
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
        if (sdName == "lArBoxSD/lar_box") {
          std::cout<<"Can't find the primary particle of the hit in TPC volume, something is wrong "
            <<hit->GetParticle()<<" edep("<<hit->GetEdep()<<") TID("<<hit->GetTID()<<") PID("
            <<hit->GetPID()<<") creator process ("<<hit->GetCreatorProcess()<<") position"
            <<hit->GetEdepPosition()<<std::endl; 
          missCountedEnergy += hit->GetEdep();
          continue;
        }
      } 

      double pos_x = hit->GetEdepPosition().x();
      double pos_y = hit->GetEdepPosition().y();
      double pos_z = hit->GetEdepPosition().z();
      double hit_position_xyz[3] = {pos_x, pos_y, pos_z};
      double vtx_xyz[3];
      if (neutrino.NuPDG()!=0) {
        vtx_xyz[0] = neutrino.NuVx(); 
        vtx_xyz[1] = neutrino.NuVy(); 
        vtx_xyz[2] = neutrino.NuVz();
      }
      else {
        vtx_xyz[0] = primaries[0].Vx(); 
        vtx_xyz[1] = primaries[0].Vy();
        vtx_xyz[2] = primaries[0].Vz();
      }

      if ((sdName == "lArBoxSD/lar_box") && (m_addDiffusion == "toy")) {
        pm3D->FillEntryWithToyElectronTransportation(hit_position_xyz, vtx_xyz, hit->GetEdep(), whichPrim);
      } else if ((sdName == "lArBoxSD/lar_box") && (m_addDiffusion == "single")) {
        pm3D->FillEntryWithToySingleElectronTransportation(hit_position_xyz, vtx_xyz, hit->GetEdep(), whichPrim);
      } else if (sdName == "lArBoxSD/lar_box") {
        pm3D->FillEntry(hit_position_xyz, vtx_xyz, hit->GetEdep(), whichPrim);
      }

      if (sdName == "lArBoxSD/lar_box") {
        double longitudinal_distance_to_vtx;  // in mm
        if (neutrino.NuPDG()!=0) {
          longitudinal_distance_to_vtx = (pos_z-vtx_xyz[2]);
        } else {
          longitudinal_distance_to_vtx = ((pos_x-vtx_xyz[0])*primaries[0].Px()+
                                          (pos_y-vtx_xyz[1])*primaries[0].Py()+
                                          (pos_z-vtx_xyz[2])*primaries[0].Pz())/primaries[0].P();
        }
        if (Int_t(longitudinal_distance_to_vtx)>=0 && Int_t(longitudinal_distance_to_vtx)<3000) {  // within 3000 mm
          TrueTotalDedxLongitudinal[Int_t(longitudinal_distance_to_vtx)] += hit->GetEdep();
        }
      }
      // calculate dEdx along the track
      // combine the tracks if they come from the final state lepton, namely tau-
      double ShowerP = primaries[whichPrim].P();
      if ((hit->GetPID()==0) |
          (tracksFromFSLSecondary.find(hit->GetTID()) != tracksFromFSLSecondary.end()) |
          (tracksFromFSPizeroSecondary.find(hit->GetTID()) != tracksFromFSPizeroSecondary.end()) |
          (tracksFromFSLDecayPizeroSecondary.find(hit->GetTID()) != tracksFromFSLDecayPizeroSecondary.end())) {
        primaryTrackLength[whichPrim] += hit->GetStepLength();
        if (sdName == "lArBoxSD/lar_box") {
          primaryTrackLengthInTPC[whichPrim] += hit->GetStepLength();
          if ((hit->GetPID()==0) |
              (tracksFromFSPizeroSecondary.find(hit->GetTID()) != tracksFromFSPizeroSecondary.end())) {
            double longitudinal_distance_to_vtx = ((pos_x-primaries[whichPrim].Vx())*primaries[whichPrim].Px()+
                                                   (pos_y-primaries[whichPrim].Vy())*primaries[whichPrim].Py()+
                                                   (pos_z-primaries[whichPrim].Vz())*primaries[whichPrim].Pz())/ShowerP;
            if (int(longitudinal_distance_to_vtx)>=0 && int(longitudinal_distance_to_vtx)<100) {  // within 100 mm
              ProngdEdxAlongTrack[whichPrim][int(longitudinal_distance_to_vtx)] += hit->GetEdep();
              ProngdEdxTrackLength[whichPrim][int(longitudinal_distance_to_vtx)] = int(longitudinal_distance_to_vtx);
            }
          } else {
            if (fPrimIdxFSL>=0) {
              double ShowerP_FSL = primaries[fPrimIdxFSL].P();
              double longitudinal_distance_to_vtx = ((pos_x-primaries[fPrimIdxFSL].Vx())*primaries[fPrimIdxFSL].Px()+
                                                     (pos_y-primaries[fPrimIdxFSL].Vy())*primaries[fPrimIdxFSL].Py()+
                                                     (pos_z-primaries[fPrimIdxFSL].Vz())*primaries[fPrimIdxFSL].Pz())/ShowerP_FSL;
              if (int(longitudinal_distance_to_vtx)>=0 && int(longitudinal_distance_to_vtx)<100) {  // within 100 mm
                ProngdEdxAlongTrack[fPrimIdxFSL][int(longitudinal_distance_to_vtx)] += hit->GetEdep();
                ProngdEdxTrackLength[fPrimIdxFSL][int(longitudinal_distance_to_vtx)] = int(longitudinal_distance_to_vtx);
              }
            }
          }
        }
      }
      // calculate the shower/track length and width
      // length: defined as the longest projection distance at the true direction between vertex and hits
      //         length = |\vector{hit_position}|\cdot\cos(theta) = \vertor{hit_position}\cdot\vector{P} / |\vector{P}|
      // width: defined as the weighted average of the least distance of the hits to the true direction
      double dsquare_hit_vtx = TMath::Power((pos_x-primaries[whichPrim].Vx()),2)+
                               TMath::Power((pos_y-primaries[whichPrim].Vy()),2)+
                               TMath::Power((pos_z-primaries[whichPrim].Vz()),2);
      double product_hit_p = (pos_x-primaries[whichPrim].Vx())*primaries[whichPrim].Px()+
                             (pos_y-primaries[whichPrim].Vy())*primaries[whichPrim].Py()+
                             (pos_z-primaries[whichPrim].Vz())*primaries[whichPrim].Pz();
      double len_hit = TMath::Abs(product_hit_p)/ShowerP;
      double width_hit = TMath::Sqrt((dsquare_hit_vtx - product_hit_p*product_hit_p/ShowerP/ShowerP));
      // exclude zero hit when calculating showerlength of the primary particle
      // exclude hits from the cryo gap (detID=8)
      if (hit->GetEdep()>0 && 
          (sdName=="lArBoxSD/lar_box" || sdName=="HadAbsorbSD/lar_box" || sdName=="MuonFinderAbsorbSD/lar_box")) {
        ProngEInDetector[whichPrim] += hit->GetEdep();
        ShowerLength[whichPrim] = std::max({ShowerLength[whichPrim], len_hit});
        //double square_weighted_width_hit = TMath::Power(width_hit*hit->GetEdep(),2);
        double weighted_width_hit = width_hit*hit->GetEdep();
        if (!std::isnan(weighted_width_hit)) ShowerWidth[whichPrim] += weighted_width_hit;
        if (sdName=="lArBoxSD/lar_box") {
          ProngEInLAr[whichPrim] += hit->GetEdep();
          ShowerLengthInLAr[whichPrim] = std::max({ShowerLengthInLAr[whichPrim], len_hit});
          if (!std::isnan(weighted_width_hit)) ShowerWidthInLAr[whichPrim] += weighted_width_hit;
        }
      }
      if ((sdName=="HadCalXSD/lar_box") || 
          (sdName=="HadCalYSD/lar_box") || 
          (sdName=="HadAbsorbSD/lar_box")) {
        ProngEInHadCal[whichPrim] += hit->GetEdep();
      }
      if ((sdName=="MuonFinderXSD/lar_box") || 
          (sdName=="MuonFinderYSD/lar_box") || 
          (sdName=="MuonFinderAbsorbSD/lar_box")) {
        ProngEInMuonFinder[whichPrim] += hit->GetEdep();
        if (sdName=="MuonFinderXSD/lar_box") {
          if (pos_z < GeometricalParameters::Get()->GetFLArEPosition().z()/mm+
              GeometricalParameters::Get()->GetTPCInsulationThickness()/mm+
              GeometricalParameters::Get()->GetHadCalLength()/mm+
              GeometricalParameters::Get()->GetMuonCatcherLength()/mm/2) {
            ProngEInMuonFinderLayer1X[whichPrim] += hit->GetEdep();
          } else {
            ProngEInMuonFinderLayer2X[whichPrim] += hit->GetEdep();
          }
        } else if (sdName=="MuonFinderYSD/lar_box") {
          if (pos_z < GeometricalParameters::Get()->GetFLArEPosition().z()/mm+
              GeometricalParameters::Get()->GetTPCInsulationThickness()/mm+
              GeometricalParameters::Get()->GetHadCalLength()/mm+
              GeometricalParameters::Get()->GetMuonCatcherLength()/mm/2) {
            ProngEInMuonFinderLayer1Y[whichPrim] += hit->GetEdep();
          } else {
            ProngEInMuonFinderLayer2Y[whichPrim] += hit->GetEdep();
          }
        }
      }

    } // end of hit loop
  }
}

double AnalysisManager::GetTotalEnergy(double px, double py, double pz, double m) {
  return TMath::Sqrt(px*px+py*py+pz*pz+m*m);
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
    if (ProngEInDetector[iPrim]>0) {
      ShowerWidth[iPrim] = ShowerWidth[iPrim]/ProngEInDetector[iPrim];
    }
    if (ProngEInLAr[iPrim]>0) {
      ShowerWidthInLAr[iPrim] = ShowerWidthInLAr[iPrim]/ProngEInLAr[iPrim];
    }

    double ShowerP = primaries[iPrim].P();
    double costheta = primaries[iPrim].Pz()/ShowerP;
    ProngAngleToBeamDir[iPrim] = TMath::ACos(costheta);

    ProngAvgdEdx[iPrim] = (ProngEInLAr[iPrim] + 
                           ProngEInHadCal[iPrim] + 
                           ProngEInMuonFinder[iPrim])/ShowerLength[iPrim];
    ProngAvgdEdxInLAr[iPrim] = ProngEInLAr[iPrim]/ShowerLengthInLAr[iPrim];

    std::cout<<std::setiosflags(std::ios::fixed)<<std::setprecision(3);
    std::cout<<std::setw(10)<<primaries[iPrim].PDG();
    std::cout<<std::setw(12)<<ProngAngleToBeamDir[iPrim];
    std::cout<<std::setw(13)<<primaryTrackLength[iPrim];
    std::cout<<std::setw(13)<<ShowerLength[iPrim];
    std::cout<<std::setw(18)<<ShowerWidthInLAr[iPrim];
    std::cout<<std::setw(12)<<ProngEInLAr[iPrim] ;
    std::cout<<std::setw(12)<<ProngEInHadCal[iPrim];
    std::cout<<std::setw(12)<<ProngAvgdEdxInLAr[iPrim];
    std::cout<<std::setw(10)<<primaries[iPrim].ProngType();
    std::cout<<std::setw(12)<<primaries[iPrim].Pz()<<std::endl;
  }

  slid::ShowerLID* shwlid = new slid::ShowerLID(pm3D->Get3DPixelMap(), 
      neutrino.NuVx(), neutrino.NuVy(), neutrino.NuVz(), 0., 0., 1.); 
  Double_t* ptr_dedx = shwlid->GetTotalDedxLongitudinal();
  std::copy(ptr_dedx, ptr_dedx+3000, TotalDedxLongitudinal);

  for(int iPrim= 0; iPrim< nPrimaryParticle; ++iPrim) {
    directionfitter::LinearFit* linFit = new directionfitter::LinearFit(
        pm3D->Get2DPixelMapZX(iPrim+1), 
        pm3D->Get2DPixelMapZY(iPrim+1),
        pm3D->Get2DVtxPixelMapZX(iPrim+1), 
        pm3D->Get2DVtxPixelMapZY(iPrim+1), 
        neutrino.NuVx(), neutrino.NuVy(), neutrino.NuVz(), 
        primaries[iPrim].Vx(), primaries[iPrim].Vy(), primaries[iPrim].Vz());
    dir_pol_x[iPrim] = linFit->GetDir().X();
    dir_pol_y[iPrim] = linFit->GetDir().Y();
    dir_pol_z[iPrim] = linFit->GetDir().Z();
    dir_coc_x[iPrim] = linFit->GetCOCDir().X();
    dir_coc_y[iPrim] = linFit->GetCOCDir().Y();
    dir_coc_z[iPrim] = linFit->GetCOCDir().Z();
    delete linFit;
  }

  delete shwlid;
}
