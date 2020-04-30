#pragma once

#include <memory>

namespace egl
{
class ShaderProgram;

class ShaderProgramFactory
{
public:
  static std::shared_ptr<ShaderProgram> GetMeshShaderProgram();
  static std::shared_ptr<ShaderProgram> Get2DShaderProgram();
  static std::shared_ptr<ShaderProgram> GetOnlyColorShaderProgram();
  static std::shared_ptr<ShaderProgram> GetDrawFullScreenTextureShaderProgram();

  ShaderProgramFactory() = delete;

private:
  static std::shared_ptr<ShaderProgram> sMeshShaderProgram;
  static std::shared_ptr<ShaderProgram> s2DShaderProgram;
  static std::shared_ptr<ShaderProgram> sOnlyColorShaderProgram;
  static std::shared_ptr<ShaderProgram> sDrawFullScreenTextureShaderProgram;
};
}