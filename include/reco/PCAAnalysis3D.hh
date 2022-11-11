#ifndef PCAANALYSIS3D_HH
#define PCAANALYSIS3D_HH

#include <vector>
#include <Math/Point3D.h>

#include "reco/Cluster3D.hh"

namespace pcaanalysis3d {
  class PCAAnalysis3D{
    public:
      PCAAnalysis3D(const std::vector<std::vector<ROOT::Math::XYZPoint> > & HitClusters,
                    const std::vector<std::vector<double> > & HitEdeps,
                    const std::vector<ROOT::Math::XYZPoint> & Vertex);
      //PCAAnalysis3D();
      ~PCAAnalysis3D();

      std::vector<ROOT::Math::XYZPoint> GetPCA3DDir() { return fPCA3DDir; };
      std::vector<ROOT::Math::XYZPoint> GetCOCDir() { return fCOCDir; };

    private:
      void runPCA();
      void Initialize();
      void PCAFit(const std::vector<ROOT::Math::XYZPoint> & HitCluster, 
                  std::vector<double> & HitEdep,
                  reco::PrincipalComponents& pca,
                  ROOT::Math::XYZPoint & vertex);
      ROOT::Math::XYZPoint CenterOfChargeDir(
                             std::vector<ROOT::Math::XYZPoint> & HitCluster,
                             std::vector<double> & HitEdep,
                             ROOT::Math::XYZPoint & vertex);
      std::vector<std::vector<ROOT::Math::XYZPoint> > f3DHitClusters;
      std::vector<std::vector<double> > f3DHitEdeps;
      std::vector<ROOT::Math::XYZPoint> fVertex;

      std::vector<ROOT::Math::XYZPoint> fPCA3DDir;
      std::vector<ROOT::Math::XYZPoint> fCOCDir;
  };
}

#endif
