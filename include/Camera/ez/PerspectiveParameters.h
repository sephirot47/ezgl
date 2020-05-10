#pragma once

#include "ez/Vec.h"
#include "ez/MathInitializers.h"

namespace ez
{
template <typename T>
struct PerspectiveParameters final
{
  AngleRads mAngleOfView = HalfPi<T>();
  T mAspectRatio = 1.0f;
  T mZNear = 0.05f;
  T mZFar = 1000.0f;
};

using PerspectiveParametersf = PerspectiveParameters<float>;
}