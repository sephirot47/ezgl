#include "ezgl/ShaderProgramFactory.h"
#include "ezgl/Shader.h"
#include "ezgl/ShaderProgram.h"
#include <string_view>

namespace egl
{
std::shared_ptr<ShaderProgram> ShaderProgramFactory::sMeshShaderProgram;
std::shared_ptr<ShaderProgram> ShaderProgramFactory::s2DShaderProgram;
std::shared_ptr<ShaderProgram> ShaderProgramFactory::sOnlyColorShaderProgram;
std::shared_ptr<ShaderProgram> ShaderProgramFactory::sDrawFullScreenTextureShaderProgram;

std::shared_ptr<ShaderProgram> CreateShaderProgram(const std::string_view inVertexShaderCode,
    const std::string_view inFragmentShaderCode)
{
  return std::make_shared<ShaderProgram>(VertexShader(inVertexShaderCode), FragmentShader(inFragmentShaderCode));
}

std::shared_ptr<ShaderProgram> ShaderProgramFactory::GetMeshShaderProgram()
{
  if (!sMeshShaderProgram)
  {
    sMeshShaderProgram = CreateShaderProgram(
#include "/home/sephirot47/ezgl/res/Shaders/Mesh.vert"
        ,
#include "/home/sephirot47/ezgl/res/Shaders/Mesh.frag"
    );
  }
  return sMeshShaderProgram;
}

std::shared_ptr<ShaderProgram> ShaderProgramFactory::Get2DShaderProgram()
{
  if (!s2DShaderProgram)
  {
    s2DShaderProgram = CreateShaderProgram(
#include "/home/sephirot47/ezgl/res/Shaders/2D.vert"
        ,
#include "/home/sephirot47/ezgl/res/Shaders/2D.frag"
    );
  }
  return s2DShaderProgram;
}

std::shared_ptr<ShaderProgram> ShaderProgramFactory::GetOnlyColorShaderProgram()
{
  if (!sOnlyColorShaderProgram)
  {
    sOnlyColorShaderProgram = CreateShaderProgram(
#include "/home/sephirot47/ezgl/res/Shaders/OnlyColor.vert"
        ,
#include "/home/sephirot47/ezgl/res/Shaders/OnlyColor.frag"
    );
  }
  return sOnlyColorShaderProgram;
}

std::shared_ptr<ShaderProgram> ShaderProgramFactory::GetDrawFullScreenTextureShaderProgram()
{
  if (!sDrawFullScreenTextureShaderProgram)
  {
    sDrawFullScreenTextureShaderProgram = CreateShaderProgram(
#include "/home/sephirot47/ezgl/res/Shaders/DrawFullScreenTexture.vert"
        ,
#include "/home/sephirot47/ezgl/res/Shaders/DrawFullScreenTexture.frag"
    );
  }
  return sDrawFullScreenTextureShaderProgram;
}

#undef SHADER_BEGIN
#undef SHADER_END

}