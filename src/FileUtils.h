#pragma once

#include <string>
#include <filesystem>

namespace egl
{
std::string GetFileContents(const std::filesystem::path& filepath);
}