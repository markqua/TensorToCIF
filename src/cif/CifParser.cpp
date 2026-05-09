#include <gemmi/cif.hpp>
#include <gemmi/smcif.hpp>
#include <sstream>

#include "cif/CifParser.h"

namespace parser {
namespace {

Eigen::Matrix3d mat33ToEigen(const gemmi::Mat33& mat)
{
  Eigen::Matrix3d e;
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      e(i, j) = mat[i][j];
    }
  }
  return e;
}

CrystalData fromSmallStructure(const gemmi::SmallStructure& st)
{
  CrystalData out;
  out.name = st.name;

  const gemmi::UnitCell& cell = st.cell;
  if (!cell.is_crystal()) {
    throw ParseError(
      out.name.empty() ? "CIF block has no valid crystal unit cell." : "data_" + out.name + ": no valid crystal unit cell.");
  }
  if (st.sites.empty()) {
    throw ParseError(
      out.name.empty() ? "CIF block contains no atom sites." : "data_" + out.name + ": no atom sites.");
  }

  out.spaceGroupHm = st.spacegroup_hm;

  out.cell.a = cell.a;
  out.cell.b = cell.b;
  out.cell.c = cell.c;
  out.cell.alpha = cell.alpha;
  out.cell.beta = cell.beta;
  out.cell.gamma = cell.gamma;
  out.cell.volume = cell.volume;
  out.cell.fracToCart = mat33ToEigen(cell.orth.mat);

  out.atoms.reserve(st.sites.size());
  for (const gemmi::SmallStructure::Site& s : st.sites) {
    Atom a;
    a.label = s.label;
    a.element = s.element.name();
    a.frac_coords = Eigen::Vector3d(s.fract.x, s.fract.y, s.fract.z);
    out.atoms.push_back(std::move(a));
  }

  return out;
}

gemmi::cif::Block& selectBlock(gemmi::cif::Document& doc,
  const std::filesystem::path& path,
  const std::string& blockName)
{
  gemmi::cif::Block* b = doc.find_block(blockName);
  if (!b) {
    std::ostringstream msg;
    msg << path.string() << ": data block \"" << blockName << "\" not found.";
    throw ParseError(msg.str());
  }
  return *b;
}

gemmi::cif::Block& firstBlock(gemmi::cif::Document& doc,
  const std::filesystem::path& path)
{
  if (doc.blocks.empty()) {
    std::ostringstream msg;
    msg << path.string() << ": CIF contains no data blocks.";
    throw ParseError(msg.str());
  }
  return doc.blocks.front();
}

CrystalData parseImpl(const std::filesystem::path& path,
  gemmi::cif::Block& block)
{
  try {
    gemmi::SmallStructure st = gemmi::make_small_structure_from_block(block);
    return fromSmallStructure(st);
  }
  catch (const ParseError&) {
    throw;
  }
  catch (const std::exception& ex) {
    std::ostringstream msg;
    msg << path.string();
    const std::string name = block.name;
    if (!name.empty())
      msg << " (data_" << name << ")";
    msg << ": " << ex.what();
    throw ParseError(msg.str());
  }
}

} // namespace

CrystalData parseCifFile(const std::filesystem::path& path)
{
  try {
    gemmi::cif::Document doc = gemmi::cif::read_file(path.string());
    gemmi::cif::Block& block = firstBlock(doc, path);
    return parseImpl(path, block);
  }
  catch (const ParseError&) {
    throw;
  }
  catch (const std::exception& ex) {
    std::ostringstream msg;
    msg << path.string() << ": " << ex.what();
    throw ParseError(msg.str());
  }
}

CrystalData parseCifFile(const std::filesystem::path& path,
  const std::string& blockName)
{
  try {
    gemmi::cif::Document doc = gemmi::cif::read_file(path.string());
    gemmi::cif::Block& block = selectBlock(doc, path, blockName);
    return parseImpl(path, block);
  }
  catch (const ParseError&) {
    throw;
  }
  catch (const std::exception& ex) {
    std::ostringstream msg;
    msg << path.string() << ": " << ex.what();
    throw ParseError(msg.str());
  }
}

} // namespace parser
