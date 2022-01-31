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

    G4int    nuIntType;
    G4int    nuScatteringType;
    G4int    nuFSLPDG;
    G4double nuFSLPx;
    G4double nuFSLPy;
    G4double nuFSLPz;
    G4double nuFSLE;

    G4int    nPrimaryParticle;
    G4int    PDG[2000000];
    G4double vtxX[2000000];
    G4double vtxY[2000000];
    G4double vtxZ[2000000];
    G4double Px[2000000];
    G4double Py[2000000];
    G4double Pz[2000000];
    G4double Pmass[2000000];

    G4double edepInLAr;
    G4double edepInHadCalX;
    G4double edepInHadCalY;
    G4double primaryTrackLength;

    G4int    nFSParticles;
    G4int    fsParticlePDG[2000000];
    G4double fsParticleKinE[2000000];
    G4double fsParticlePx[2000000];
    G4double fsParticlePy[2000000];
    G4double fsParticlePz[2000000];
    G4int    nSecondaryTracks;
    G4int    secondaryTrackPDG[2000000];

    G4bool m_saveSecondary;
    G4bool m_saveEvd;

    TH2D* hEdepXY;
    TH2D* hEdepZX;
    TH2D* hEdepZY;

  private:
    G4int flArBoxSDId  { -1 };
    G4int fHadCalXSDId { -1 };
    G4int fHadCalYSDId { -1 };
//    std::set<G4int> allTrackID;
};

#endif
