#include <iterator>
#include <iostream>
#include <climits>

#include "FPFParticle.hh"

FPFParticle::FPFParticle()
  : fPDG(0)
  , fPID(-1)
  , fTID(-1)
  , fProngIdx(-1)
  , fProngType(-1)
  , fMass(-999)
  , fVx(-999)
  , fVy(-999)
  , fVz(-999)
  , fT(-999)
  , fPx(-999)
  , fPy(-999)
  , fPz(-999)
  , fE(-999)
  , fEndPosx(-999)
  , fEndPosy(-999)
    , fEndPosz(-999)
{
}

FPFParticle::FPFParticle(const int pdg,
    const int pid,
    const int tid,
    const int prongIdx,
    const int prongType,
    const double mass,
    const double Vx,
    const double Vy,
    const double Vz,
    const double T,
    const double Px,
    const double Py,
    const double Pz,
    const double E)
  : fPDG(pdg)
  , fPID(pid)
  , fTID(tid)
  , fProngIdx(prongIdx)
  , fProngType(prongType)
  , fMass(mass)
  , fVx(Vx)
  , fVy(Vy)
  , fVz(Vz)
  , fT(T)
  , fPx(Px)
  , fPy(Py)
  , fPz(Pz)
  , fE(E)
  , fEndPosx(-999)
  , fEndPosy(-999)
    , fEndPosz(-999)
{
}

FPFParticle::~FPFParticle()
{
}
