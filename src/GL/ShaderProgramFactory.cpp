#include "ez/ShaderProgramFactory.h"
#include "ez/Shader.h"
#include "ez/ShaderPreprocessor.h"
#include "ez/ShaderProgram.h"
#include <string_view>

namespace ez
{
std::shared_ptr<ShaderProgram> ShaderProgramFactory::sMeshShaderProgram;
std::shared_ptr<ShaderProgram> ShaderProgramFactory::sTextShaderProgram;
std::shared_ptr<ShaderProgram> ShaderProgramFactory::s2DShaderProgram;
std::shared_ptr<ShaderProgram> ShaderProgramFactory::s2DTextShaderProgram;
std::shared_ptr<ShaderProgram> ShaderProgramFactory::sOnlyColorShaderProgram;
std::shared_ptr<ShaderProgram> ShaderProgramFactory::sDrawFullScreenTextureShaderProgram;

std::shared_ptr<ShaderProgram> ShaderProgramFactory::CreateVertexFragmentShaderProgram(
    const std::string_view inVertexShaderCode,
    const std::string_view inFragmentShaderCode,
    const Span<std::filesystem::path>& inIncludeDirs)
{
  const auto preprocessed_vertex_shader_code
      = ShaderPreprocessor::PreprocessShaderCode(inVertexShaderCode, inIncludeDirs);
  const auto preprocessed_fragment_shader_code
      = ShaderPreprocessor::PreprocessShaderCode(inFragmentShaderCode, inIncludeDirs);
  return std::make_shared<ShaderProgram>(VertexShader { preprocessed_vertex_shader_code },
      FragmentShader { preprocessed_fragment_shader_code });
}

std::shared_ptr<ShaderProgram> ShaderProgramFactory::CreateVertexFragmentShaderProgramFromPath(
    const std::filesystem::path& inVertexShaderPath,
    const std::filesystem::path& inFragmentShaderPath,
    const Span<std::filesystem::path>& inIncludeDirs)
{
  const auto preprocessed_vertex_shader_code
      = ShaderPreprocessor::PreprocessShaderCode(GetFileContents(inVertexShaderPath),
          inIncludeDirs,
          inVertexShaderPath);
  const auto preprocessed_fragment_shader_code
      = ShaderPreprocessor::PreprocessShaderCode(GetFileContents(inFragmentShaderPath),
          inIncludeDirs,
          inFragmentShaderPath);
  return std::make_shared<ShaderProgram>(VertexShader { preprocessed_vertex_shader_code },
      FragmentShader { preprocessed_fragment_shader_code });
}

std::shared_ptr<ShaderProgram> ShaderProgramFactory::CreateComputeShaderProgramFromPath(
    const std::filesystem::path& inComputeShaderPath,
    const Span<std::filesystem::path>& inIncludeDirs)
{
  const auto preprocessed_compute_shader_code
      = ShaderPreprocessor::PreprocessShaderCode(GetFileContents(inComputeShaderPath),
          inIncludeDirs,
          inComputeShaderPath);
  return std::make_shared<ShaderProgram>(ComputeShader { preprocessed_compute_shader_code });
}

std::shared_ptr<ShaderProgram> ShaderProgramFactory::CreateComputeShaderProgram(
    const std::string_view inComputeShaderCode,
    const Span<std::filesystem::path>& inIncludeDirs)
{
  const auto preprocessed_compute_shader_code
      = ShaderPreprocessor::PreprocessShaderCode(inComputeShaderCode, inIncludeDirs);
  return std::make_shared<ShaderProgram>(ComputeShader { preprocessed_compute_shader_code });
}

std::shared_ptr<ShaderProgram> ShaderProgramFactory::GetMeshShaderProgram()
{
  if (!sMeshShaderProgram)
  {
    sMeshShaderProgram = CreateVertexFragmentShaderProgram(
#include "Shaders/Mesh.vert"
        ,
#include "Shaders/Mesh.frag"
    );
  }
  return sMeshShaderProgram;
}

std::shared_ptr<ShaderProgram> ShaderProgramFactory::GetTextShaderProgram()
{
  if (!sTextShaderProgram)
  {
    sTextShaderProgram = CreateVertexFragmentShaderProgram(
#include "Shaders/Text.vert"
        ,
#include "Shaders/Text.frag"
    );
  }
  return sTextShaderProgram;
}

std::shared_ptr<ShaderProgram> ShaderProgramFactory::Get2DShaderProgram()
{
  if (!s2DShaderProgram)
  {
    s2DShaderProgram = CreateVertexFragmentShaderProgram(
#include "Shaders/2D.vert"
        ,
#include "Shaders/2D.frag"
    );
  }
  return s2DShaderProgram;
}

std::shared_ptr<ShaderProgram> ShaderProgramFactory::Get2DTextShaderProgram()
{
  if (!s2DTextShaderProgram)
  {
    s2DTextShaderProgram = CreateVertexFragmentShaderProgram(
#include "Shaders/2D.vert"
        ,
#include "Shaders/2DText.frag"
    );
  }
  return s2DTextShaderProgram;
}

std::shared_ptr<ShaderProgram> ShaderProgramFactory::GetOnlyColorShaderProgram()
{
  if (!sOnlyColorShaderProgram)
  {
    sOnlyColorShaderProgram = CreateVertexFragmentShaderProgram(
#include "Shaders/OnlyColor.vert"
        ,
#include "Shaders/OnlyColor.frag"
    );
  }
  return sOnlyColorShaderProgram;
}

std::shared_ptr<ShaderProgram> ShaderProgramFactory::GetDrawFullScreenTextureShaderProgram()
{
  if (!sDrawFullScreenTextureShaderProgram)
  {
    sDrawFullScreenTextureShaderProgram = CreateVertexFragmentShaderProgram(
#include "Shaders/DrawFullScreenTexture.vert"
        ,
#include "Shaders/DrawFullScreenTexture.frag"
    );
  }
  return sDrawFullScreenTextureShaderProgram;
}

}