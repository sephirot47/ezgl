#pragma once

#include "Camera.h"
#include "OrthographicParameters.h"

namespace egl
{
class OrthographicCamera final : public Camera
{
public:
  void SetOrthoMin(const Vec3f& inOrthoMin) { GetOrthographicParameters().mMin = inOrthoMin; }
  const Vec3f& GetOrthoMin() const { return GetOrthographicParameters().mMin; }

  void SetOrthoMax(const Vec3f& inOrthoMax) { GetOrthographicParameters().mMax = inOrthoMax; }
  const Vec3f& GetOrthoMax() const { return GetOrthographicParameters().mMin; }

  void SetOrthographicParameters(const OrthographicParameters& inOrthographicParameters)
  {
    mOrthographicParameters = inOrthographicParameters;
  }
  OrthographicParameters& GetOrthographicParameters() { return mOrthographicParameters; }
  const OrthographicParameters& GetOrthographicParameters() const { return mOrthographicParameters; }

  virtual Mat4f GetProjectionMatrix() const final override;

private:
  OrthographicParameters mOrthographicParameters;
};
}