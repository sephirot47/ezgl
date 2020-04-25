#pragma once

#include "Math.h"
#include "Transformation.h"

namespace egl
{
class Camera
{
public:
  Camera() = default;
  Camera(const Camera& inRHS) = default;
  Camera& operator=(const Camera& inRHS) = default;
  Camera(Camera&& ioRHS) = default;
  Camera& operator=(Camera&& ioRHS) = default;
  virtual ~Camera() = default;

  void SetPosition(const Vec3f& inPosition);
  const Vec3f& GetPosition() const { return mTransformation.GetPosition(); }

  void SetRotation(const Quatf& inRotation);
  const Quatf& GetRotation() const { return mTransformation.GetRotation(); }

  void LookAtPoint(const Vec3f& inPointToLookAt, const Vec3f& inUpNormalized = Up<Vec3f>());

  Vec3f GetForward() const;
  Vec3f GetRight() const;
  Vec3f GetUp() const;

  Mat4f GetModelMatrix() const;
  Mat4f GetViewMatrix() const;
  virtual Mat4f GetProjectionMatrix() const = 0;

private:
  Transformation3f mTransformation;
};
}