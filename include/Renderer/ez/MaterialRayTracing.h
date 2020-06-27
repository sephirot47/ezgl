#pragma once

#include "ez/Color.h"

namespace ez
{
class MaterialRayTracing
{
public:
  MaterialRayTracing() = default;
  explicit MaterialRayTracing(const Color4f& inColor) : mColor { inColor } {}

  void SetColor(const Color4f& inColor) { mColor = inColor; }
  const Color4f& GetColor() const { return mColor; }

  Vec3f GetReflectDirection(const Vec3f& inIncidentLight, const Vec3f& inNormal) const
  {
    // BRDF
    EXPECTS(IsNormalized(inIncidentLight));
    EXPECTS(IsNormalized(inNormal));
    const auto perfect_reflect = Reflect(inIncidentLight, inNormal);
    const auto dot = Max(Dot(inIncidentLight, inNormal), 0.0f);
    const auto factor = std::pow(dot, 3.0f);
    const auto reflect = NormalizedSafe((perfect_reflect * factor) + (inNormal * (1.0f - factor)));
    return reflect;
  }

private:
  Color4f mColor = White<Color4f>();
};
}