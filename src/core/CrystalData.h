#pragma once

#include <Eigen/Dense>
#include <string>
#include <vector>

// Unit-cell parameters with fractional Cartesian transforms (lengths in angstrom).
struct UnitCell {
  double a = 1.0;
  double b = 1.0;
  double c = 1.0;
  // Angles in degrees.
  double alpha = 90.0;
  double beta = 90.0;
  double gamma = 90.0;
  // Cartesian = fracToCart * frac_coords (Eigen column vectors, angstrom).
  Eigen::Matrix3d fracToCart;
  double volume = 1.0;
};

// One atom site from the asymmetric unit (as listed in the CIF).
struct Atom {
  std::string label;
  std::string element;
  Eigen::Vector3d frac_coords;
};

struct CrystalData {
  std::string name;
  std::string spaceGroupHm;
  UnitCell cell;
  std::vector<Atom> atoms;
};
