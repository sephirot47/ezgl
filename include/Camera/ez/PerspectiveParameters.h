#pragma once

#include "ez/MathInitializers.h"
#include "ez/Vec.h"

namespace ez
{
template <typename T>
struct PerspectiveParameters final
{
  AngleRads<T> mFullAngleOfView = HalfPi<T>();
  T mAspectRatio = 1.0f;
  T mZNear = 0.05f;
  T mZFar = 1000.0f;
};

using PerspectiveParametersf = PerspectiveParameters<float>;
}