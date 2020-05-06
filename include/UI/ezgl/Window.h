#pragma once

#include "ezgl/GL.h"
#include "ezgl/Input.h"
#include "ezgl/Time.h"
#include <cstdint>
#include <functional>
#include <memory>
#include <string_view>
#include <vector>

class GLFWwindow;

namespace ez
{
struct InputListener;

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

  enum class LoopResult
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

  void SetInterFrameRestTime(const TimeDuration& inInterFrameRestTime);

  Vec2i GetSize() const;
  Vec2i GetFramebufferSize() const;
  Vec2i GetPosition() const;
  float GetFramebufferAspectRatio() const;

  // Loop callback
  using LoopCallback = std::function<Window::LoopResult(const DeltaTime)>;
  void Loop(const Window::LoopCallback& inLoopCallback);

  // Input callbacks
  using InputEventCallback = std::function<void(const InputEvent&)>;
  void SetInputEventCallback(const Window::InputEventCallback& inInputEventCallback);
  const Window::InputEventCallback& GetInputEventCallback() const;

  // Input listeners
  void AddInputListener(InputListener* inInputListener);
  void RemoveInputListener(InputListener* inInputListener);
  const std::vector<InputListener*>& GetInputListeners() const;

  // Input direct
  bool IsMouseButtonPressed(const MouseButton& inMouseButton);
  bool IsKeyPressed(const Key& inKey);
  Vec2f GetMousePosition() const;

private:
  GLFWwindow* mHandle = nullptr;
  InputEventCallback mInputEventCallback;
  std::vector<InputListener*> mInputListeners;
  TimeDuration mInterFrameRestTime = 1ms;
};

class InputListener
{
public:
  virtual ~InputListener();
  void ListenToInput(const std::shared_ptr<Window>& inWindow);

  virtual void OnInput(const InputEvent& inInputEvent) = 0;
  const std::weak_ptr<Window>& GetWindow() const;

private:
  std::weak_ptr<Window> mWindow;
};

}

#include "ezgl/Window.tcc"