#pragma once

#include <ez/Input.h>
#include <ez/Window.h>
#include <memory>

namespace ez
{
class InputListener
{
public:
  virtual ~InputListener();
  void ListenToInput(const std::shared_ptr<Window>& inWindow);

  // Generic input event. You can override it but it's easier with the specific ones
  virtual void OnInputEvent(const InputEvent& inInputEvent);

  virtual void OnKeyEvent(const KeyEvent&) {}
  virtual void OnMouseButtonEvent(const MouseButtonEvent&) {}
  virtual void OnMouseEnterExitEvent(const MouseEnterExitEvent&) {}
  virtual void OnMouseMoveEvent(const MouseMoveEvent&) {}
  virtual void OnMouseScrollEvent(const MouseScrollEvent&) {}

  const std::weak_ptr<Window>& GetWindow() const;

private:
  std::weak_ptr<Window> mWindow;
};
}