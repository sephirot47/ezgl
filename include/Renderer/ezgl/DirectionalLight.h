#pragma once

#include "ezgl/Vec.h"
#include "ezgl/Color.h"
#include "ezgl/MathInitializers.h"

namespace egl
{
class GLSLDirectionalLight
{
public:
  Vec3f mDirection = Forward<Vec3f>();

private:
  float mPadding0 = 0.0f;

public:
  Color3f mColor = White<Color3f>();

private:
  float mPadding1 = 0.0f;
};
}
