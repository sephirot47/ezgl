#pragma once

#include "Color.h"
#include "Math.h"
#include "Vec.h"

namespace egl
{
class ShaderProgram;

class Light
{
public:
  virtual void Bind(ShaderProgram& ioShaderProgram);

  void SetColor(const Color3f &inColor);
  const Color3f& GetColor() const;

private:
  Color3f mColor = White<Color3f>();
};
}