#pragma once

#include "ezgl/Vec.h"
#include "ezgl/MathInitializers.h"

namespace egl
{
template <typename T>
struct PerspectiveParameters final
{
  static constexpr auto VariantIndex = 1;

  AngleRads mAngleOfView = HalfPi<T>();
  T mAspectRatio = 1.0f;
  T mZNear = 0.05f;
  T mZFar = 1000.0f;
};

using PerspectiveParametersf = PerspectiveParameters<float>;
}