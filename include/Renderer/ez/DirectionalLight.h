#pragma once

#include "ez/Vec.h"
#include "ez/Color.h"
#include "ez/MathInitializers.h"

namespace ez
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
