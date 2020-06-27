#pragma once

#include "ez/Span.h"
#include <filesystem>
#include <set>
#include <string>
#include <string_view>

namespace ez
{
class ShaderPreprocessor final
{
public:
  ShaderPreprocessor() = delete;

  static std::string PreprocessShaderCode(const std::string_view inShaderCode,
      const Span<std::filesystem::path>& inIncludeDirs);

private:
  static std::string PreprocessShaderCode(const std::string_view inShaderCode,
      const Span<std::filesystem::path>& inIncludeDirs,
      std::set<std::filesystem::path>& ioAlreadyProcessedFiles);
};
}