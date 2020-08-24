#pragma once

#include <ez/Camera.h>
#include <ez/Math.h>
#include <ez/PerspectiveParameters.h>
#include <ez/Ray.h>

namespace ez
{
template <typename T>
class PerspectiveCamera final : public Camera<T, 3>
{
public:
  void SetFullAngleOfView(const AngleRads<T> inFullAngleOfView)
  {
    mPerspectiveParameters.mFullAngleOfView = inFullAngleOfView;
  }
  AngleRads<T> GetFullAngleOfView() const { return mPerspectiveParameters.mFullAngleOfView; }

  void SetAspectRatio(const float inAspectRatio) { mPerspectiveParameters.mAspectRatio = inAspectRatio; }
  float GetAspectRatio() const { return mPerspectiveParameters.mAspectRatio; }

  void SetZNear(const float inZNear) { mPerspectiveParameters.mZNear = inZNear; }
  float GetZNear() const { return mPerspectiveParameters.mZNear; }

  void SetZFar(const float inZFar) { mPerspectiveParameters.mZFar = inZFar; }
  float GetZFar() const { return mPerspectiveParameters.mZFar; }

  Ray3<T> GetViewportRay(const Vec2<T>& inViewportPoint) const; // inViewportPoint in [0..1]

  void SetPerspectiveParameters(const PerspectiveParameters<T>& inPerspectiveParameters)
  {
    mPerspectiveParameters = inPerspectiveParameters;
  }
  PerspectiveParameters<T>& GetPerspectiveParameters() { return mPerspectiveParameters; }
  const PerspectiveParameters<T>& GetPerspectiveParameters() const { return mPerspectiveParameters; }

  virtual Mat4<T> GetProjectionMatrix() const final override
  {
    const auto perspective_projection_matrix = PerspectiveMat(mPerspectiveParameters.mFullAngleOfView,
        mPerspectiveParameters.mAspectRatio,
        mPerspectiveParameters.mZNear,
        mPerspectiveParameters.mZFar);
    return perspective_projection_matrix;
  }

private:
  PerspectiveParameters<T> mPerspectiveParameters;
};

using PerspectiveCameraf = PerspectiveCamera<float>;
}

#include "ez/PerspectiveCamera.tcc"