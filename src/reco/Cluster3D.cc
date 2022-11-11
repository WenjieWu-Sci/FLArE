#include <iomanip>

#include "reco/Cluster3D.hh"

namespace reco{

PrincipalComponents::PrincipalComponents() :
       m_svdOK(false),
       m_numHitsUsed(0),
       m_eigenValues(EigenValues::Zero()),
       m_eigenVectors(EigenVectors::Zero()),
       m_avePosition(Eigen::Vector3f::Zero()),
       m_aveHitDoca(9999.)
{}

PrincipalComponents::PrincipalComponents(bool ok, int nHits, const EigenValues& eigenValues, const EigenVectors& eigenVecs, const Eigen::Vector3f& avePos, const float aveHitDoca) :
           m_svdOK(ok),
           m_numHitsUsed(nHits),
           m_eigenValues(eigenValues),
           m_eigenVectors(eigenVecs),
           m_avePosition(avePos),
           m_aveHitDoca(aveHitDoca)
{}

void PrincipalComponents::flipAxis(size_t axisDir)
{
    m_eigenVectors.row(axisDir) = -m_eigenVectors.row(axisDir);

    return;
}

std::ostream&  operator << (std::ostream & o, const PrincipalComponents& a)
{
    if (a.m_svdOK)
    {
        o << std::setiosflags(std::ios::fixed) << std::setprecision(2);
        o << " PCAxis ID run with " << a.m_numHitsUsed << " space points" << std::endl;
        o << "   - center position: " << std::setw(6) << a.m_avePosition(0) << ", " << a.m_avePosition(1) << ", " << a.m_avePosition(2) << std::endl;
        o << "   - eigen values: " << std::setw(8) << std::right << a.m_eigenValues(0) << ", "
        << a.m_eigenValues(1) << ", " << a.m_eigenValues(1) << std::endl;
        o << "   - average doca: " << a.m_aveHitDoca << std::endl;
        o << "   - Principle axis: " << std::setw(7) << std::setprecision(4) << a.m_eigenVectors(0,0) << ", " << a.m_eigenVectors(0,1) << ", " << a.m_eigenVectors(0,2) << std::endl;
        o << "   - second axis:    " << std::setw(7) << std::setprecision(4) << a.m_eigenVectors(1,0) << ", " << a.m_eigenVectors(1,1) << ", " << a.m_eigenVectors(1,2) << std::endl;
        o << "   - third axis:     " << std::setw(7) << std::setprecision(4) << a.m_eigenVectors(2,0) << ", " << a.m_eigenVectors(2,1) << ", " << a.m_eigenVectors(2,2) << std::endl;
    }
    else
        o << " Principal Components Axis is not valid" << std::endl;

    return o;
}

bool operator < (const PrincipalComponents& a, const PrincipalComponents& b)
{
    if (a.m_svdOK && b.m_svdOK)
        return a.m_eigenValues(0) > b.m_eigenValues(0);

    return false; //They are equal
}

}// namespace
