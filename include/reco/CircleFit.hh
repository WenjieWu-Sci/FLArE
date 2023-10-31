////////////////////////////////////////////////////////////////////////
// \file    CircularFit.hh
// \brief   Class headers to perform circular fits using analytical methods
//          for momentum estimation in magnetic fields
// \author  M. Vicenzi (mvicenzi@bnl.gov)
////////////////////////////////////////////////////////////////////////

#ifndef CIRCULARFIT_HH
#define CIRCULARFIT_HH

#include <vector>

namespace circularfitter {
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
  
  struct hit {
    double x;
    double y;
    double z;
  };

  class CircleExtractor {
    public:
      CircleExtractor(const std::vector<double> x, const std::vector<double> y, const std::vector<double> z);

      // splits hits between pre and post magnet(s) according to list of magnet z position(s).
      void splitHits(std::vector<double> magnetZs,
                     std::vector<double> x, std::vector<double> y, std::vector<double> z, 
                     std::vector<std::vector<hit>> &hits);
      
      // for a given magnet, compute circle from pre and post hit collections + magnet z limits
      void computeCircle(std::vector<double> zpre, std::vector<double> xpre, std::vector<double> zpost, std::vector<double> xpost, double Zin, double Zout);
      
      // support functions
      double extrapolateLine(double z, line l);
      std::pair<double,double> getPerpLineIntersection(double z1, double x1, double z2, double x2, line l1, line l2);
      double getR(double za, double xa, double zb, double xb);
      
      // return methods
      std::vector<double> GetXc() { return fXc; };
      std::vector<double> GetZc() { return fZc; };
      std::vector<double> GetR();
      std::vector<line> GetPreLine() { return fpre; };
      std::vector<line> GetPostLine() { return fpost; };     

      ~CircleExtractor();
    
    private:
      std::vector<line> fpre;
      std::vector<line> fpost;
      std::vector<double> fZc;
      std::vector<double> fXc;
      std::vector<double> fR1;
      std::vector<double> fR2;        
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
