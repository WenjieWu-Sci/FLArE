#include "reco/LinearFit.hh"

#include <TH2F.h>
#include <TF1.h>
#include <TFitResultPtr.h>
#include <TFitResult.h>

namespace directionfitter {
  LinearFit::LinearFit(const TH2F* aEvdViewX, const TH2F* aEvdViewY,
                       const TH2F* aVtxEvdViewX, const TH2F* aVtxEvdViewY,
                       const double aVtxX, const double aVtxY, const double aVtxZ,
                       const double bVtxX, const double bVtxY, const double bVtxZ)
  {
    TF1* fx = new TF1("fx", "[0]*(x-[1])+[2]", -20, 280);
    TF1* fy = new TF1("fy", "[0]*(x-[1])+[2]", -20, 280);
    double dir_z = 0; 
    double dir_x = 0; 
    double dir_y = 0;
    double dir_coc_z = 0;
    double dir_coc_x = 0;
    double dir_coc_y = 0;
    if (aVtxEvdViewX->Integral() > 0 && aVtxEvdViewY->Integral() > 0) {
      double center_xz_z = aVtxEvdViewX->GetMean(1);
      double center_xz_x = aVtxEvdViewX->GetMean(2);
      double center_yz_z = aVtxEvdViewY->GetMean(1);
      double center_yz_y = aVtxEvdViewY->GetMean(2);
      fx->FixParameter(1, bVtxZ-aVtxZ);
      fx->FixParameter(2, bVtxX-aVtxX);
      fy->FixParameter(1, bVtxZ-aVtxZ);
      fy->FixParameter(2, bVtxY-aVtxY);
      TFitResultPtr rx = ((TH2F*)aVtxEvdViewX->Clone())->Fit("fx", "SQ0");
      TFitResultPtr ry = ((TH2F*)aVtxEvdViewY->Clone())->Fit("fy", "SQ0");
      if (rx->IsValid() && ry->IsValid()) {
        if (center_xz_z - bVtxZ + aVtxZ > 0) {
          dir_z = 1/TMath::Sqrt(1+TMath::Power(rx->Parameter(0),2)+TMath::Power(ry->Parameter(0),2));
        } else {
          dir_z = -1/TMath::Sqrt(1+TMath::Power(rx->Parameter(0),2)+TMath::Power(ry->Parameter(0),2));
        }
        dir_x = rx->Parameter(0) * dir_z;
        dir_y = ry->Parameter(0) * dir_z;
      }
      // center of charge
      double slope_xz = (center_xz_x-bVtxX+aVtxX)/(center_xz_z-bVtxZ+aVtxZ);
      double slope_yz = (center_yz_y-bVtxY+aVtxY)/(center_yz_z-bVtxZ+aVtxZ);
      if (center_xz_z - bVtxZ + aVtxZ > 0) {
        dir_coc_z = 1/TMath::Sqrt(1+TMath::Power(slope_xz,2)+TMath::Power(slope_yz,2));
      } else {
        dir_coc_z = -1/TMath::Sqrt(1+TMath::Power(slope_xz,2)+TMath::Power(slope_yz,2));
      }
      dir_coc_x = slope_xz * dir_coc_z;
      dir_coc_y = slope_yz * dir_coc_z;
    } else if (aEvdViewX->Integral() > 0 && aEvdViewY->Integral() > 0) {
      double center_xz_z = aEvdViewX->GetMean(1);
      double center_xz_x = aEvdViewX->GetMean(2);
      double center_yz_z = aEvdViewY->GetMean(1);
      double center_yz_y = aEvdViewY->GetMean(2);
      fx->FixParameter(1, bVtxZ);
      fx->FixParameter(2, bVtxX);
      fy->FixParameter(1, bVtxZ);
      fy->FixParameter(2, bVtxY);
      TFitResultPtr rx = ((TH2F*)aEvdViewX->Clone())->Fit("fx", "SQ0");
      TFitResultPtr ry = ((TH2F*)aEvdViewY->Clone())->Fit("fy", "SQ0");
      if (rx->IsValid() && ry->IsValid()) {
        if (center_xz_z - bVtxZ > 0) {
          dir_z = 1/TMath::Sqrt(1+TMath::Power(rx->Parameter(0),2)+TMath::Power(ry->Parameter(0),2));
        } else {
          dir_z = -1/TMath::Sqrt(1+TMath::Power(rx->Parameter(0),2)+TMath::Power(ry->Parameter(0),2));
        }
        dir_x = rx->Parameter(0) * dir_z;
        dir_y = ry->Parameter(0) * dir_z;
      }
      // center of charge
      double slope_xz = (center_xz_x-bVtxX)/(center_xz_z-bVtxZ);
      double slope_yz = (center_yz_y-bVtxY)/(center_yz_z-bVtxZ);
      if (center_xz_z - bVtxZ > 0) {
        dir_coc_z = 1/TMath::Sqrt(1+TMath::Power(slope_xz,2)+TMath::Power(slope_yz,2));
      } else {
        dir_coc_z = -1/TMath::Sqrt(1+TMath::Power(slope_xz,2)+TMath::Power(slope_yz,2));
      }
      dir_coc_x = slope_xz * dir_coc_z;
      dir_coc_y = slope_yz * dir_coc_z;
    }
    fDir = ROOT::Math::XYZPoint(dir_x, dir_y, dir_z);
    fCOCDir = ROOT::Math::XYZPoint(dir_coc_x, dir_coc_y, dir_coc_z);
    delete fx;
    delete fy;
  }

  LinearFit::~LinearFit() 
  {
  }
}
