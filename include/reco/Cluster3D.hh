#ifndef RECO_CLUSTER3D_H
#define RECO_CLUSTER3D_H

#include <iosfwd>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <unordered_map>

// Eigen
#include <Eigen/Core>

namespace reco {
// a container for the output of the PCA Analysis
class PrincipalComponents
{
public:

    using EigenValues  = Eigen::Vector3f;
    using EigenVectors = Eigen::Matrix3f;

    PrincipalComponents();

private:

    bool            m_svdOK;              ///< SVD Decomposition was successful
    int             m_numHitsUsed;        ///< Number of hits in the decomposition
    EigenValues     m_eigenValues;        ///< Eigen values from SVD decomposition
    EigenVectors    m_eigenVectors;       ///< The three principle axes
    Eigen::Vector3f m_avePosition;        ///< Average position of hits fed to PCA
    mutable double  m_aveHitDoca;         ///< Average doca of hits used in PCA

public:

    PrincipalComponents(bool ok, int nHits, const EigenValues& eigenValues, const EigenVectors& eigenVecs, const Eigen::Vector3f& avePos, const float aveHitDoca = 9999.);

    bool                   getSvdOK()                 const {return m_svdOK;}
    int                    getNumHitsUsed()           const {return m_numHitsUsed;}
    const EigenValues&     getEigenValues()           const {return m_eigenValues;}
    const EigenVectors&    getEigenVectors()          const {return m_eigenVectors;}
    const Eigen::Vector3f& getAvePosition()           const {return m_avePosition;}
    const float            getAveHitDoca()            const {return m_aveHitDoca;}

    void                   flipAxis(size_t axis);
    void                   setAveHitDoca(double doca) const {m_aveHitDoca = doca;}

    friend std::ostream&  operator << (std::ostream & o, const PrincipalComponents& a);
    friend bool operator < (const PrincipalComponents& a, const PrincipalComponents& b);

};

}

#endif //RECO_CLUSTER3D_H
