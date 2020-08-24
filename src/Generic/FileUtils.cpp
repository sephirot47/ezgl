#include <ez/FileUtils.h>
#include <ez/Macros.h>
#include <fstream>

namespace ez
{
std::string GetFileContents(const std::filesystem::path& filepath)
{
  if (!std::filesystem::exists(filepath))
    THROW_EXCEPTION("File " << filepath << " does not exist");

  if (!std::filesystem::is_regular_file(filepath))
    THROW_EXCEPTION("File " << filepath << " is not a regular file");

  std::ifstream ifs(filepath.c_str());
  const std::string contents((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
  return contents;
}
}