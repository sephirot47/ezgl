#pragma once

#include <chrono>
#include <ez/Camera.h>
#include <ez/InputListener.h>
#include <ez/MathInitializers.h>
#include <ez/Time.h>
#include <ez/Vec.h>
#include <ez/Window.h>
#include <memory>

namespace ez
{
template <typename T, std::size_t N>
class CameraControllerFly : public InputListener
{
public:
  struct Parameters
  {
    Parameters()
    {
      if constexpr (N == 2) // Adjust default parameters better for 2D
      {
        mMaxFlySpeed *= 30.0f;
      }
    }

    float mMinFlySpeed = 0.05f;
    float mMaxFlySpeed = 100.0f;

    float mAltSpeedFactor = 0.3f;
    float mShiftSpeedFactor = 3.0f;
    float mScrollAccelerationFactor = 0.1f;

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

  void ApplyParameters();

  // InputListener
  virtual void OnKeyEvent(const KeyEvent& inKeyEvent) override;
  virtual void OnMouseButtonEvent(const MouseButtonEvent& inMouseButtonEvent) override;
  virtual void OnMouseScrollEvent(const MouseScrollEvent& inMouseScrollEvent) override;

private:
  Parameters mParameters;
  std::weak_ptr<Camera<T, N>> mCamera;
  float mCurrentFlySpeed = (mParameters.mMinFlySpeed + mParameters.mMaxFlySpeed) * 0.5f;

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

#include "ez/CameraControllerFly.tcc"