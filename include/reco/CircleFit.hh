#ifndef CIRCULARFIT_HH
#define CIRCULARFIT_HH

#include <vector>

namespace circularfitter {

  const double fMagnetZPos = 35960.+3500.;
  const double fMagnetZSize = 4000.; //4 m

  struct line {
    double m;
    double q;
  };

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
      line GetLine(); 

      ~LineFit();

    private:
      double fP0;
      double fP1;
      double fCosDip;
      double fChi2;
      int fStatus;
  };
  
  class CircleExtractor {
    public:
      CircleExtractor(const std::vector<double> zpre, const std::vector<double> xpre,
                      const std::vector<double> zpost, const std::vector<double> xpost);

      double extrapolateLine(double z, line l);
      std::pair<double,double> getPerpLineIntersection(double z1, double x1, double z2, double x2, line l1, line l2);
      double getR(double za, double xa, double zb, double xb);

      double GetXc() { return fXc; };
      double GetZc() { return fZc; };
      double GetR() { return (fR1+fR2)/2.; };
      line GetPreLine() { return fpre; };
      line GetPostLine() { return fpost; };     

      ~CircleExtractor();
    
    private:
      line fpre;
      line fpost;
      double fZc, fXc;
      double fZin, fXin;
      double fZout, fXout;
      double fR1;
      double fR2;        
  };
  
  class ParabolicFit {
    public:
      ParabolicFit(const std::vector<double> z, const std::vector<double> x, double r0);

      double GetA() { return fA; };
      double GetB() { return fB; };
      double GetR() { return fR; };

      ~ParabolicFit();

    private:
      double fA;
      double fB;
      double fR;
  };
}
#endif
