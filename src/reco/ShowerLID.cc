////////////////////////////////////////////////////////////////////////
// \file    ShowerLID.cc
// \brief   A Shower class that records the log likelihood information 
//          for every shower.
// \version 
// \author  Wenjie Wu (wenjieww@uci.edu)
////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cstring>

#include "reco/ShowerLID.hh"

#include <THnSparse.h>
#include <TAxis.h>

namespace slid {
  ShowerLID::ShowerLID(THnSparseF* aHist3DEdep,
                       const double aVtxX, const double aVtxY, const double aVtxZ,
                       const double aPx, const double aPy, const double aPz)
  {
    Int_t dim = aHist3DEdep->GetNdimensions();
    Double_t* x = new Double_t[dim+1];
    std::memset(x, 0, sizeof(Double_t) * (dim+1));
    Int_t *bins = new Int_t[dim];
    for (Long64_t i= 0; i< aHist3DEdep->GetNbins(); ++i) {
      x[dim] = aHist3DEdep->GetBinContent(i, bins);
      for (Int_t d= 0; d< dim; ++d) {
        x[d] = aHist3DEdep->GetAxis(d)->GetBinCenter(bins[d]);
      }
      if (x[2]>7000) continue;
      Double_t longitudinal_distance_to_vtx = ((x[0] - aVtxX) * aPx +
        (x[1] - aVtxY) * aPy + (x[2] - aVtxZ) * aPz) / TMath::Sqrt(aPx*aPx+aPy*aPy+aPz*aPz);
      if (Int_t(longitudinal_distance_to_vtx)>=0 && 
          Int_t(longitudinal_distance_to_vtx)<kNumLongitudinalBins) {
        TotalDedxLongitudinal[Int_t(longitudinal_distance_to_vtx)] += x[3];
      }
    }
    delete [] bins;
  }
}
