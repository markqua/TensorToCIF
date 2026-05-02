#include "LinearAlgebraUtils.h"

namespace TTC {

AtomCoordinates::AtomCoordinates(double x, double y, double z)
  : _x(x)
  , _y(y)
  , _z(z) {};

AtomCoordinates::~AtomCoordinates()
{};

AtomCoordinates AtomCoordinates::GetCoordinates() const
{
  return *this;
}

double AtomCoordinates::GetNorm() {
    double xSq = _x * _x;
    double ySq = _y * _y;
    double zSq = _z * _z;

    return sqrt(xSq + ySq + zSq);
}

AtomCoordinates Utils::AdaptEigenToTTC(const Eigen::Vector3d& iEigenVec) {
    return AtomCoordinates(iEigenVec[0], iEigenVec[1], iEigenVec[2]);
}

} // namespace TTC
