#pragma once

#include "Mat.h"
#include "Quat.h"
#include "Vec.h"

namespace egl
{
class Transformation
{
public:
  Transformation() = default;
  explicit Transformation(const Vec3f &inPosition);
  Transformation(const Vec3f &inPosition, const Quatf &inRotation);
  Transformation(const Vec3f &inPosition, const Quatf &inRotation, const Vec3f &inScale);

  void SetPosition(const Vec3f &inPosition);
  const Vec3f &GetPosition() const;

  void SetRotation(const Quatf &inRotation);
  const Quatf &GetRotation() const;

  void SetScale(const Vec3f &inScale);
  const Vec3f& GetScale() const;

  Vec3f Transformed(const Vec3f &inPoint);
  Vec3f InverseTransformed(const Vec3f &inPoint);
  Mat4f GetMatrix() const;
  Mat4f GetInverseMatrix() const;

private:
  Vec3f mPosition = Vec3f { 0.0f, 0.0f, 0.0f };
  Quatf mRotation = {}; // Identity
  Vec3f mScale = Vec3f { 1.0f, 1.0f, 1.0f };
};
}