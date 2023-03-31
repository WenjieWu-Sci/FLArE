#ifndef CIRCULARFIT_HH
#define CIRCULARFIT_HH

#include <vector>

namespace circularfitter {
  class CircleFit {
    public:
      CircleFit(const std::vector<double> x, const std::vector<double> z);

      double GetXc() { return fXc; };
      double GetZc() { return fZc; };
      double GetR() { return fR; };
      double GetChi2() { return fChi2; };
      int GetStatus() { return fStatus; };

      ~CircleFit();

    private:
      double fXc;
      double fZc;
      double fR;
      double fChi2;
      int fStatus;
  };
}

#endif
