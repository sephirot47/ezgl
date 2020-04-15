#pragma once

#include "GL.h"
#include "Input.h"
#include <cstdint>
#include <functional>
#include <memory>
#include <string_view>
#include <vector>

class GLFWwindow;

namespace egl
{
struct InputListener;

class Window
{
public:
  struct CreateOptions
  {
    CreateOptions() {};

    uint32_t mWidth = 800;
    uint32_t mHeight = 800;

    std::string_view mTitle = "EZGLWindow";

    uint32_t mGLVersionMajor = 4;
    uint32_t mGLVersionMinor = 0;

    bool mResizable = true;
    bool mUseAntialiasing = true;
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

  // Input callbacks
  using InputEventCallback = std::function<void(const InputEvent&)>;
  void SetInputEventCallback(const InputEventCallback& inInputEventCallback);
  const InputEventCallback& GetInputEventCallback() const;

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

#include "Window.tcc"