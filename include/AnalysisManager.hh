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
    void saveSecondary(G4bool val) { m_saveSecondary = val; }
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
    G4int    nPrimaryParticle;  ///<- number of primary particle 
                                ///   (in case of genie neutrino interaction, number of stable particle in the final state)
    G4int    PDG[2000000];      ///<- PDG code of primary particles
    G4double Px[2000000];       ///<- Px of primary particles
    G4double Py[2000000];       ///<- Py of primary particles
    G4double Pz[2000000];       ///<- Pz of primary particles
    G4double Pmass[2000000];    ///<- mass of primary particles
    // Geant4 truth
    G4int    primaryTrackID[2000000];          ///<- track ID of primary particles
    G4int    primaryTrackPDG[2000000];         ///<- PDG of primary particles, for G4 use
    G4double primaryTrackLength[2000000];      ///<- track length of primary particles
    G4double primaryTrackLengthInTPC[2000000]; ///<- track length of primary particles in TPC region
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

    G4int    nFromFSLParticles;
    G4int    fromFSLParticlePDG[2000000];
    G4double fromFSLParticleKinE[2000000];
    G4double fromFSLParticlePx[2000000];
    G4double fromFSLParticlePy[2000000];
    G4double fromFSLParticlePz[2000000];
    G4double fromFSLTrackLength[2000000];
    G4double fromFSLTrackLengthInTPC[2000000];

    G4int    nSecondaryTracks;
    G4int    secondaryTrackPDG[2000000];

    // refined tracks record
    G4int nStepsIn25cm;
    G4int StepPIDIn25cm[2000000];
    G4int StepTIDIn25cm[2000000];
    G4int StepPDGCodeIn25cm[2000000];
    G4int StepnoIn25cm[2000000];
    G4double StepPrePosIn25cmX[2000000];
    G4double StepPrePosIn25cmY[2000000];
    G4double StepPrePosIn25cmZ[2000000];
    G4double StepPostPosIn25cmX[2000000];
    G4double StepPostPosIn25cmY[2000000];
    G4double StepPostPosIn25cmZ[2000000];
    G4double StepEdepIn25cm[2000000];

    G4bool m_saveSecondary;
    G4bool m_saveEvd;

    TH2D* hEdepXY;
    TH2D* hEdepZX;
    TH2D* hEdepZY;
    TH2D* hEdepXYFSL;
    TH2D* hEdepZXFSL;
    TH2D* hEdepZYFSL;

  private:
    void FillTree(G4int sdId, std::string sdName);
    void FillTrueEdep(G4int sdId, std::string sdName);

    G4HCofThisEvent* hcofEvent;

    std::set<std::pair<int, int> > allTracksPTPair;
    std::set<int> tracksFromFSL;
    std::set<int> tracksFromFSLSecondary;
};

#endif
