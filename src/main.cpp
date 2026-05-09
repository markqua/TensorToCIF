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

    const auto& uc = crystal.cell;
    std::cout << "\ncell: a,b,c [A]: " << uc.a << ' ' << uc.b << ' ' << uc.c << '\n';
    std::cout << "angles [deg]: " << uc.alpha << ' ' << uc.beta << ' '
              << uc.gamma << '\n';
    std::cout << "volume [A^3]: " << uc.volume << '\n';

    std::cout << "\nfracToCart:\n"
              << uc.fracToCart << '\n';

    const int n_show = std::min(10, static_cast<int>(crystal.atoms.size()));
    std::cout << "\nfirst " << n_show << " atoms (ASU):\n";
    for (int i = 0; i < n_show; ++i) {
      const auto& a = crystal.atoms[static_cast<std::size_t>(i)];
      std::cout << a.label << ' ' << a.element << " frac "
                << a.frac_coords.transpose() << '\n';
    }
    std::cout << "total sites: " << crystal.atoms.size() << '\n';
  }
  catch (const parser::ParseError& e) {
    std::cerr << e.what() << '\n';
    return 2;
  }

  return 0;
}
