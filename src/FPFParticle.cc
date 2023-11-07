#include <iterator>
#include <iostream>
#include <climits>

#include "FPFParticle.hh"

FPFParticle::FPFParticle()
  : fPDG(0)
  , fVx(-999)
  , fVy(-999)
  , fVz(-999)
  , fT(-999)
  , fPx(-999)
  , fPy(-999)
  , fPz(-999)
    , fE(-999)
{
}

FPFParticle::FPFParticle(const int pdg,
    const double Vx,
    const double Vy,
    const double Vz,
    const double T,
    const double Px,
    const double Py,
    const double Pz,
    const double E)
  : fPDG(pdg)
  , fVx(Vx)
  , fVy(Vy)
  , fVz(Vz)
  , fT(T)
  , fPx(Px)
  , fPy(Py)
  , fPz(Pz)
    , fE(E)
{
}

FPFParticle::~FPFParticle()
{
}
