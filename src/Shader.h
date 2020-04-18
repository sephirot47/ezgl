#pragma once

#include "GL.h"
#include <cstdint>
#include <string>
#include <string_view>

namespace egl
{
class Shader
{
public:
  GL::Id GetGLId() const { return mGLId; }

protected:
  explicit Shader(GL::EShaderType inShaderType);
  Shader(GL::EShaderType inShaderType, const std::string_view inSourceCode);
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
  explicit VertexShader(const std::string_view inSourceCode) : Shader(GL::EShaderType::VERTEX, inSourceCode) {}
  VertexShader(VertexShader&& inRHS) noexcept = default;
};

class FragmentShader : public Shader
{
public:
  explicit FragmentShader() : Shader(GL::EShaderType::FRAGMENT) {}
  explicit FragmentShader(const std::string_view inSourceCode) : Shader(GL::EShaderType::FRAGMENT, inSourceCode) {}
  FragmentShader(FragmentShader&& inRHS) noexcept = default;
};
}
