#include <gemmi/cif.hpp>
#include <gemmi/smcif.hpp>
#include <sstream>

#include "cif/CifParser.h"

namespace parser {
namespace {

Eigen::Matrix3d adaptMat33ToEigen(const gemmi::Mat33& iMat)
{
  Eigen::Matrix3d eigenMatrix;

  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      eigenMatrix(i, j) = iMat[i][j];
    }
  }

  return eigenMatrix;
}

CrystalData adaptSmallStructureToCrystalData(const gemmi::SmallStructure& iStruct)
{
  CrystalData outCrystalData;
  outCrystalData.name = iStruct.name;

  const gemmi::UnitCell& cell = iStruct.cell;

  if (!cell.is_crystal()) {
    throw ParseError(
      outCrystalData.name.empty() ? "CIF block has no valid crystal unit cell." : "data_" + outCrystalData.name + ": no valid crystal unit cell.");
  }

  if (iStruct.sites.empty()) {
    throw ParseError(
      outCrystalData.name.empty() ? "CIF block contains no atom sites." : "data_" + outCrystalData.name + ": no atom sites.");
  }

  outCrystalData.spaceGroupHm = iStruct.spacegroup_hm;

  outCrystalData.a = cell.a;
  outCrystalData.b = cell.b;
  outCrystalData.c = cell.c;

  outCrystalData.alpha = cell.alpha;
  outCrystalData.beta = cell.beta;
  outCrystalData.gamma = cell.gamma;

  outCrystalData.volume = cell.volume;

  outCrystalData.fracToCartJac = adaptMat33ToEigen(cell.orth.mat);

  outCrystalData.atoms.reserve(iStruct.sites.size());
  for (const gemmi::SmallStructure::Site& s : iStruct.sites) {
    Atom a;

    a.label = s.label;
    a.element = s.element.name();
    a.fractionalCoords = Eigen::Vector3d(s.fract.x, s.fract.y, s.fract.z);

    outCrystalData.atoms.push_back(std::move(a));
  }

  return outCrystalData;
}

gemmi::cif::Block& selectBlock(gemmi::cif::Document& iDoc,
  const std::filesystem::path& iPath,
  const std::string& iBlockName)
{
  gemmi::cif::Block* b = iDoc.find_block(iBlockName);

  if (!b) {
    std::ostringstream msg;
    msg << iPath.string() << ": data block \"" << iBlockName << "\" not found.";

    throw ParseError(msg.str());
  }

  return *b;
}

gemmi::cif::Block& firstBlock(gemmi::cif::Document& iDoc,
  const std::filesystem::path& iPath)
{
  if (iDoc.blocks.empty()) {
    std::ostringstream msg;
    msg << iPath.string() << ": CIF contains no data blocks.";
    throw ParseError(msg.str());
  }

  return iDoc.blocks.front();
}

CrystalData parseImpl(const std::filesystem::path& iPath,
  gemmi::cif::Block& iBlock)
{
  try {
    gemmi::SmallStructure iStruct = gemmi::make_small_structure_from_block(iBlock);
    return adaptSmallStructureToCrystalData(iStruct);
  }
  catch (const ParseError&) {
    throw;
  }
  catch (const std::exception& ex) {
    std::ostringstream msg;
    msg << iPath.string();
    const std::string name = iBlock.name;
    if (!name.empty())
      msg << " (data_" << name << ")";
    msg << ": " << ex.what();
    throw ParseError(msg.str());
  }
}

} // namespace

CrystalData parseCifFile(const std::filesystem::path& iPath)
{
  try {
    gemmi::cif::Document doc = gemmi::cif::read_file(iPath.string());
    gemmi::cif::Block& block = firstBlock(doc, iPath);
    return parseImpl(iPath, block);
  }
  catch (const ParseError&) {
    throw;
  }
  catch (const std::exception& ex) {
    std::ostringstream msg;
    msg << iPath.string() << ": " << ex.what();
    throw ParseError(msg.str());
  }
}

CrystalData parseCifFile(const std::filesystem::path& iPath,
  const std::string& iBlockName)
{
  try {
    gemmi::cif::Document doc = gemmi::cif::read_file(iPath.string());
    gemmi::cif::Block& block = selectBlock(doc, iPath, iBlockName);

    return parseImpl(iPath, block);
  }
  catch (const ParseError&) {
    throw;
  }
  catch (const std::exception& ex) {
    std::ostringstream msg;
    msg << iPath.string() << ": " << ex.what();

    throw ParseError(msg.str());
  }
}

} // namespace parser
