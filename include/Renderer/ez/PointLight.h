#pragma once

#include "ez/Math.h"
#include "ez/TypeTraits.h"

namespace ez
{
class PointLight
{
  Vec3f mPosition = Zero<Vec3f>();
  float mRange = 10.0f;
  Vec3f mColor = White<Vec3f>();
};

class GLSLPointLight final
{
public:
  Vec3f mPosition = Zero<Vec3f>();
  float mRange = 10.0f;
  Vec3f mColor = White<Vec3f>();

private:
  float mPadding0 = 0.0f;
};

// Traits
template <>
struct IsLight<PointLight> : std::true_type
{
};
template <>
struct IsLight<GLSLPointLight> : std::true_type
{
};
}