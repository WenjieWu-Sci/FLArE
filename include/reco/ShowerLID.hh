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

namespace slid {

  /// Number of bins considered for dE/dx histograms, 1 mm per bin.
  static const int kNumLongitudinalBins = 3000;

  class ShowerLID {
    public:
      ShowerLID(THnSparseF* aHist3DEdep, const double aVtxX, const double aVtxY, const double aVtxZ, 
          const double aPx, const double aPy, const double aPz);
      ~ShowerLID();

      Double_t* GetTotalDedxLongitudinal() { return TotalDedxLongitudinal; }

    private:
      Double_t TotalDedxLongitudinal[kNumLongitudinalBins] = { 0. };
  };
}

#endif
