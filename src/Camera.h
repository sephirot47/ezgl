#pragma once

#include "Math.h"
#include "Transformation.h"
#include "ProjectionParametersVariant.h"

namespace egl
{
class Camera
{
public:
  Camera();
  Camera(const Camera& inRHS) = default;
  Camera& operator=(const Camera& inRHS) = default;
  Camera(Camera&& ioRHS) = default;
  Camera& operator=(Camera&& ioRHS) = default;
  ~Camera() = default;

  void SetPosition(const Vec3f& inPosition);
  void SetRotation(const Quatf& inRotation);
  void SetPerspectiveParameters(const PerspectiveParameters& inPerspectiveParameters);
  void SetOrthographicParameters(const OrthographicParameters& inOrthographicParameters);

  void LookAtPoint(const Vec3f& inPointToLookAt, const Vec3f& inUpNormalized = Up<Vec3f>());

  const Vec3f& GetPosition() const;
  const Quatf& GetRotation() const;
  Vec3f GetForward() const;
  Vec3f GetRight() const;
  Vec3f GetUp() const;

  bool IsOrthographic() const;
  bool IsPerspective() const;
  OrthographicParameters& GetOrthographicParameters();
  PerspectiveParameters& GetPerspectiveParameters();
  const OrthographicParameters& GetOrthographicParameters() const;
  const PerspectiveParameters& GetPerspectiveParameters() const;

  Mat4f GetModelMatrix() const;
  Mat4f GetViewMatrix() const;
  Mat4f GetProjectionMatrix() const;

private:
  Transformation mTransformation;
  ProjectionParametersVariant mProjectionParametersVariant;
};
}