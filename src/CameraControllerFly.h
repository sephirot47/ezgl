#pragma once

#include "Camera.h"
#include "Math.h"
#include "Time.h"
#include "Window.h"
#include <chrono>
#include <memory>

namespace egl
{
struct InputEvent;

template <typename T, std::size_t N>
class CameraControllerFly : public InputListener
{
public:
  struct Parameters
  {
    float mInitFlySpeed = 10.0f;
    float mMinFlySpeed = 0.05f;
    float mMaxFlySpeed = 100.0f;

    float mAltSpeedFactor = 0.3f;
    float mShiftSpeedFactor = 3.0f;
    float mScrollAccelerationFactor = 0.1f;

    float mPanSpeed = 3.0f;

    // Only for 3D
    float mRotationSpeedFactor = 0.005f;
    Vec2f mRotationAngleYLimit = Vec2f(-QuarterCircleRads<float>(), QuarterCircleRads<float>()) * 0.99f;
  };

  CameraControllerFly() = default;
  CameraControllerFly(const CameraControllerFly& inRHS) = default;
  CameraControllerFly& operator=(const CameraControllerFly& inRHS) = default;
  CameraControllerFly(CameraControllerFly&& inRHS) = default;
  CameraControllerFly& operator=(CameraControllerFly&& inRHS) = default;
  virtual ~CameraControllerFly() = default;

  void SetWindow(const std::shared_ptr<Window>& inWindow);
  void SetCamera(const std::shared_ptr<Camera<T, N>>& inCamera);

  void Update(const DeltaTime& inDeltaTime);
  CameraControllerFly::Parameters& GetParameters() { return mParameters; }
  const CameraControllerFly::Parameters& GetParameters() const { return mParameters; }

  virtual void OnInput(const InputEvent& inInputEvent) override;

private:
  Parameters mParameters;
  std::weak_ptr<Camera<T, N>> mCamera;
  float mCurrentFlySpeed = mParameters.mInitFlySpeed;

  bool mWantsToMoveForward = false;
  bool mWantsToMoveBack = false;
  bool mWantsToMoveLeft = false;
  bool mWantsToMoveRight = false;
  bool mWantsToMoveUp = false;
  bool mWantsToMoveDown = false;
  bool mWantsToSlowDown = false;
  bool mWantsToSpeedUp = false;
  bool mWantsToRotate = false;
  bool mWantsToPan = false;
  Vec2f mPreviousMousePosition = Zero<Vec2f>();

  // Only for 3D
  Vec2f mCurrentRotationSpeed = Zero<Vec2f>();
  Vec2f mCurrentRotationAngle = Zero<Vec2f>();
};

template <typename T>
using CameraControllerFly2 = CameraControllerFly<T, 2>;
using CameraControllerFly2f = CameraControllerFly2<float>;

template <typename T>
using CameraControllerFly3 = CameraControllerFly<T, 3>;
using CameraControllerFly3f = CameraControllerFly3<float>;

}

#include "CameraControllerFly.tcc"