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

private:
  Color4f mColor = White<Color4f>();
};
}