#ifndef ANALYSISMANAGER_HH
#define ANALYSISMANAGER_HH

#include <set>
#include "globals.hh"
#include <G4Event.hh>
#include <TFile.h>
#include <TTree.h>
#include <TH2F.h>
#include "AnalysisManagerMessenger.hh"
#include "PixelMap3D.hh"
#include "FPFParticle.hh"
#include "FPFNeutrino.hh"

#include "hep_hpc/hdf5/File.hpp"

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
    TFile* GetOutputFile() { return thefile; }
    void SetTrackPTPair(G4int PID, G4int TID) { allTracksPTPair.insert(std::make_pair(PID, TID)); }
    void AddOnePrimaryTrack() { nTestNPrimaryTrack++; }

  public:
    // function for controlling from the configuration file
    void setFileName(std::string val) { m_filename = val; }
    void saveHit(G4bool val) { m_saveHit = val; }
    void save3DEvd(G4bool val) { m_save3DEvd = val; }
    void save2DEvd(G4bool val) { m_save2DEvd = val; }
    void circleFit(G4bool val) { m_circularFit = val; }
    void addDiffusion(G4String val) { m_addDiffusion = val; }

  private:
    static AnalysisManager* instance;
    AnalysisManagerMessenger* messenger;

    TFile*   thefile;
    std::string  m_filename;
    TTree*   evt;
    std::string  fH5Filename;
    hep_hpc::hdf5::File fH5file;

    G4int    evtID;
    FPFNeutrino neutrino;
    std::vector<FPFParticle> primaries;
    // Truth information from genie
    G4int    nuIdx;             ///<- neutrino index (for genie neutrino interaction)
    G4int    nuPDG;             ///<- neutrino PDG code (for genie neutrino interaction)
    G4double nuE;               ///<- neutrino energy
    G4double nuX;               ///<- neutrino vertex X
    G4double nuY;               ///<- neutrino vertex Y
    G4double nuZ;               ///<- neutrino vertex Z
    G4int    nuIntType;         ///<- interaction type: CC, NC, et.al.
    G4int    nuScatteringType;  ///<- scattering type: QE, DIS, RES, MEC, et. al.
    G4double nuW;               ///<- invariant hadronic mass
    G4int    nuFSLPDG;          ///<- Final state lepton PDG code (for genie neutrino interaction)
    G4double nuFSLPx;           ///<- Final state lepton Px 
    G4double nuFSLPy;           ///<- Final state lepton Py
    G4double nuFSLPz;           ///<- Final state lepton Pz
    G4double nuFSLE;            ///<- Final state lepton total energy (GeV)

    G4int    nTestNPrimaryTrack;
    G4int    countPrimaryParticle;
    G4int    nPrimaryVertex;
    G4int    nPrimaryParticle;  ///<- number of primary particle 
                                ///<- (in case of genie neutrino interaction, number of stable particle in the final state)
                                ///<- (in case of the FSL decay, decay products counted as primary particle)
                                ///<- (in case of the final state pizero, decay products counted as primary particle)
    //// Geant4 truth
    G4int    primaryParentPDG[1000];        ///<- parent PDG of primary particles
    G4double primaryTrackLength[1000];      ///<- track length of primary particles
    G4double primaryTrackLengthInTPC[1000]; ///<- track length of primary particles in TPC region

    // pseudo-reco
    G4double ProngEInDetector[1000];
    G4double ProngEInLAr[1000];
    G4double ProngEInHadCal[1000];
    G4double ProngEInMuonFinder[1000];
    G4double ProngEInMuonFinderLayer1X[1000];
    G4double ProngEInMuonFinderLayer1Y[1000];
    G4double ProngEInMuonFinderLayer2X[1000];
    G4double ProngEInMuonFinderLayer2Y[1000];
    G4double ProngAngleToBeamDir[1000];
    G4double ShowerLength[1000];
    G4double ShowerLengthInLAr[1000];
    G4double ShowerWidth[1000];
    G4double ShowerWidthInLAr[1000];
    G4double ProngAvgdEdx[1000];
    G4double ProngAvgdEdxInLAr[1000];
    G4double ProngdEdxAlongTrack[1000][100];
    G4int    ProngdEdxTrackLength[1000][100];
    G4double TotalDedxLongitudinal[3000];
    G4double TrueTotalDedxLongitudinal[3000];
    // reco
    // direction
    G4double dir_pol_x[1000];
    G4double dir_pol_y[1000];
    G4double dir_pol_z[1000];
    G4double dir_coc_x[1000];
    G4double dir_coc_y[1000];
    G4double dir_coc_z[1000];

    G4double edepInLAr;
    G4double edepInHadCalX;
    G4double edepInHadCalY;
    G4double edepInMuonFinderX;
    G4double edepInMuonFinderY;
    G4double edepInHadAborb;
    G4double edepInMuonFinderAbsorb;
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
    G4bool m_save3DEvd;
    G4bool m_save2DEvd;
    G4bool m_circularFit;
    TString m_addDiffusion;

    PixelMap3D* pm3D;
    G4double sparseFractionMem;
    G4double sparseFractionBins;

    // Circular fit in HadCat + MF
    G4int circNhits;         //number of hits
    G4double xc, zc, rc;     //circle fit
    G4double p0, p1, cosDip; //dip angle fit
    std::vector<double> hitXFSL; // MC truth
    std::vector<double> hitZFSL;
    std::vector<double> hitYFSL;
    std::vector<double> hitPFSL;
   
    // Circular fit in FASER magnet(s)
    G4int Nmagnets; // size of output vectors
    std::vector<double> magzpos;
    G4int trkNhits; // hits in tracking stations
    std::vector<double> trkxc; //circle fit results
    std::vector<double> trkzc;
    std::vector<double> trkrc; 
    std::vector<double> trkmIn; // entering track fit
    std::vector<double> trkqIn;
    std::vector<double> trkmOut; // exiting track fit
    std::vector<double> trkqOut;
    G4double trkp0, trkp1, trkcosDip; //dip angle fit
    std::vector<double> trkXFSL; // MC truth
    std::vector<double> trkZFSL;
    std::vector<double> trkYFSL;
    std::vector<double> trkPFSL;

  private:
    void FillPrimaryTruthTree(G4int sdId, std::string sdName);
    void FillTrueEdep(G4int sdId, std::string sdName);
    double GetTotalEnergy(double px, double py, double pz, double m);
    void FillPseudoRecoVar();

    G4int NumberOfSDs;
    std::set<std::pair<int, std::string> > SDNamelist;
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
