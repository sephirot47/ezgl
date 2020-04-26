#include "Camera.h"
#include "CameraControllerFly.h"
#include "Window.h"

namespace egl
{

template <typename T, std::size_t N>
void CameraControllerFly<T, N>::SetCamera(const std::shared_ptr<Camera<T, N>>& inCamera)
{
  mCamera = inCamera;
  if constexpr (N == 3)
  {
    if (inCamera)
    {
      mCurrentRotationAngle = YX(AngleAxis(inCamera->GetRotation()));
    }
  }
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

  // Rotation
  if constexpr (N == 3)
  {
    if (mWantsToRotate)
    {
      auto new_rotation = Identity<Quatf>();
      {
        auto current_mouse_rotation_speed = mParameters.mRotationSpeedFactor * NormalizedSafe(current_mouse_speed);

        mCurrentRotationAngle -= current_mouse_speed;
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

  // Panning
  if (mWantsToPan)
  {
    const auto pan_speed = mParameters.mPanSpeed;
    const auto camera_old_position = camera->GetPosition();

    const auto pan_displacement_local = (current_mouse_speed * pan_speed) * Vec2f(-1.0f, 1.0f);
    auto pan_displacement_local_n = Zero<Vec<T, N>>();
    pan_displacement_local_n[0] = pan_displacement_local[0];
    pan_displacement_local_n[1] = pan_displacement_local[1];
    const auto pan_displacement_world = Rotated(pan_displacement_local_n, camera->GetRotation());
    const auto camera_new_position = camera_old_position + pan_displacement_world;
    camera->SetPosition(camera_new_position);
  }

  mPreviousMousePosition = current_mouse_position;
}

template <typename T, std::size_t N>
void CameraControllerFly<T, N>::OnInput(const InputEvent& inInputEvent)
{
  const auto camera = mCamera.lock();
  if (!camera)
    return;

  switch (inInputEvent.GetType())
  {
  case InputEvent::Type::MouseScroll:
  {
    const auto& scroll_event = inInputEvent.As<InputEvent::Type::MouseScroll>();
    mCurrentFlySpeed += mParameters.mScrollAccelerationFactor * mCurrentFlySpeed * -scroll_event.mDeltaScroll[1];
    mCurrentFlySpeed = Clamp(mCurrentFlySpeed, mParameters.mMinFlySpeed, mParameters.mMaxFlySpeed);
  }
  break;

  case InputEvent::Type::Key:
  {
    const auto& key_event = inInputEvent.As<InputEvent::Type::Key>();

    mWantsToSlowDown = key_event.IsAltModifierPressed();
    mWantsToSpeedUp = key_event.IsShiftModifierPressed();
    switch (key_event.mKey)
    {
    case Key::W:
      mWantsToMoveForward = key_event.IsPressOrRepeat();
      break;
    case Key::S:
      mWantsToMoveBack = key_event.IsPressOrRepeat();
      break;
    case Key::A:
      mWantsToMoveLeft = key_event.IsPressOrRepeat();
      break;
    case Key::D:
      mWantsToMoveRight = key_event.IsPressOrRepeat();
      break;
    case Key::Q:
      mWantsToMoveUp = key_event.IsPressOrRepeat();
      break;
    case Key::E:
      mWantsToMoveDown = key_event.IsPressOrRepeat();
      break;
    default:
      break;
    }
  }
  break;

  case InputEvent::Type::MouseButton:
  {
    const auto& mouse_button_event = inInputEvent.As<InputEvent::Type::MouseButton>();

    if (mouse_button_event.mButton == MouseButton::RIGHT)
      mWantsToRotate = mouse_button_event.IsPress();

    if (mouse_button_event.mButton == MouseButton::MIDDLE)
      mWantsToPan = mouse_button_event.IsPress();

    break;
  }

  default:
    break;
  }
}
}