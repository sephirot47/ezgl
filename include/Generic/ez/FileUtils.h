#pragma once

#include <filesystem>
#include <string>

namespace ez
{
std::string GetFileContents(const std::filesystem::path& filepath);
}