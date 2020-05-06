#pragma once

#include "ez/Camera.h"
#include "ez/Math.h"
#include "ez/PerspectiveParameters.h"

namespace ez
{
template <typename T>
class PerspectiveCamera final : public Camera<T, 3>
{
public:
  void SetAngleOfView(const AngleRads inAngleOfView) { mPerspectiveParameters.mAngleOfView = inAngleOfView; }
  AngleRads GetAngleOfView() const { return mPerspectiveParameters.mAngleOfView; }

  void SetAspectRatio(const float inAspectRatio) { mPerspectiveParameters.mAspectRatio = inAspectRatio; }
  float GetAspectRatio() const { return mPerspectiveParameters.mAspectRatio; }

  void SetZNear(const float inZNear) { mPerspectiveParameters.mZNear = inZNear; }
  float GetZNear() const { return mPerspectiveParameters.mZNear; }

  void SetZFar(const float inZFar) { mPerspectiveParameters.mZFar = inZFar; }
  float GetZFar() const { return mPerspectiveParameters.mZFar; }

  void SetPerspectiveParameters(const PerspectiveParameters<T>& inPerspectiveParameters)
  {
    mPerspectiveParameters = inPerspectiveParameters;
  }
  PerspectiveParameters<T>& GetPerspectiveParameters() { return mPerspectiveParameters; }
  const PerspectiveParameters<T>& GetPerspectiveParameters() const { return mPerspectiveParameters; }

  virtual Mat4<T> GetProjectionMatrix() const final override
  {
    const auto perspective_projection_matrix = PerspectiveMat(mPerspectiveParameters.mAngleOfView,
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