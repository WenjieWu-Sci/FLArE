#ifndef LINEARFIT_HH
#define LINEARFIT_HH

#include <TH2F.h>
#include <Math/Point3D.h>

namespace directionfitter {
  class LinearFit {
    public:
      LinearFit(TH2F* aEvdViewX, TH2F* aEvdViewY,
                TH2F* aVtxEvdViewX, TH2F* aVtxEvdViewY,
                const double aVtxX, const double aVtxY, const double aVtxZ,
                const double bVtxX, const double bVtxY, const double bVtxZ);

      ROOT::Math::XYZPoint GetDir() { return fDir; };
      ROOT::Math::XYZPoint GetCOCDir() { return fCOCDir; };

      ~LinearFit();


    private:
      ROOT::Math::XYZPoint fDir;
      ROOT::Math::XYZPoint fCOCDir;
  };
}

#endif
