#include "reco/CircleFit.hh"

#include <vector>
#include <TMath.h>

namespace circularfitter {
  CircleFit::CircleFit(const std::vector<double> x, const std::vector<double> z)
  {
    fXc = -9999;
    fZc = -9999;
    fR = -9999;
    fChi2 = -9999;

    // This code follows an analytical method for circle fitting (Modified Least Squares method)
    // Ref: https://doi.org/10.1109/TIM.2003.820472
    // using paper formulas with y -> z
    
    if( x.size() != z.size() ) { fStatus = 1; return; }	
    int n = x.size(); 

    double sumx = 0, sumz = 0;                            // linear    terms
    double sumx2 = 0, sumz2 = 0, sumxz = 0;               // quadratic terms
    double sumxz2 = 0, sumx2z = 0, sumx3 = 0, sumz3 = 0;  // cubic     terms

    for (int i = 0; i < n; i++) {
      double xp = x.at(i);
      double zp = z.at(i);
      sumx += xp;
      sumz += zp;
      sumx2 += xp * xp;
      sumz2 += zp * zp;
      sumxz += xp * zp;
      sumxz2 += xp * zp * zp;
      sumx2z += xp * xp * zp;
      sumx3 += xp * xp * xp;
      sumz3 += zp * zp * zp;
    }

    double a = n * sumx2 - sumx * sumx;
    double b = n * sumxz - sumx * sumz;
    double c = n * sumz2 - sumz * sumz;
    double d = 0.5 * (n * sumxz2 - sumx * sumz2 + n * sumx3 - sumx * sumx2);
    double e = 0.5 * (n * sumx2z - sumz * sumx2 + n * sumz3 - sumz * sumz2);

    if (a * c - b * b == 0.) { fStatus = 2; return; }
 
    //center of the circle
    fXc = (d * c - b * e) / (a * c - b * b);
    fZc = (a * e - b * d) / (a * c - b * b);

    //radius of the circle
    fR = 0;
    for (int i = 0; i < n; i++) {
      double xp = x.at(i);
      double zp = z.at(i);
      double r2 = (xp - fXc) * (xp - fXc) + (zp - fZc) * (zp - fZc);

      fR += TMath::Sqrt(r2);
    }

    fR /= n;

    //compute chi2
    fChi2 = 0.0;
    for (int i = 0; i < n; i++) {
      fChi2 += TMath::Abs((z.at(i) - fZc) * (z.at(i) - fZc) + (x.at(i) - fXc) * (x.at(i) - fXc) - fR * fR);
    }

    fChi2 /= n;
    fStatus = 0;
  }

  CircleFit::~CircleFit() 
  {
  }
}
