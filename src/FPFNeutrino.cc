#include <iterator>
#include <iostream>
#include <climits>

#include "FPFNeutrino.hh"

FPFNeutrino::FPFNeutrino()
  : fNuPDG(0)
  , fNuVx(-999)
  , fNuVy(-999)
  , fNuVz(-999)
  , fNuT(-999)
  , fNuPx(-999)
  , fNuPy(-999)
  , fNuPz(-999)
  , fNuE(-999)
  , fNuIdx(0)
  , fNuIntType(0)
  , fNuScatteringType(0)
  , fNuW(-999)
  , fNuFSLPDG(0)
  , fNuFSLPx(-999)
  , fNuFSLPy(-999)
  , fNuFSLPz(-999)
    , fNuFSLE(-999)
{
}

FPFNeutrino::FPFNeutrino( int pdg,
    double Vx,
    double Vy,
    double Vz,
    double T,
    double Px,
    double Py,
    double Pz,
    double E,
    int nuIdx,
    int nuIntType,
    int nuScatteringType,
    double nuW,
    int nuFSLPDG,
    double nuFSLPx,
    double nuFSLPy,
    double nuFSLPz,
    double nuFSLE)
  : fNuPDG(pdg)
  , fNuVx(Vx)
  , fNuVy(Vy)
  , fNuVz(Vz)
  , fNuT(T)
  , fNuPx(Px)
  , fNuPy(Py)
  , fNuPz(Pz)
  , fNuE(E)
  , fNuIdx(nuIdx)
  , fNuIntType(nuIntType)
  , fNuScatteringType(nuScatteringType)
  , fNuW(nuW)
  , fNuFSLPDG(nuFSLPDG)
  , fNuFSLPx(nuFSLPx)
  , fNuFSLPy(nuFSLPy)
  , fNuFSLPz(nuFSLPz)
    , fNuFSLE(nuFSLE)
{
}

FPFNeutrino::~FPFNeutrino()
{
}

