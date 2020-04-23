#pragma once

#include "Camera.h"
#include "PerspectiveParameters.h"

namespace egl
{
class PerspectiveCamera final : public Camera
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

  void SetPerspectiveParameters(const PerspectiveParameters& inPerspectiveParameters)
  {
    mPerspectiveParameters = inPerspectiveParameters;
  }
  PerspectiveParameters& GetPerspectiveParameters() { return mPerspectiveParameters; }
  const PerspectiveParameters& GetPerspectiveParameters() const { return mPerspectiveParameters; }

  virtual Mat4f GetProjectionMatrix() const final override;

  virtual PerspectiveCamera* Clone() const { return new PerspectiveCamera(*this); }

private:
  PerspectiveParameters mPerspectiveParameters;
};
}