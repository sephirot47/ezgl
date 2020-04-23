#pragma once

#include "Math.h"

namespace egl
{
struct OrthographicParameters
{
  static constexpr auto VariantIndex = 0;

  Vec3f mMin = Zero<Vec3f>();
  Vec3f mMax = One<Vec3f>();
};
}