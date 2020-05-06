#include "ez/FileUtils.h"

#include <fstream>

namespace ez
{
std::string GetFileContents(const std::filesystem::path& filepath)
{
  std::ifstream ifs(filepath.c_str());
  const std::string contents((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
  return contents;
}
}