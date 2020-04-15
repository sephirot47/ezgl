#include "CameraControllerFly.h"
#include "Camera.h"
#include "Window.h"

namespace egl
{

CameraControllerFly::~CameraControllerFly() {}

void CameraControllerFly::SetCamera(const std::shared_ptr<Camera>& inCamera)
{
  mCamera = inCamera;
  if (inCamera)
  {
    mCurrentRotationAngle = YX(AngleAxis(inCamera->GetOrientation()));
  }
}

void CameraControllerFly::SetWindow(const std::shared_ptr<Window>& inWindow) { InputListener::ListenToInput(inWindow); }

void CameraControllerFly::Update(const DeltaTime& inDeltaTime)
{
  const auto window = GetWindow().lock();
  if (!window)
    return;

  const auto camera = mCamera.lock();
  if (!camera)
    return;

  // Fly movement
  {
    auto fly_direction_local = Zero<Vec3f>();
    {
      if (mWantsToMoveForward)
        fly_direction_local += Forward<Vec3f>();
      else if (mWantsToMoveBack)
        fly_direction_local += Back<Vec3f>();

      if (mWantsToMoveRight)
        fly_direction_local += Right<Vec3f>();
      else if (mWantsToMoveLeft)
        fly_direction_local += Left<Vec3f>();

      if (mWantsToMoveDown)
        fly_direction_local += Down<Vec3f>();
      else if (mWantsToMoveUp)
        fly_direction_local += Up<Vec3f>();

      fly_direction_local = NormalizedSafe(fly_direction_local);
    }

    auto fly_speed = 0.0f;
    {
      const auto fly_speed_factor = (mWantsToSlowDown ? AltSpeedFactor : (mWantsToSpeedUp ? ShiftSpeedFactor : 1.0f));
      fly_speed = mCurrentFlySpeed * fly_speed_factor;
    }

    const auto camera_old_position = camera->GetPosition();
    const auto fly_direction_world = camera->GetOrientation() * fly_direction_local;
    const auto fly_speed_deltaed = fly_speed * inDeltaTime.count();
    const auto fly_displacement = (fly_direction_world * fly_speed_deltaed);
    const auto camera_new_position = camera_old_position + fly_displacement;
    camera->SetPosition(camera_new_position);
  }

  const auto current_mouse_position = window->GetMousePosition();
  auto current_mouse_speed = Zero<Vec2f>();
  {
    const auto mouse_position_delta = (current_mouse_position - mPreviousMousePosition);
    const auto mouse_delta_direction = NormalizedSafe(mouse_position_delta);
    current_mouse_speed = RotationSpeedFactor * mouse_position_delta;
  }

  // Rotation
  if (mWantsToRotate)
  {
    auto new_orientation = Identity<Quatf>();
    {
      mCurrentRotationAngle -= current_mouse_speed;
      mCurrentRotationAngle[1] = Clamp(mCurrentRotationAngle[1], RotationAngleYLimit[0], RotationAngleYLimit[1]);

      const auto current_rotation_angle_x = mCurrentRotationAngle[0];
      const auto current_rotation_angle_y = mCurrentRotationAngle[1];
      const auto orientation_x = AngleAxis(current_rotation_angle_x, Up<Vec3f>());
      const auto new_orientation_right = orientation_x * Right<Vec3f>();
      const auto orientation_y = AngleAxis(current_rotation_angle_y, new_orientation_right);
      new_orientation = (orientation_y * orientation_x);
    }
    camera->SetOrientation(new_orientation);
  }

  // Panning
  if (mWantsToPan)
  {
    const auto pan_speed = PanSpeed;
    const auto camera_old_position = camera->GetPosition();
    const auto pan_displacement_local = (current_mouse_speed * pan_speed) * Vec2f(-1.0f, 1.0f);
    const auto pan_displacement_world = (camera->GetOrientation() * XY0(pan_displacement_local));
    const auto camera_new_position = camera_old_position + pan_displacement_world;
    camera->SetPosition(camera_new_position);
  }

  mPreviousMousePosition = current_mouse_position;
}

void CameraControllerFly::OnInput(const InputEvent& inInputEvent)
{
  const auto camera = mCamera.lock();
  if (!camera)
    return;

  switch (inInputEvent.GetType())
  {
  case InputEvent::Type::MouseScroll:
  {
    const auto& scroll_event = inInputEvent.As<InputEvent::Type::MouseScroll>();
    mCurrentFlySpeed += ScrollAccelerationFactor * mCurrentFlySpeed * -scroll_event.mDeltaScroll[1];
    mCurrentFlySpeed = Clamp(mCurrentFlySpeed, MinFlySpeed, MaxFlySpeed);
  }
  break;

  case InputEvent::Type::Key:
  {
    const auto& key_event = inInputEvent.As<InputEvent::Type::Key>();

    mWantsToSlowDown = key_event.IsAltModifierPressed();
    mWantsToSpeedUp = key_event.IsShiftModifierPressed();
    switch (key_event.mKey)
    {
    case Key::W: mWantsToMoveForward = key_event.IsPressOrRepeat(); break;
    case Key::S: mWantsToMoveBack = key_event.IsPressOrRepeat(); break;
    case Key::A: mWantsToMoveLeft = key_event.IsPressOrRepeat(); break;
    case Key::D: mWantsToMoveRight = key_event.IsPressOrRepeat(); break;
    case Key::Q: mWantsToMoveUp = key_event.IsPressOrRepeat(); break;
    case Key::E: mWantsToMoveDown = key_event.IsPressOrRepeat(); break;
    default: break;
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

  default: break;
  }
}
}