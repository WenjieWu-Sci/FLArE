#ifndef PCAANALYSIS3D_HH
#define PCAANALYSIS3D_HH

#include <vector>
#include <Math/Point3D.h>

#include "reco/Cluster3D.hh"

namespace pcaanalysis3d {
  class PCAAnalysis3D{
    public:
      PCAAnalysis3D(const std::vector<std::vector<ROOT::Math::XYZPoint> > & HitClusters,
                    const std::vector<ROOT::Math::XYZPoint> & Vertex);
      //PCAAnalysis3D();
      ~PCAAnalysis3D();

    private:
      void runPCA();
      void PCAFit(std::vector<ROOT::Math::XYZPoint> & HitCluster, 
                  reco::PrincipalComponents& pca,
                  ROOT::Math::XYZPoint & vertex);
      std::vector<std::vector<ROOT::Math::XYZPoint> > f3DHitClusters;
      std::vector<ROOT::Math::XYZPoint> fVertex;
  };
}

#endif
