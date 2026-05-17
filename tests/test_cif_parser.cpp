#include <catch2/catch_test_macros.hpp>
#include <cif/CifParser.h>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

#ifndef TTC_SAMPLES_DIR
  #error "tests must be built with TTC_SAMPLES_DIR defined"
#endif

static fs::path samplePath(const std::string& filename)
{
  return fs::path{TTC_SAMPLES_DIR} / filename;
}

static void requireReasonableCrystal(const CrystalData& c)
{
  REQUIRE_FALSE(c.atoms.empty());
  REQUIRE(c.a > 1e-3);
  REQUIRE(c.b > 1e-3);
  REQUIRE(c.c > 1e-3);
  REQUIRE(c.volume > 1e-6);
  REQUIRE_FALSE((c.fracToCartJac.array().isNaN().any()));
  REQUIRE_FALSE((c.fracToCartJac.array().isInf().any()));
  REQUIRE(std::abs(c.fracToCartJac.determinant()) > 1e-18);

  for (const Atom& a : c.atoms) {
    REQUIRE_FALSE(a.label.empty());
    REQUIRE_FALSE(a.element.empty());
  }
}

TEST_CASE("sample 103001.cif parses via first block", "[parser]")
{
  const CrystalData c = parser::parseCifFile(samplePath("103001.cif"));
  REQUIRE(c.name == "global");
  REQUIRE(c.atoms.size() == 23);
  requireReasonableCrystal(c);
}

TEST_CASE("sample 103001.cif parses via explicit block name", "[parser]")
{
  const CrystalData c =
    parser::parseCifFile(samplePath("103001.cif"), "global");
  requireReasonableCrystal(c);
}

TEST_CASE("sample 1488011.cif parses via first block", "[parser]")
{
  const CrystalData c = parser::parseCifFile(samplePath("1488011.cif"));
  REQUIRE_FALSE(c.name.empty());
  requireReasonableCrystal(c);
}

TEST_CASE("sample 1488011.cif parses named block", "[parser]")
{
  const CrystalData c = parser::parseCifFile(
    samplePath("1488011.cif"), "XU1113_6_CORR_phase_1");
  REQUIRE(c.name == "XU1113_6_CORR_phase_1");
  REQUIRE(c.atoms.size() == 3);
  requireReasonableCrystal(c);
}

TEST_CASE("unknown block name throws ParseError", "[parser]")
{
  REQUIRE_THROWS_AS(parser::parseCifFile(samplePath("103001.cif"), "no_such_block"),
    parser::ParseError);
}
