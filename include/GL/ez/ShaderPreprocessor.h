#pragma once

#include <ez/Span.h>
#include <filesystem>
#include <map>
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
      const Span<std::filesystem::path>& inIncludeDirs,
      const std::filesystem::path& inFilePath = "");

  static std::string PreprocessShaderErrorMessage(const std::string_view inErrorMessage);

  static const std::map<std::string, uint64_t>& GetFileNameToFileIdentifier() { return sFileNameToFileIdentifier; }

private:
  static std::map<std::string, uint64_t> sFileNameToFileIdentifier;
  inline static const std::string FileIdentifierToken = "474747";

  static std::string PreprocessShaderCode(const std::string_view inShaderCode,
      const Span<std::filesystem::path>& inIncludeDirs,
      std::set<std::filesystem::path>& ioAlreadyProcessedFiles,
      const std::filesystem::path& inFilePath = "");
};
}