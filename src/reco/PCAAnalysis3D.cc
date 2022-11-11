#include "reco/PCAAnalysis3D.hh"
#include "reco/Cluster3D.hh"
#include "utils/range.hh"

#include <TMath.h>

// Eigen includes
#include "Eigen/Core"
#include "Eigen/Dense"
#include "Eigen/Eigenvalues"
#include "Eigen/Geometry"
#include "Eigen/Jacobi"

namespace pcaanalysis3d {
  PCAAnalysis3D::PCAAnalysis3D(const std::vector<std::vector<ROOT::Math::XYZPoint> > & HitClusters,
      const std::vector<std::vector<double> > & HitEdeps,
      const std::vector<ROOT::Math::XYZPoint> & Vertex):
    f3DHitClusters(HitClusters),
    f3DHitEdeps(HitEdeps),
    fVertex(Vertex)
  {
    Initialize();
    runPCA();
  }

  PCAAnalysis3D::~PCAAnalysis3D()
  {
  }

  void PCAAnalysis3D::runPCA() {
    reco::PrincipalComponents pca = reco::PrincipalComponents();
    // run PCA fit for each cluster
    for (unsigned int iprim= 0; iprim< f3DHitClusters.size(); ++iprim) {
      PCAFit(f3DHitClusters[iprim], f3DHitEdeps[iprim], pca, fVertex[iprim]);
      fPCA3DDir.push_back(ROOT::Math::XYZPoint(pca.getEigenVectors().row(2)(0),
            pca.getEigenVectors().row(2)(1), pca.getEigenVectors().row(2)(2)));

      fCOCDir.push_back(CenterOfChargeDir(f3DHitClusters[iprim], f3DHitEdeps[iprim], fVertex[iprim]));
      //if (iprim==4) {
      //  for (auto hit : f3DHitClusters[iprim]) {
      //    std::cout<<hit.X()<<" "<<hit.Y()<<" "<<hit.Z()<<std::endl;
      //  }
      //}
    }
  }

  void PCAAnalysis3D::PCAFit(const std::vector<ROOT::Math::XYZPoint> & HitCluster,
                             std::vector<double> & HitEdep,
                             reco::PrincipalComponents& pca,
                             ROOT::Math::XYZPoint & vertex)
  {
    // We want to run a PCA on the input TkrVecPoints...
    // The steps are:
    // 1) do a mean normalization of the input vec points
    // 2) compute the covariance matrix
    // 3) run the SVD
    // 4) extract the eigen vectors and values
    // see what happens

    // Run through Hits and get the mean position of all the hits
    Eigen::Vector3d meanPos(Eigen::Vector3d::Zero());
    double meanWeightSum(0.);
    int numHits(0);
    for (unsigned i : util::lang::indices(HitCluster)) {
    //for (auto hit : HitCluster) {
      auto hit = HitCluster[i];
      auto weight = HitEdep[i];
      //double weight = 1;
      meanPos(0) += hit.X() * weight;
      meanPos(1) += hit.Y() * weight;
      meanPos(2) += hit.Z() * weight;
      numHits++;

      meanWeightSum += weight;
    }
    meanPos /= meanWeightSum;

    // Define elements of our covariance matrix
    double xi2(0.);
    double xiyi(0.);
    double xizi(0.0);
    double yi2(0.0);
    double yizi(0.0);
    double zi2(0.);
    double weightSum(0.);

    // Back through the hits to build the matrix
    for (const auto& hit : HitCluster) {

      double weight = 1.;

      double x = (hit.X() - meanPos(0)) * weight;
      double y = (hit.Y() - meanPos(1)) * weight;
      double z = (hit.Z() - meanPos(2)) * weight;

      weightSum += weight * weight;

      xi2 += x * x;
      xiyi += x * y;
      xizi += x * z;
      yi2 += y * y;
      yizi += y * z;
      zi2 += z * z;
    }

    // Using Eigen package
    Eigen::Matrix3d sig;

    sig << xi2, xiyi, xizi, xiyi, yi2, yizi, xizi, yizi, zi2;

    sig *= 1. / weightSum;

    Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> eigenMat(sig);

    if (eigenMat.info() == Eigen::ComputationInfo::Success) {
      // Now copy output
      // The returned eigen values and vectors will be returned in an xyz system where x is the smallest spread,
      // y is the next smallest and z is the largest. Adopt that convention going forward
      reco::PrincipalComponents::EigenValues recobEigenVals = eigenMat.eigenvalues().cast<float>();
      reco::PrincipalComponents::EigenVectors recobEigenVecs =
        eigenMat.eigenvectors().transpose().cast<float>();

      // Check for a special case (which may have gone away with switch back to doubles for computation?)
      if (std::isnan(recobEigenVals[0])) {
        std::cout << "==> Third eigenvalue returns a nan" << std::endl;

        recobEigenVals[0] = 0.;

        // Assume the third axis is also kaput?
        recobEigenVecs.row(0) = recobEigenVecs.row(1).cross(recobEigenVecs.row(2));
      }

      // Store away
      pca = reco::PrincipalComponents(
        true, numHits, recobEigenVals, recobEigenVecs, meanPos.cast<float>());
    } else {
      std::cout << "(PCAAnalysis3D) PCA decompose failure, numPairs = " << numHits << std::endl;
      pca = reco::PrincipalComponents();
    }

    if ((meanPos(2)-vertex.Z())*(pca.getEigenVectors().row(2)(2))<0) {
      pca.flipAxis(2);
    }
    //std::cout<<"AngleToBeamDir : "<<TMath::ACos(pca.getEigenVectors().row(2)(2))<<std::endl;

    return;
  }

  ROOT::Math::XYZPoint PCAAnalysis3D::CenterOfChargeDir(std::vector<ROOT::Math::XYZPoint> & HitCluster,
                         std::vector<double> & HitEdep,
                         ROOT::Math::XYZPoint & vertex)

  {
    // Run through Hits and get the mean position of all the hits
    Eigen::Vector3d meanPos(Eigen::Vector3d::Zero());
    double meanWeightSum(0.);
    int numHits(0);
    for (unsigned i : util::lang::indices(HitCluster)) {
      auto hit = HitCluster[i];
      auto weight = HitEdep[i];
      //double weight = 1;
      meanPos(0) += hit.X() * weight;
      meanPos(1) += hit.Y() * weight;
      meanPos(2) += hit.Z() * weight;
      numHits++;

      meanWeightSum += weight;
    }

    ROOT::Math::XYZPoint COCDir;
    if (meanWeightSum!=0.) {
      meanPos /= meanWeightSum;
      COCDir = ROOT::Math::XYZPoint(meanPos(0)-vertex.X(),
                                    meanPos(1)-vertex.Y(),
                                    meanPos(2)-vertex.Z());
      COCDir /= TMath::Sqrt(COCDir.Mag2());
      //std::cout<<"COCDirToBeamDir : "<<TMath::ACos(COCDir.Z())<<std::endl;
    } else {
      meanPos /= numHits;
      COCDir = ROOT::Math::XYZPoint(0, 0, 0);
      //std::cout<<"CenterOfCharge failed, zero deposited energy"<<std::endl;
    }

    return COCDir;
  }

  void PCAAnalysis3D::Initialize() {
    fPCA3DDir.clear();
    fCOCDir.clear();
  }

}
