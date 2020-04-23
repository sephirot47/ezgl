#pragma once

#include "Math.h"

namespace egl
{
struct PerspectiveParameters
{
  static constexpr auto VariantIndex = 1;

  AngleRads mAngleOfView = HalfPi<float>();
  float mAspectRatio = 1.0f;
  float mZNear = 0.05f;
  float mZFar = 1000.0f;
};
}