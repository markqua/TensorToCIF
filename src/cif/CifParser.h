#pragma once

#include <filesystem>
#include <stdexcept>
#include <string>

#include "core/CrystalData.h"

namespace parser {

// Exception thrown when CIF parsing or validation fails.
struct ParseError : std::runtime_error {
  using std::runtime_error::runtime_error;
};

//  Parses the first `data_` block in the file into a crystal description.
CrystalData parseCifFile(const std::filesystem::path& path);

//  Parses a specific block by name (without the `data_` prefix).
CrystalData parseCifFile(const std::filesystem::path& path,
  const std::string& blockName);

} // namespace parser
