#include "PixelMap3D.hh"
#include <G4ThreeVector.hh>

PixelMap3D::PixelMap3D() :
  xmin(-900), ymin(-900), zmin(800),
  xmax(900), ymax(900), zmax(7800),
  resx(1), resy(5), resz(5)
{
}

PixelMap3D::~PixelMap3D() {
  delete hist3DEdep;
}

void PixelMap3D::SetPMBoundary(G4ThreeVector min, G4ThreeVector max) {
  xmin = min.x();
  ymin = min.y();
  zmin = min.z();
  xmax = max.x();
  ymax = max.y();
  zmax = max.z();
}

void PixelMap3D::SetPMResolution(G4ThreeVector res) {
  resx = rec.x();
  resy = rec.y();
  resz = rec.z();
}
