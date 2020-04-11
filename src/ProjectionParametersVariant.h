#pragma once

#include "Math.h"
#include <variant>

namespace egl
{

struct OrthographicParameters
{
  static constexpr auto VariantIndex = 0;

  Vec3f mMin = Zero<Vec3f>();
  Vec3f mMax = One<Vec3f>();
};

struct PerspectiveParameters
{
  static constexpr auto VariantIndex = 1;

  float mAngleOfViewRads = HalfPi<float>();
  float mAspectRatio = 1.0f;
  float mZNear = 0.05f;
  float mZFar = 1.0f;
};

using ProjectionParametersVariant = std::variant<OrthographicParameters, PerspectiveParameters>;

}