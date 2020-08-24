#include "ez/InputListener.h"

namespace ez
{
InputListener::~InputListener()
{
  if (const auto window = mWindow.lock())
    window->RemoveInputListener(this);
}

void InputListener::OnInputEvent(const InputEvent& inInputEvent)
{
  switch (inInputEvent.GetType())
  {
  case InputEvent::EType::KEY:
    OnKeyEvent(inInputEvent.As<InputEvent::EType::KEY>());
    break;

  case InputEvent::EType::MOUSE_BUTTON:
    OnMouseButtonEvent(inInputEvent.As<InputEvent::EType::MOUSE_BUTTON>());
    break;

  case InputEvent::EType::MOUSE_ENTER_EXIT:
    OnMouseEnterExitEvent(inInputEvent.As<InputEvent::EType::MOUSE_ENTER_EXIT>());
    break;

  case InputEvent::EType::MOUSE_MOVE:
    OnMouseMoveEvent(inInputEvent.As<InputEvent::EType::MOUSE_MOVE>());
    break;

  case InputEvent::EType::MOUSE_SCROLL:
    OnMouseScrollEvent(inInputEvent.As<InputEvent::EType::MOUSE_SCROLL>());
    break;
  }
}

void InputListener::ListenToInput(const std::shared_ptr<Window>& inWindow)
{
  EXPECTS(inWindow);
  mWindow = inWindow;

  inWindow->AddInputListener(this);
}

const std::weak_ptr<Window>& InputListener::GetWindow() const { return mWindow; }
}