#include <algorithm>
#include <iostream>

#include "cif/CifParser.h"

int main(int argc, char* argv[])
{
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0]
              << " <file.cif> [data_block_name]\n";
    return 1;
  }

  try {
    const CrystalData crystal = (argc >= 3) ? parser::parseCifFile(argv[1], argv[2]) : parser::parseCifFile(argv[1]);

    std::cout << "block: data_" << crystal.name << "\n";
    if (!crystal.spaceGroupHm.empty())
      std::cout << "space group H-M: " << crystal.spaceGroupHm << '\n';
    std::cout << "\ncell: a,b,c [A]: " << crystal.a << ' ' << crystal.b << ' ' << crystal.c << '\n';
    std::cout << "angles [deg]: " << crystal.alpha << ' ' << crystal.beta << ' '
              << crystal.gamma << '\n';
    std::cout << "volume [A^3]: " << crystal.volume << '\n';

    std::cout << "\nfracToCart:\n"
              << crystal.fracToCartJac << '\n';

    const int n_show = std::min(10, static_cast<int>(crystal.atoms.size()));
    std::cout << "\nfirst " << n_show << " atoms (ASU):\n";
    for (int i = 0; i < n_show; ++i) {
      const auto& a = crystal.atoms[static_cast<std::size_t>(i)];
      std::cout << a.label << ' ' << a.element << " frac "
                << a.fractionalCoords.transpose() << '\n';
    }
    std::cout << "total sites: " << crystal.atoms.size() << '\n';
  }
  catch (const parser::ParseError& e) {
    std::cerr << e.what() << '\n';
    return 2;
  }

  return 0;
}
