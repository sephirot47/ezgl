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
  CameraControllerFly() = default;
  CameraControllerFly(const CameraControllerFly& inRHS) = default;
  CameraControllerFly& operator=(const CameraControllerFly& inRHS) = default;
  CameraControllerFly(CameraControllerFly&& inRHS) = default;
  CameraControllerFly& operator=(CameraControllerFly&& inRHS) = default;
  virtual ~CameraControllerFly() = default;

  void SetWindow(const std::shared_ptr<Window>& inWindow);
  void SetCamera(const std::shared_ptr<Camera<T, N>>& inCamera);

  void Update(const DeltaTime& inDeltaTime);

  virtual void OnInput(const InputEvent& inInputEvent) override;

private:
  static constexpr float InitFlySpeed = 10.0f;
  static constexpr float MinFlySpeed = 0.05f;
  static constexpr float MaxFlySpeed = 100.0f;
  static_assert(InitFlySpeed >= MinFlySpeed && InitFlySpeed <= MaxFlySpeed);

  static constexpr float AltSpeedFactor = 0.3f;
  static constexpr float ShiftSpeedFactor = 3.0f;
  static constexpr float ScrollAccelerationFactor = 0.1f;

  static constexpr float RotationSpeedFactor = 0.005f;
  static constexpr Vec2f RotationAngleYLimit = Vec2f(-QuarterCircleRads<float>(), QuarterCircleRads<float>()) * 0.99f;

  static constexpr float PanSpeed = 3.0f;

  std::weak_ptr<Camera<T, N>> mCamera;
  float mCurrentFlySpeed = InitFlySpeed;

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
  Vec2f mCurrentRotationSpeed = Zero<Vec2f>();
  Vec2f mPreviousMousePosition = Zero<Vec2f>();
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