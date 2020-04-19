#pragma once

#include "Math.h"

namespace egl
{
class GLSLPointLight
{
  public:
  Vec3f mPosition = Zero<Vec3f>();
  float mRange = 10.0f;

  public:
  Vec3f mColor = White<Vec3f>();

  private:
  float mPadding0 = 0.0f;
};
}