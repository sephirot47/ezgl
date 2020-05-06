#pragma once

#include <string>
#include <filesystem>

namespace ez
{
std::string GetFileContents(const std::filesystem::path& filepath);
}