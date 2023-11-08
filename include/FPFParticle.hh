#ifndef FPFPARTICLE_HH
#define FPFPARTICLE_HH

#include <TMath.h>

class FPFParticle
{
  public:

    FPFParticle();

    FPFParticle(const int pdg,
        const int    pid,
        const int    tid,
        const int    prongIdx,
        const int    prongType,
        const double mass,
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
    int    PDG()       const { return fPDG;      }
    int    PID()       const { return fPID;      }
    int    TID()       const { return fTID;      }
    int    ProngIdx()  const { return fProngIdx; }
    int    ProngType() const { return fProngType;}
    double Mass()      const { return fMass;     }
    double Vx()        const { return fVx;       }
    double Vy()        const { return fVy;       }
    double Vz()        const { return fVz;       }
    double T()         const { return fT;        }
    double Px()        const { return fPx;       }
    double Py()        const { return fPy;       }
    double Pz()        const { return fPz;       }
    double E()         const { return fE;        }
    double P()         const { return TMath::Sqrt(fPx*fPx+fPy*fPy+fPz*fPz); }
    void   SetProngType(int _val) { fProngType = _val; }

  private:
    int    fPDG;         ///<- PDG of primary particles, for G4 use
    int    fPID;         ///<- parent ID of primary particles
    int    fTID;         ///<- track ID of primary particles
    int    fProngIdx;
    int    fProngType;   ///<- 0: final state lepton
                         ///<- 1: original primary particle
                         ///<- 2: decay products of the short lived final state lepton (namely tau-,
                         ///<-    could also be a michel electron from muon decay)
                         ///<- 3: decay products of the primary pizero (dominantely 2gamma)
                         ///<- 4: decay prodcuts of the tau decay pizero
    double fMass;        ///<- mass of primary particle
    double fVx;          ///<- Vx of primary particles
    double fVy;          ///<- Vy of primary particles
    double fVz;          ///<- Vz of primary particles
    double fT;
    double fPx;          ///<- Px of primary particles
    double fPy;          ///<- Py of primary particles
    double fPz;          ///<- Pz of primary particles
    double fE;               
};                           
                             
#endif                       
