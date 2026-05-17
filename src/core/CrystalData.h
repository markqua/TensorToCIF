#pragma once

#include <Eigen/Dense>
#include <string>
#include <vector>

// One atom site from the asymmetric unit (as listed in the CIF).
struct Atom {
  std::string label;
  std::string element;

  Eigen::Vector3d fractionalCoords;
};

struct CrystalData {
  std::string name;
  std::string spaceGroupHm;

  // Unit-cell parameters with fractional Cartesian transforms (lengths in angstrom).
  double a = 1.0;
  double b = 1.0;
  double c = 1.0;

  // Angles in degrees.
  double alpha = 90.0;
  double beta = 90.0;
  double gamma = 90.0;

  // Cartesian = fracToCart * frac_coords (Eigen column vectors, angstrom).
  Eigen::Matrix3d fracToCartJac;
  double volume = 1.0;

  // Pressure in GPa
  // Temperature in K
  double pressure = 0.00001;
  double temperature = 297;

  std::vector<Atom> atoms;
};
