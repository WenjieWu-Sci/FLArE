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

    TFile*   thefile;
    TString  m_filename;
    TTree*   evt;
    G4int    evtID;
    G4double vtxX;
    G4double vtxY;
    G4double vtxZ;
    //G4double vtxX2;
    //G4double vtxY2;
    //G4double vtxZ2;
    G4double edepInLAr;
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
    G4int flArBoxSDId { -1 };
//    std::set<G4int> allTrackID;
};

#endif
