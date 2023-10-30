#ifndef PIXELMAP3D_HH
#define PIXELMAP3D_HH

#include <THnSparse.h>
#include <G4ThreeVector.hh>

class PixelMap3D {
  public:
    PixelMap3D();
    ~PixelMap3D();

    void SetPMBoundary(G4ThreeVector min, G4ThreeVector max);
    void SetPMResolution(G4ThreeVector res);
    void FillEntry(G4ThreeVector xyz, G4double edep);

  private:
    G4double xmin, ymin, zmin;  // unit: mm  
    G4double xmax, ymax, zmax;  // unit: mm
    G4double resx, resy, resz;  // unit: mm
    THnSparseF* hist3DEdep;
};

#endif
