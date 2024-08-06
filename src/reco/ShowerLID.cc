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
#include "geometry/GeometricalParameters.hh"
#include "PixelMap3D.hh"

#include <THnSparse.h>
#include <TAxis.h>

namespace slid {
  ShowerLID::ShowerLID()
  {
  }


  ShowerLID::~ShowerLID()
  {
  }


  void ShowerLID::CalculateDedxFrom3DPM(THnSparseF* aHist3DEdep,
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
      if (x[2]>GeometricalParameters::Get()->GetTPCSizeZ()) continue;
      Double_t longitudinal_distance_to_vtx = ((x[0] - aVtxX) * aPx +
        (x[1] - aVtxY) * aPy + (x[2] - aVtxZ) * aPz) / TMath::Sqrt(aPx*aPx+aPy*aPy+aPz*aPz);
      if (Int_t(longitudinal_distance_to_vtx)>=0 && 
          Int_t(longitudinal_distance_to_vtx)<kNumLongitudinalBins) {
        TotalDedxLongitudinal[Int_t(longitudinal_distance_to_vtx)] += x[3];
      }
    }
    delete [] bins;
  }


  void ShowerLID::CalculateShowerFrom2DPM(TH2F* px, TH2F* py,
                       const double aVtxX, const double aVtxY, const double aVtxZ,
                       const double aPx, const double aPy, const double aPz)
  {
    Int_t nbins_x = px->GetNbinsY();
    Int_t nbins_y = py->GetNbinsY();
    Int_t nbins_z = py->GetNbinsX();

    ShowerLengthFrom2DPM = 0;
    ShowerWidthFrom2DPM = 0;
    NCell = 0;
    NPlane = 0;

    // find the z-plane where the vertex is located at
    Int_t vtx_binZ = px->GetXaxis()->FindBin(aVtxZ);
    Double_t TotalEdep = px->Integral();

    Double_t pmag = TMath::Sqrt(aPx*aPx+aPy*aPy+aPz*aPz);
    if (pmag==0) {
      ShowerLengthFrom2DPM = 0;
      ShowerWidthFrom2DPM = 0;
    } else {
      Double_t tmp_showerlength = 0;
      Double_t tmp_showerwidth = 0;
      Double_t tot_e = 0;
      for (Long64_t i= 0; i< nbins_z; ++i) {
        // calculate the deposited energy between current plane and the plane where the vertex is
        // if the deposited energy is 99.7% (3sigma) or higher of the total deposited energy, exclude this plane
        Double_t interEdep = px->ProjectionX()->Integral(vtx_binZ<=i+1?vtx_binZ:i+1, vtx_binZ<=i+1?i+1:vtx_binZ);
        if (interEdep/TotalEdep > 0.997) continue;

        Int_t flag_validPlane = 0;
        for (Long64_t j= 0; j< nbins_x; ++j) {
          Double_t xview_e = px->GetBinContent(i+1, j+1);
          if (xview_e>0.1) {  // only count pixels with deposited energy higher than 0.1 MeV
            Double_t x = px->GetYaxis()->GetBinCenter(j+1);
            Double_t z = px->GetXaxis()->GetBinCenter(i+1);
            // find matched bin in the y-view
            Int_t matched_biny = 0;
            Double_t min_diff = 1e6;
            for (Long64_t k= 0; k< nbins_y; ++k) {
              Double_t tmp_yview_e = py->GetBinContent(i+1, k+1);
              if ( (tmp_yview_e>0.1) && (abs(xview_e-tmp_yview_e)<min_diff) ) { 
                min_diff = abs(xview_e - tmp_yview_e);
                matched_biny = k+1;
              }
            }
            if (matched_biny>0) { // matched bin in the y-view found
              NCell++;
              flag_validPlane = 1;
              Double_t yview_e = py->GetBinContent(i+1, matched_biny);
              tot_e += (xview_e+yview_e)/2.;
              Double_t y = py->GetYaxis()->GetBinCenter(matched_biny);
              // calculate the shower/track length and width
              // length: defined as the longest projection distance at the true direction between vertex and hits
              //         length = |\vector{hit_position}|\cdot\cos(theta) = \vertor{hit_position}\cdot\vector{P} / |\vector{P}|
              // width: defined as the weighted average of the least distance of the hits to the true direction
              Double_t tmp_len = abs((x-aVtxX)*aPx+(y-aVtxY)*aPy+(z-aVtxZ)*aPz)/pmag;
              if (tmp_len>tmp_showerlength) tmp_showerlength = tmp_len;

              Double_t dsquare_hit_vtx = TMath::Power((x-aVtxX),2)+
                                         TMath::Power((y-aVtxY),2)+
                                         TMath::Power((z-aVtxZ),2);
              Double_t width_hit = TMath::Sqrt((dsquare_hit_vtx - tmp_len*tmp_len));
              Double_t weighted_width_hit = width_hit*(xview_e+yview_e)/2;
              if (!std::isnan(weighted_width_hit)) tmp_showerwidth += weighted_width_hit;
            }
          }
        }
        if (flag_validPlane>0) NPlane++;
      }
      ShowerLengthFrom2DPM = tmp_showerlength;
      ShowerWidthFrom2DPM = tot_e>0 ? tmp_showerwidth/tot_e : 0;
    }
  }
                      
  void ShowerLID::CalculateEdepProfileFrom2DPM(TH2F* px, TH2F* py)
  {
    Int_t nbins_x = px->GetNbinsY();
    Int_t nbins_y = py->GetNbinsY();
    Int_t nbins_z = py->GetNbinsX();
    for (Long64_t i= 0; i< nbins_z; ++i) {
      if ((i+2)%7==0) {
        for (Long64_t j= 0; j< nbins_x; ++j) {
          XViewEdepProfile[(Int_t((i+2)/7)-1)*50+j] = px->GetBinContent(i+1,j+1);
        }
      }
      if ((i+1)%7==0) {
        for (Long64_t k= 0; k< nbins_y; ++k) {
          YViewEdepProfile[(Int_t((i+1)/7)-1)*60+k] = py->GetBinContent(i+1,k+1);
        }
      }
    }
  }

}
