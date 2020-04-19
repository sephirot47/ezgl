#include "Light.h"
#include "ShaderProgram.h"

namespace egl
{
void Light::Bind(ShaderProgram& ioShaderProgram) { }
void Light::SetColor(const Color3f& inColor) { mColor = inColor; }
const Color3f& Light::GetColor() const { return mColor; }
}