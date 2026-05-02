#include <cmath>
#include <Eigen/Dense>

class AtomCoordinates {
public:
  AtomCoordinates(double x = 0, double y = 0, double z = 0);
  ~AtomCoordinates();

AtomCoordinates operator+(const AtomCoordinates& other)
{
    _x+=other._x;
    _y+=other._y;
    _z+=other._z;
    return *this;
}

AtomCoordinates operator-(const AtomCoordinates& other)
{
    _x-=other._x;
    _y-=other._y;
    _z-=other._z;
    return *this;
}

double GetNorm();

AtomCoordinates GetCoordinates() const;

private:

  double _x;
  double _y;
  double _z;
};

class Utils {
 public:
    AtomCoordinates AdaptEigenToTTC(const Eigen::Vector3d& iEigenVec);
    double CalculateDistance(AtomCoordinates iCoords1, AtomCoordinates iCoords2);

};