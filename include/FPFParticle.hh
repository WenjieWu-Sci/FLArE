#ifndef FPFPARTICLE_HH
#define FPFPARTICLE_HH

class FPFParticle
{
  public:

    FPFParticle();

    FPFParticle(const int pdg,
        const double Vx,
        const double Vy,
        const double Vz,
        const double T,
        const double Px,
        const double Py,
        const double Pz,
        const double E);

    ~FPFParticle();

  public:
    // methods to access data members and other information
    int    PDG()   const { return fPDG;}
    double Vx()    const { return fVx; }
    double Vy()    const { return fVy; }
    double Vz()    const { return fVz; }
    double T()     const { return fT;  }
    double Px()    const { return fPx; }
    double Py()    const { return fPy; }
    double Pz()    const { return fPz; }
    double E()     const { return fE;  }

  private:
    int    fPDG;
    double fVx;
    double fVy;
    double fVz;
    double fT;
    double fPx;
    double fPy;
    double fPz;
    double fE;
};

#endif
