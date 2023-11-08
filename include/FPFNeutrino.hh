#ifndef FPFNEUTRINO_HH
#define FPFNEUTRINO_HH

class FPFNeutrino
{
  public:

    FPFNeutrino();

    FPFNeutrino(const int pdg,
        const double Vx,
        const double Vy,
        const double Vz,
        const double T,
        const double Px,
        const double Py,
        const double Pz,
        const double E,
        const int nuIdx,
        const int nuIntType,
        const int nuScatteringType,
        const double nuW,
        const int nuFSLPDG,
        const double nuFSLPx,
        const double nuFSLPy,
        const double nuFSLPz,
        const double nuFSLE);

    ~FPFNeutrino();

  public:
    // methods to access data members and other information
    int    NuPDG()            const { return fNuPDG;           }
    double NuVx()             const { return fNuVx;            }
    double NuVy()             const { return fNuVy;            }
    double NuVz()             const { return fNuVz;            }
    double NuT()              const { return fNuT;             }
    double NuPx()             const { return fNuPx;            }
    double NuPy()             const { return fNuPy;            }
    double NuPz()             const { return fNuPz;            }
    double NuE()              const { return fNuE;             }
    int    NuIdx()            const { return fNuIdx;           }
    int    NuIntType()        const { return fNuIntType;       }
    int    NuScatteringType() const { return fNuScatteringType;}
    double NuW()              const { return fNuW;             }
    int    NuFSLPDG()         const { return fNuFSLPDG;        }
    double NuFSLPx()          const { return fNuFSLPx;         }
    double NuFSLPy()          const { return fNuFSLPy;         }
    double NuFSLPz()          const { return fNuFSLPz;         }
    double NuFSLE()           const { return fNuFSLE;          }

  private:
    int    fNuPDG;
    double fNuVx;
    double fNuVy;
    double fNuVz;
    double fNuT;
    double fNuPx;
    double fNuPy;
    double fNuPz;
    double fNuE;
    int    fNuIdx;
    int    fNuIntType;
    int    fNuScatteringType;
    double fNuW;
    int    fNuFSLPDG;
    double fNuFSLPx;
    double fNuFSLPy;
    double fNuFSLPz;
    double fNuFSLE;
};

#endif
