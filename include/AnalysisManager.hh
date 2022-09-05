#ifndef ANALYSISMANAGER_HH
#define ANALYSISMANAGER_HH

#include <set>
#include "globals.hh"
#include <G4Event.hh>
#include <TFile.h>
#include <TTree.h>
#include <TH2D.h>
#include "AnalysisManagerMessenger.hh"

class AnalysisManager {
  public:
    AnalysisManager();
    ~AnalysisManager();
    static AnalysisManager* GetInstance();
    void bookEvtTree();
    void BeginOfRun();
    void EndOfRun();
    void BeginOfEvent();
    void EndOfEvent(const G4Event* event);

  public:
    void setFileName(G4String val) { m_filename = val; }
    void saveHit(G4bool val) { m_saveHit = val; }
    void saveEvd(G4bool val) { m_saveEvd = val; }

  private:
    static AnalysisManager* instance;
    AnalysisManagerMessenger* messenger;

    G4int    nPrimaryVertex;

    TFile*   thefile;
    TString  m_filename;
    TTree*   evt;
    G4int    evtID;

    // Truth information from genie
    G4int    nuIdx;             ///<- neutrino index (for genie neutrino interaction)
    G4int    nuPDG;             ///<- neutrino PDG code (for genie neutrino interaction)
    G4double nuE;               ///<- neutrino energy
    G4double nuX;               ///<- neutrino vertex X
    G4double nuY;               ///<- neutrino vertex Y
    G4double nuZ;               ///<- neutrino vertex Z
    G4int    nuIntType;         ///<- interaction type: CC, NC, et.al.
    G4int    nuScatteringType;  ///<- scattering type: QE, DIS, RES, MEC, et. al.
    G4int    nuFSLPDG;          ///<- Final state lepton PDG code (for genie neutrino interaction)
    G4double nuFSLPx;           ///<- Final state lepton Px 
    G4double nuFSLPy;           ///<- Final state lepton Py
    G4double nuFSLPz;           ///<- Final state lepton Pz
    G4double nuFSLE;            ///<- Final state lepton total energy (GeV)

    G4int    countPrimaryParticle;
    G4int    nPrimaryParticle;  ///<- number of primary particle 
                                ///   (in case of genie neutrino interaction, number of stable particle in the final state)
                                ///   (in case of the FSL decay, the decay products are counted as primary particle)
    G4double Px[2000000];       ///<- Px of primary particles
    G4double Py[2000000];       ///<- Py of primary particles
    G4double Pz[2000000];       ///<- Pz of primary particles
    G4double Pmass[2000000];    ///<- mass of primary particles
    // Geant4 truth
    G4int    primaryParentID[2000000];         ///<- parent ID of primary particles
    G4int    primaryTrackID[2000000];          ///<- track ID of primary particles
    G4int    primaryTrackPDG[2000000];         ///<- PDG of primary particles, for G4 use
    G4double primaryTrackLength[2000000];      ///<- track length of primary particles
    G4double primaryTrackLengthInTPC[2000000]; ///<- track length of primary particles in TPC region
    G4int    prongType[2000000];
    // pseudo-reco
    G4double EInLAr[2000000];
    G4double EInHadCal[2000000];
    G4double EInMuonFinder[2000000];
    G4double AngleToBeamDir[2000000];
    G4double ShowerLength[2000000];
    G4double ShowerLengthInLAr[2000000];
    G4double dEdx[2000000];
    G4double dEdxInLAr[2000000];

    G4double edepInLAr;
    G4double edepInLArXY2500mm;
    G4double edepInLArXY2000mm;
    G4double edepInLArXY1500mm;
    G4double edepInLArXY1000mm;
    G4double edepInHadCalX;
    G4double edepInHadCalY;
    G4double edepInMuonFinderX;
    G4double edepInMuonFinderY;
    G4double edepInHadAborb;
    G4double edepInMuonFinderAbsorb;
    G4double edepInCryGap;
    G4double missCountedEnergy;

    G4int    nFromFSLParticles;
    //G4int    fromFSLParticleTID[2000000];
    G4int    fromFSLParticlePDG[2000000];
    //G4double fromFSLParticleKinE[2000000];
    //G4double fromFSLParticlePx[2000000];
    //G4double fromFSLParticlePy[2000000];
    //G4double fromFSLParticlePz[2000000];
    //G4double fromFSLTrackLength[2000000];
    //G4double fromFSLTrackLengthInTPC[2000000];

    G4int    nHits;
    G4int    HitTID[40000000];
    G4int    HitPID[40000000];
    G4int    HitPDG[40000000];
    G4int    HitTrackStatus[40000000];
    G4double HitPrePositionX[40000000];
    G4double HitPrePositionY[40000000];
    G4double HitPrePositionZ[40000000];
    G4double HitPosPositionX[40000000];
    G4double HitPosPositionY[40000000];
    G4double HitPosPositionZ[40000000];
    G4double HitEdep[40000000];

    G4bool m_saveHit;
    G4bool m_saveEvd;

    std::vector<TH2D*> hitClusterXY;
    std::vector<TH2D*> hitClusterZX;
    std::vector<TH2D*> hitClusterZY;
//    TH2D* hEdepXY;
//    TH2D* hEdepZX;
//    TH2D* hEdepZY;
//    TH2D* hEdepXYFSL;
//    TH2D* hEdepZXFSL;
//    TH2D* hEdepZYFSL;


  private:
    void FillTree(G4int sdId, std::string sdName);
    void FillTrueEdep(G4int sdId, std::string sdName);

    G4HCofThisEvent* hcofEvent;

    std::set<std::pair<int, int> > allTracksPTPair;
    std::vector<std::set<int> > trackClusters;
    std::set<int> tracksFromFSL;
    std::set<int> tracksFromFSLSecondary;
    std::vector<double> ShowerP;
};

#endif
