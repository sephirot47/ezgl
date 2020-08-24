#include <ez/Camera.h>
#include <ez/CameraControllerFly.h>
#include <ez/Window.h>

namespace ez
{

template <typename T, std::size_t N>
void CameraControllerFly<T, N>::SetCamera(const std::shared_ptr<Camera<T, N>>& inCamera)
{
  mCamera = inCamera;
  ApplyParameters();
}

template <typename T, std::size_t N>
void CameraControllerFly<T, N>::SetWindow(const std::shared_ptr<Window>& inWindow)
{
  InputListener::ListenToInput(inWindow);
}

template <typename T, std::size_t N>
void CameraControllerFly<T, N>::Update(const DeltaTime& inDeltaTime)
{
  const auto window = GetWindow().lock();
  if (!window)
    return;

  const auto camera = mCamera.lock();
  if (!camera)
    return;

  // Fly movement
  {
    auto fly_direction_local = Zero<Vec<T, N>>();
    {
      if (mWantsToMoveRight)
        fly_direction_local += Right<Vec<T, N>>();

      if (mWantsToMoveLeft)
        fly_direction_local += Left<Vec<T, N>>();

      if constexpr (N == 3)
      {
        if (mWantsToMoveForward)
          fly_direction_local += Forward<Vec<T, N>>();

        if (mWantsToMoveBack)
          fly_direction_local += Back<Vec<T, N>>();

        if (mWantsToMoveDown)
          fly_direction_local += Down<Vec<T, N>>();

        if (mWantsToMoveUp)
          fly_direction_local += Up<Vec<T, N>>();
      }
      else if constexpr (N == 2)
      {
        if (mWantsToMoveBack)
          fly_direction_local += Down<Vec<T, N>>();

        if (mWantsToMoveForward)
          fly_direction_local += Up<Vec<T, N>>();
      }

      fly_direction_local = NormalizedSafe(fly_direction_local);
    }

    auto fly_speed = 0.0f;
    {
      const auto fly_speed_factor
          = (mWantsToSlowDown ? mParameters.mAltSpeedFactor : (mWantsToSpeedUp ? mParameters.mShiftSpeedFactor : 1.0f));
      fly_speed = mCurrentFlySpeed * fly_speed_factor;
    }

    const auto camera_old_position = camera->GetPosition();
    const auto fly_direction_world = Rotated(fly_direction_local, camera->GetRotation());
    const auto fly_speed_deltaed = fly_speed * inDeltaTime.count();
    const auto fly_displacement = (fly_direction_world * fly_speed_deltaed);
    const auto camera_new_position = camera_old_position + fly_displacement;
    camera->SetPosition(camera_new_position);
  }

  const auto current_mouse_position = window->GetMousePosition();
  auto current_mouse_speed = Zero<Vec2f>();
  {
    const auto mouse_position_delta = (current_mouse_position - mPreviousMousePosition);
    current_mouse_speed = mouse_position_delta;
  }

  // Rotation, only for 3D
  if constexpr (N == 3)
  {
    if (mWantsToRotate)
    {
      auto new_rotation = Identity<Quatf>();
      {
        auto current_mouse_rotation_speed = mParameters.mRotationSpeedFactor * current_mouse_speed;

        mCurrentRotationAngle -= current_mouse_rotation_speed;
        mCurrentRotationAngle[1]
            = Clamp(mCurrentRotationAngle[1], mParameters.mRotationAngleYLimit[0], mParameters.mRotationAngleYLimit[1]);

        const auto current_rotation_angle_x = mCurrentRotationAngle[0];
        const auto current_rotation_angle_y = mCurrentRotationAngle[1];
        const auto rotation_x = AngleAxis(current_rotation_angle_x, Up<Vec<T, N>>());
        const auto new_rotation_right = rotation_x * Right<Vec<T, N>>();
        const auto rotation_y = AngleAxis(current_rotation_angle_y, new_rotation_right);
        new_rotation = (rotation_y * rotation_x);
      }
      camera->SetRotation(new_rotation);
    }
  }

  // Panning, only for 2D
  if constexpr (N == 2)
  {
    if (mWantsToPan)
    {
      const auto pan_displacement_world = current_mouse_speed * Vec2f(-1.0f, 1.0f);
      const auto camera_old_position = camera->GetPosition();
      const auto camera_new_position = camera_old_position + pan_displacement_world;

      camera->SetPosition(camera_new_position);
    }
  }

  mPreviousMousePosition = current_mouse_position;
}

template <typename T, std::size_t N>
void CameraControllerFly<T, N>::ApplyParameters()
{
  mCurrentFlySpeed = (mParameters.mMinFlySpeed + mParameters.mMaxFlySpeed) * 0.5f;

  if constexpr (N == 3)
  {
    if (const auto camera = mCamera.lock())
    {
      mCurrentRotationAngle = YX(AngleAxis(camera->GetRotation()));
    }
  }
}

template <typename T, std::size_t N>
void CameraControllerFly<T, N>::OnKeyEvent(const KeyEvent& inKeyEvent)
{
  mWantsToSlowDown = inKeyEvent.IsAltModifierPressed();
  mWantsToSpeedUp = inKeyEvent.IsShiftModifierPressed();

  switch (inKeyEvent.mKey)
  {
  case EKey::W:
    mWantsToMoveForward = inKeyEvent.IsPressOrRepeat();
    break;
  case EKey::S:
    mWantsToMoveBack = inKeyEvent.IsPressOrRepeat();
    break;
  case EKey::A:
    mWantsToMoveLeft = inKeyEvent.IsPressOrRepeat();
    break;
  case EKey::D:
    mWantsToMoveRight = inKeyEvent.IsPressOrRepeat();
    break;
  case EKey::Q:
    mWantsToMoveUp = inKeyEvent.IsPressOrRepeat();
    break;
  case EKey::E:
    mWantsToMoveDown = inKeyEvent.IsPressOrRepeat();
    break;
  default:
    break;
  }
}

template <typename T, std::size_t N>
void CameraControllerFly<T, N>::OnMouseButtonEvent(const MouseButtonEvent& inMouseButtonEvent)
{
  if (inMouseButtonEvent.mButton == EMouseButton::RIGHT)
    mWantsToRotate = inMouseButtonEvent.IsPress();

  if (inMouseButtonEvent.mButton == EMouseButton::MIDDLE)
    mWantsToPan = inMouseButtonEvent.IsPress();
}

template <typename T, std::size_t N>
void CameraControllerFly<T, N>::OnMouseScrollEvent(const MouseScrollEvent& inMouseScrollEvent)
{
  mCurrentFlySpeed += mParameters.mScrollAccelerationFactor * mCurrentFlySpeed * -inMouseScrollEvent.mDeltaScroll[1];
  mCurrentFlySpeed = Clamp(mCurrentFlySpeed, mParameters.mMinFlySpeed, mParameters.mMaxFlySpeed);
}
}