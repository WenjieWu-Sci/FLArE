////////////////////////////////////////////////////////////////////////
// \file    ShowerLID.hh
// \brief   A Shower class that records the log likelihood information 
//          for every shower.
// \version 
// \author  Wenjie Wu (wenjieww@uci.edu)
////////////////////////////////////////////////////////////////////////

#ifndef SHOWERLID_HH
#define SHOWERLID_HH

#include <THnSparse.h>
#include <TH2.h>

namespace slid {

  /// Number of bins considered for dE/dx histograms, 1 mm per bin.
  static const int kNumLongitudinalBins = 3000;

  class ShowerLID {
    public:
      ShowerLID(THnSparseF* aHist3DEdep, const double aVtxX, const double aVtxY, const double aVtxZ, 
          const double aPx, const double aPy, const double aPz);
      ShowerLID(TH2F* px, TH2F* py, const double aVtxX, const double aVtxY, const double aVtxZ,
          const double aPx, const double aPy, const double aPz);
      ~ShowerLID();

      Double_t* GetTotalDedxLongitudinal() { return TotalDedxLongitudinal; }
      Double_t GetShowerLength() { return ShowerLengthFrom2DPM; }
      Double_t GetShowerWidth() { return ShowerWidthFrom2DPM; }
      Int_t    GetNCell() { return NCell; }
      Int_t    GetNPlane() { return NPlane; }

    private:
      Double_t TotalDedxLongitudinal[kNumLongitudinalBins] = { 0. };

      Double_t ShowerLengthFrom2DPM;
      Double_t ShowerWidthFrom2DPM;
      Int_t    NCell;
      Int_t    NPlane;
  };
}

#endif
