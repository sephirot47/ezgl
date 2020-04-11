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
  Shader(GL::EShaderType inShaderType, const std::string_view inSourceCode);
  Shader(const Shader& inRHS) = delete;
  Shader& operator=(const Shader& inRHS) = delete;
  Shader(Shader&& inRHS) = default;
  Shader& operator=(Shader&& inRHS) = default;
  virtual ~Shader();

private:
  GL::Id mGLId = 0;
};

class VertexShader : public Shader
{
public:
  explicit VertexShader(const std::string_view inSourceCode) : Shader(GL::EShaderType::VERTEX, inSourceCode) {}
  VertexShader(const VertexShader& inRHS) = delete;
  VertexShader& operator=(const VertexShader& inRHS) = delete;
  VertexShader(VertexShader&& inRHS) = default;
  VertexShader& operator=(VertexShader&& inRHS) = default;
  ~VertexShader() override {};
};

class FragmentShader : public Shader
{
public:
  explicit FragmentShader(const std::string_view inSourceCode) : Shader(GL::EShaderType::FRAGMENT, inSourceCode) {}
  FragmentShader(const FragmentShader& inRHS) = delete;
  FragmentShader& operator=(const FragmentShader& inRHS) = delete;
  FragmentShader(FragmentShader&& inRHS) = default;
  FragmentShader& operator=(FragmentShader&& inRHS) = default;
  ~FragmentShader() override {};
};
}
