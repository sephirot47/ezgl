#pragma once

#include <ez/Span.h>
#include <filesystem>
#include <memory>
#include <string_view>

namespace ez
{
class ShaderProgram;

class ShaderProgramFactory
{
public:
  static std::shared_ptr<ShaderProgram> GetMeshShaderProgram();
  static std::shared_ptr<ShaderProgram> GetTextShaderProgram();
  static std::shared_ptr<ShaderProgram> Get2DShaderProgram();
  static std::shared_ptr<ShaderProgram> Get2DTextShaderProgram();
  static std::shared_ptr<ShaderProgram> GetOnlyColorShaderProgram();
  static std::shared_ptr<ShaderProgram> GetDrawFullScreenTextureShaderProgram();

  static std::shared_ptr<ShaderProgram> CreateVertexFragmentShaderProgramFromPath(
      const std::filesystem::path& inVertexShaderPath,
      const std::filesystem::path& inFragmentShaderCode,
      const Span<std::filesystem::path>& inIncludeDirs = MakeEmptySpan<std::filesystem::path>());
  static std::shared_ptr<ShaderProgram> CreateVertexFragmentShaderProgram(const std::string_view inVertexShaderCode,
      const std::string_view inFragmentShaderCode,
      const Span<std::filesystem::path>& inIncludeDirs = MakeEmptySpan<std::filesystem::path>());

  static std::shared_ptr<ShaderProgram> CreateComputeShaderProgramFromPath(
      const std::filesystem::path& inComputeShaderPath,
      const Span<std::filesystem::path>& inIncludeDirs = MakeEmptySpan<std::filesystem::path>());
  static std::shared_ptr<ShaderProgram> CreateComputeShaderProgram(const std::string_view inComputeShaderCode,
      const Span<std::filesystem::path>& inIncludeDirs = MakeEmptySpan<std::filesystem::path>());

  ShaderProgramFactory() = delete;

private:
  static std::shared_ptr<ShaderProgram> sMeshShaderProgram;
  static std::shared_ptr<ShaderProgram> sTextShaderProgram;
  static std::shared_ptr<ShaderProgram> s2DShaderProgram;
  static std::shared_ptr<ShaderProgram> s2DTextShaderProgram;
  static std::shared_ptr<ShaderProgram> sOnlyColorShaderProgram;
  static std::shared_ptr<ShaderProgram> sDrawFullScreenTextureShaderProgram;
};
}