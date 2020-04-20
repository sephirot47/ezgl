#pragma once

#include "GL.h"
#include "GLObject.h"
#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>

namespace egl
{
class Shader
{
public:
  GL::Id GetGLId() const { return mGLId; }

protected:
  explicit Shader(const GL::EShaderType inShaderType);
  explicit Shader(const GL::EShaderType inShaderType, const std::filesystem::path& inSourceCodePath);
  explicit Shader(const GL::EShaderType inShaderType, const std::string& inSourceCode);
  Shader(const Shader& inRHS) = delete;
  Shader& operator=(const Shader& inRHS) = delete;
  Shader(Shader&& inRHS) noexcept = default;
  Shader& operator=(Shader&& inRHS) = delete;
  virtual ~Shader();

  void Compile(const std::string_view inSourceCode);

private:
  GL::Id mGLId = 0;
  GL::EShaderType mShaderType = GL::EShaderType::VERTEX;
};

class VertexShader : public Shader
{
public:
  explicit VertexShader() : Shader(GL::EShaderType::VERTEX) {}
  explicit VertexShader(const std::string& inSourceCode) : Shader(GL::EShaderType::VERTEX, inSourceCode) {}
  explicit VertexShader(const std::filesystem::path& inSourceCodePath)
      : Shader(GL::EShaderType::VERTEX, inSourceCodePath)
  {
  }
  VertexShader(VertexShader&& inRHS) noexcept = default;
};

class FragmentShader : public Shader
{
public:
  explicit FragmentShader() : Shader(GL::EShaderType::FRAGMENT) {}
  explicit FragmentShader(const std::string& inSourceCode) : Shader(GL::EShaderType::FRAGMENT, inSourceCode) {}
  explicit FragmentShader(const std::filesystem::path& inSourceCodePath)
      : Shader(GL::EShaderType::FRAGMENT, inSourceCodePath)
  {
  }
  FragmentShader(FragmentShader&& inRHS) noexcept = default;
};
}
