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

  class LineFit {
    public:
      LineFit(const std::vector<double> z, const std::vector<double> y);

      double GetP0() { return fP0; };
      double GetP1() { return fP1; };
      double GetCosDip() { return fCosDip; };
      double GetChi2() { return fChi2; };
      int GetStatus() { return fStatus; };

      ~LineFit();

    private:
      double fP0;
      double fP1;
      double fCosDip;
      double fChi2;
      int fStatus;
  };


}

#endif
