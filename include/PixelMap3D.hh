#ifndef PIXELMAP3D_HH
#define PIXELMAP3D_HH

#include <vector>

#include <THnSparse.h>
#include <TFile.h>
#include <TH2.h>

#include <G4ThreeVector.hh>

class PixelMap3D {
  public:
    PixelMap3D(const Int_t evtID, const Int_t nPrim, const Int_t PDG, const Double_t* res);
        
    ~PixelMap3D();

    void SetPMBoundary(G4ThreeVector min_xyz, G4ThreeVector max_xyz);
    void InitializePM();
    void FillEntry(const Double_t* pos_xyz, const Double_t* vtx_xyz, const Double_t edep, const Int_t idxPrim);
    void FillEntryWithToyElectronTransportation(const Double_t* pos_xyz, const Double_t* vtx_xyz, Double_t edep, const Int_t idxPrim);
    void FillEntryWithToySingleElectronTransportation(const Double_t* pos_xyz, const Double_t* vtx_xyz, Double_t edep, const Int_t idxPrim);
    void WriteToFile(TFile* thefile, G4bool save3D, G4bool save2D);

    // this should go to a Geometry Service class
    G4double DistanceToAnode(G4double x);
        
    G4double GetSparseFractionMem() { return hist3DEdep->GetSparseFractionMem(); }
    G4double GetSparseFractionBins() { return hist3DEdep->GetSparseFractionBins(); }

    void SetEventID(G4int val) { fEvtID = val; }

  private:
    G4int fEvtID, fNPrim, fGeneratorPDG;
    G4double fResX, fResY, fResZ;
    G4int fNBinX, fNBinY, fNBinZ;
    std::vector<G4double> fBinX, fBinY, fBinZ;
    G4ThreeVector fPMXYZMin;           // global coord. of min{x, y, z}
    G4ThreeVector fPMXYZMax;           // global coord. of max{x, y, z}
    G4ThreeVector fPMShiftedCenterXYZ; // global coord. of local (0, 0, 0)
    G4ThreeVector fPMSizeXYZ;

    std::vector<TH2F*> hitClusterZX;
    std::vector<TH2F*> hitClusterZY;
    std::vector<TH2F*> vtxHitClusterZX;
    std::vector<TH2F*> vtxHitClusterZY;
    THnSparseF* hist3DEdep;
};

#endif
