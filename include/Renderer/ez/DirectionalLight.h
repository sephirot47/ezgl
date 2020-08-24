#pragma once

#include <ez/Color.h>
#include <ez/MathInitializers.h>
#include <ez/TypeTraits.h>
#include <ez/Vec.h>

namespace ez
{
class DirectionalLight
{
public:
  Vec3f mDirection = Forward<Vec3f>();
  Color3f mColor = White<Color3f>();
};

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

// Traits
template <>
struct IsLight<DirectionalLight> : std::true_type
{
};
template <>
struct IsLight<GLSLDirectionalLight> : std::true_type
{
};
}
