
#pragma once
#include <ez/GL.h>
#include <ez/Input.h>
#include <ez/Time.h>
#include <cstdint>
#include <functional>
#include <memory>
#include <string_view>
#include <vector>

class GLFWwindow;

namespace ez
{
class InputListener;

class Window final
{
public:
  struct CreateOptions final
  {
    CreateOptions() {};

    uint32_t mWidth = 800;
    uint32_t mHeight = 800;

    std::string_view mTitle = "EZGLWindow";

    uint32_t mGLVersionMajor = 4;
    uint32_t mGLVersionMinor = 0;

    bool mResizable = true;
    bool mUseAntialiasing = false;
    bool mDoubleBuffer = true;
  };

  enum class ECursorMode
  {
    NORMAL = GLFW_CURSOR_NORMAL,
    HIDDEN = GLFW_CURSOR_HIDDEN,
    DISABLED = GLFW_CURSOR_DISABLED,
  };

  enum class ELoopResult
  {
    KEEP_LOOPING,
    END_LOOP
  };

  Window(const Window::CreateOptions& inCreateOptions = Window::CreateOptions());
  Window(const Window& inRHS) = delete;
  Window& operator=(const Window& inRHS) = delete;
  Window(Window&& inRHS) = delete;
  Window& operator=(Window&& inRHS) = delete;
  ~Window();

  bool ShouldClose() const;
  void SwapBuffers();
  void PollEvents();

  void Maximize();
  void Minimize();

  void SetCursorMode(const Window::ECursorMode inCursorMode);
  void SetInterFrameRestTime(const Seconds& inInterFrameRestTime);

  Vec2i GetSize() const;
  Vec2i GetFramebufferSize() const;
  Vec2i GetPosition() const;
  float GetFramebufferAspectRatio() const;

  // Loop callback
  using LoopCallback = std::function<Window::ELoopResult(const DeltaTime)>;
  void Loop(const Window::LoopCallback& inLoopCallback);

  // Input listeners
  void AddInputListener(InputListener* inInputListener);
  void RemoveInputListener(InputListener* inInputListener);
  const std::vector<InputListener*>& GetInputListeners() const;

  // Input callbacks
  using InputEventCallback = std::function<void(const InputEvent&)>;
  using KeyEventCallback = std::function<void(const KeyEvent&)>;
  using MouseButtonEventCallback = std::function<void(const MouseButtonEvent&)>;
  using MouseEnterExitEventCallback = std::function<void(const MouseEnterExitEvent&)>;
  using MouseMoveEventCallback = std::function<void(const MouseMoveEvent&)>;
  using MouseScrollEventCallback = std::function<void(const MouseScrollEvent&)>;
  void AddInputEventCallback(const Window::InputEventCallback& inInputEventCallback);
  void AddKeyEventCallback(const Window::KeyEventCallback& inKeyEventCallback);
  void AddMouseButtonEventCallback(const Window::MouseButtonEventCallback& inMouseButtonEventCallback);
  void AddMouseEnterExitEventCallback(const Window::MouseEnterExitEventCallback& inMouseEnterExitEventCallback);
  void AddMouseMoveEventCallback(const Window::MouseMoveEventCallback& inMouseMoveEventCallback);
  void AddMouseScrollEventCallback(const Window::MouseScrollEventCallback& inMouseScrollEventCallback);
  const std::vector<InputEventCallback>& GetInputEventCallbacks() const;
  const std::vector<KeyEventCallback>& GetKeyEventCallbacks() const;
  const std::vector<MouseButtonEventCallback>& GetMouseButtonEventCallbacks() const;
  const std::vector<MouseEnterExitEventCallback>& GetMouseEnterExitEventCallbacks() const;
  const std::vector<MouseMoveEventCallback>& GetMouseMoveEventCallbacks() const;
  const std::vector<MouseScrollEventCallback>& GetMouseScrollEventCallbacks() const;

  // Input direct
  bool IsMouseButtonPressed(const EMouseButton& inMouseButton);
  bool IsKeyPressed(const EKey& inKey);
  Vec2f GetMousePosition() const;
  Vec2f GetMousePositionViewport() const; // [0..1]

private:
  GLFWwindow* mHandle = nullptr;
  Seconds mInterFrameRestTime = 1ms;

  // InputListeners
  std::vector<InputListener*> mInputListeners;

  // Input callbacks
  std::vector<InputEventCallback> mInputEventCallbacks;
  std::vector<KeyEventCallback> mKeyEventCallbacks;
  std::vector<MouseButtonEventCallback> mMouseButtonEventCallbacks;
  std::vector<MouseEnterExitEventCallback> mMouseEnterExitEventCallbacks;
  std::vector<MouseMoveEventCallback> mMouseMoveEventCallbacks;
  std::vector<MouseScrollEventCallback> mMouseScrollEventCallbacks;
};

}

#include "ez/Window.tcc"