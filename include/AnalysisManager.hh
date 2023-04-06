#ifndef ANALYSISMANAGER_HH
#define ANALYSISMANAGER_HH

#include <set>
#include "globals.hh"
#include <G4Event.hh>
#include <TFile.h>
#include <TTree.h>
#include <TH2F.h>
#include "AnalysisManagerMessenger.hh"
//#include <Math/Point3Dfwd.h>
#include <Math/Point3D.h>

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
    // function for controlling from the configuration file
    void setFileName(G4String val) { m_filename = val; }
    void saveHit(G4bool val) { m_saveHit = val; }
    void saveEvd(G4bool val) { m_saveEvd = val; }
    void circleFit(G4bool val) { m_circularFit = val; }
    void addDiffusion(G4String val) { m_addDiffusion = val; }

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
                                ///<- (in case of genie neutrino interaction, number of stable particle in the final state)
                                ///<- (in case of the FSL decay, decay products counted as primary particle)
                                ///<- (in case of the final state pizero, decay products counted as primary particle)
    G4double Px[1000];          ///<- Px of primary particles
    G4double Py[1000];          ///<- Py of primary particles
    G4double Pz[1000];          ///<- Pz of primary particles
    G4double Pmass[1000];       ///<- mass of primary particles
    G4double VtxX[1000];        ///<- Px of primary particles
    G4double VtxY[1000];        ///<- Py of primary particles
    G4double VtxZ[1000];        ///<- Pz of primary particles
    // Geant4 truth
    G4int    primaryParentID[1000];         ///<- parent ID of primary particles
    G4int    primaryParentPDG[1000];        ///<- parent PDG of primary particles
    G4int    primaryTrackID[1000];          ///<- track ID of primary particles
    G4int    primaryTrackPDG[1000];         ///<- PDG of primary particles, for G4 use
    G4double primaryTrackLength[1000];      ///<- track length of primary particles
    G4double primaryTrackLengthInTPC[1000]; ///<- track length of primary particles in TPC region
    G4int    prongIndex[1000];
    G4int    prongType[1000];               ///<- 0: final state lepton
                                            ///<- 1: original primary particle
                                            ///<- 2: decay products of the short lived final state lepton (namely tau-,
                                            ///<-    could also be a michel electron from muon decay)
                                            ///<- 3: decay products of the primary pizero (dominantely 2gamma)
                                            ///<- 4: decay prodcuts of the tau decay pizero
    // pseudo-reco
    G4double EInDetector[1000];
    G4double EInLAr[1000];
    G4double EInHadCal[1000];
    G4double EInMuonFinder[1000];
    G4double EInMuonFinderLayer1X[1000];
    G4double EInMuonFinderLayer1Y[1000];
    G4double EInMuonFinderLayer2X[1000];
    G4double EInMuonFinderLayer2Y[1000];
    G4double AngleToBeamDir[1000];
    G4double ShowerLength[1000];
    G4double ShowerLengthInLAr[1000];
    G4double ShowerWidth[1000];
    G4double ShowerWidthInLAr[1000];
    G4double dEdx[1000];
    G4double dEdxInLAr[1000];
    G4double dEdxAlongTrack[1000][100];
    G4int    dEdxTrackLength[1000][100];
    // reco
    // direction
    G4double dir_pol_x[1000];
    G4double dir_pol_y[1000];
    G4double dir_pol_z[1000];
    G4double dir_coc_x[1000];
    G4double dir_coc_y[1000];
    G4double dir_coc_z[1000];

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
    G4int    nFromFSPizeroParticles;
    G4int    nFromFSLDecayPizeroParticles;
    G4int    fromFSLParticlePDG[2000000];

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
    G4bool m_circularFit;
    TString m_addDiffusion;

    //std::vector<TH2F*> hitClusterXY;
    std::vector<TH2F*> hitClusterZX;
    std::vector<TH2F*> hitClusterZY;
    std::vector<TH2F*> vtxHitClusterZX;
    std::vector<TH2F*> vtxHitClusterZY;

    // FSL hits for circular fit
    std::vector<double> hitXFSL;
    std::vector<double> hitZFSL;
    G4int circNhits;
    G4int circStatus;
    G4double xc, zc, rc;
    G4double chi2;

  private:
    void FillPrimaryTruthTree(G4int sdId, std::string sdName);
    void FillTrueEdep(G4int sdId, std::string sdName);
    double GetTotalEnergy(double px, double py, double pz, double m);
    void InitializeEvd();
    void FillPseudoRecoVar();
    void ToyElectronTransportation(int whichPrim, double pos_x, double pos_y, double pos_z, double hitEdep);
    void ToySingleElectronTransportation(int whichPrim, double pos_x, double pos_y, double pos_z, double hitEdep);
    double DistanceToAnode(double x);

    G4HCofThisEvent* hcofEvent;

    std::set<std::pair<int, int> > allTracksPTPair;
    std::vector<std::set<int> > trackClusters;
    std::set<int> tracksFromFSL;
    std::set<int> tracksFromFSLSecondary;
    std::set<int> tracksFromFSPizeroSecondary;
    std::set<int> tracksFromFSLDecayPizeroSecondary;
    int fPrimIdxFSL;
};

#endif
