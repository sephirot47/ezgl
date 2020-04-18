#include "Window.h"
#include "Macros.h"
#include <GL/glew.h>
#include <algorithm>
#include <thread>

namespace egl
{

template <typename TInputEvent>
void CallInputEventCallback(GLFWwindow* inGLFWWindow, const TInputEvent& inInputEvent);
void GLFWMouseButtonCallback(GLFWwindow* inGLFWWindow, int inButton, int inAction, int inModifiers);
void GLFWMouseMoveCallback(GLFWwindow* inGLFWWindow, double inPosX, double inPosY);
void GLFWMouseEnterExitCallback(GLFWwindow* inGLFWWindow, int inMouseEntered);
void GLFWMouseScrollCallback(GLFWwindow* inGLFWWindow, double inDeltaScrollX, double inDeltaScrollY);
void GLFWKeyCallback(GLFWwindow* inGLFWWindow, int inKey, int inScancode, int inAction, int inModifiers);

// During init, enable debug output
Window::Window(const Window::CreateOptions& inCreateOptions)
{
  if (!glfwInit())
    THROW_EXCEPTION("Error initiating GLFW");

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, inCreateOptions.mGLVersionMajor);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, inCreateOptions.mGLVersionMinor);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, false);

  if (inCreateOptions.mResizable)
    glfwWindowHint(GLFW_RESIZABLE, true);

  if (inCreateOptions.mUseAntialiasing)
    glfwWindowHint(GLFW_SAMPLES, 4);

  mHandle = glfwCreateWindow(inCreateOptions.mWidth,
      inCreateOptions.mHeight,
      inCreateOptions.mTitle.data(),
      nullptr,
      nullptr);
  if (!mHandle)
    THROW_EXCEPTION("Error creating GLFW window");

  glfwMakeContextCurrent(mHandle);

  glewExperimental = GL_TRUE;
  const auto glew_init_error = glewInit();
  if (glew_init_error != GLEW_OK)
    THROW_EXCEPTION("Error initiating GLEW");

  if (inCreateOptions.mUseAntialiasing)
  {
    GL::Enable(GL::Enablable::LINE_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
  }

  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(&GL::ErrorCallback, 0);

  glfwSetWindowUserPointer(mHandle, this);

  // Set input callbacks
  glfwSetMouseButtonCallback(mHandle, &GLFWMouseButtonCallback);
  glfwSetCursorPosCallback(mHandle, &GLFWMouseMoveCallback);
  glfwSetCursorEnterCallback(mHandle, &GLFWMouseEnterExitCallback);
  glfwSetScrollCallback(mHandle, &GLFWMouseScrollCallback);
  glfwSetKeyCallback(mHandle, &GLFWKeyCallback);
}

Window::~Window()
{
  glfwDestroyWindow(mHandle);
  glfwTerminate();
}

bool Window::ShouldClose() const { return glfwWindowShouldClose(mHandle); }

void Window::SwapBuffers() { glfwSwapBuffers(mHandle); }

void Window::PollEvents() { glfwPollEvents(); }

void Window::SetInterFrameRestTime(const TimeDuration& inInterFrameRestTime)
{
  mInterFrameRestTime = inInterFrameRestTime;
}

Vec2i Window::GetSize() const
{
  Vec2i size;
  glfwGetWindowSize(mHandle, &size[0], &size[1]);
  return size;
}

Vec2i Window::GetFramebufferSize() const
{
  Vec2i framebuffer_size;
  glfwGetFramebufferSize(mHandle, &framebuffer_size[0], &framebuffer_size[1]);
  return framebuffer_size;
}

float Window::GetFramebufferAspectRatio() const
{
  const auto framebuffer_size = GetFramebufferSize();
  return static_cast<float>(framebuffer_size[0]) / framebuffer_size[1];
}

Vec2i Window::GetPosition() const
{
  Vec2i position;
  glfwGetWindowPos(mHandle, &position[0], &position[1]);
  return position;
}

void Window::Loop(const Window::LoopCallback& inLoopCallback)
{
  auto previous_time = Now();
  while (!ShouldClose())
  {
    const auto delta_time = (Now() - previous_time);
    previous_time = Now();
    inLoopCallback(delta_time);

    SwapBuffers();
    PollEvents();

    std::this_thread::sleep_for(mInterFrameRestTime);
  }
}

void Window::SetInputEventCallback(const Window::InputEventCallback& inInputEventCallback)
{
  mInputEventCallback = inInputEventCallback;
}
const Window::InputEventCallback& Window::GetInputEventCallback() const { return mInputEventCallback; }

bool Window::IsMouseButtonPressed(const MouseButton& inMouseButton)
{
  return glfwGetMouseButton(mHandle, static_cast<int>(inMouseButton)) == GLFW_PRESS;
}

bool Window::IsKeyPressed(const Key& inKey) { return glfwGetKey(mHandle, static_cast<int>(inKey)) == GLFW_PRESS; }

Vec2f Window::GetMousePosition() const
{
  double mouse_position_x = 0.0;
  double mouse_position_y = 0.0;
  glfwGetCursorPos(mHandle, &mouse_position_x, &mouse_position_y);
  const auto mouse_position = Vec2f(static_cast<float>(mouse_position_x), static_cast<float>(mouse_position_y));
  return mouse_position;
}

template <typename TInputEvent>
void CallInputEventCallback(GLFWwindow* inGLFWWindow, const TInputEvent& inInputEvent)
{
  const Window& window = *(reinterpret_cast<Window*>(glfwGetWindowUserPointer(inGLFWWindow)));
  if (const auto& input_event_callback = window.GetInputEventCallback())
    input_event_callback({ inInputEvent });

  for (auto& input_listener : window.GetInputListeners())
    input_listener->OnInput({ inInputEvent });
}

void Window::AddInputListener(InputListener* inInputListener) { mInputListeners.push_back(inInputListener); }
void Window::RemoveInputListener(InputListener* inInputListener)
{
  PEEK(inInputListener);
  mInputListeners.erase(std::find(mInputListeners.cbegin(), mInputListeners.cend(), inInputListener));
  PEEK(mInputListeners.size());
}
const std::vector<InputListener*>& Window::GetInputListeners() const { return mInputListeners; }

void GLFWMouseButtonCallback(GLFWwindow* inGLFWWindow, int inButton, int inAction, int inModifiers)
{
  MouseButtonEvent mouse_button_event;
  mouse_button_event.mButton = static_cast<MouseButton>(inButton);
  mouse_button_event.mAction = static_cast<MouseAction>(inAction);
  mouse_button_event.mModifiers = static_cast<ModifierKey>(inModifiers);
  CallInputEventCallback(inGLFWWindow, mouse_button_event);
}

void GLFWMouseMoveCallback(GLFWwindow* inGLFWWindow, double inPosX, double inPosY)
{
  MouseMoveEvent mouse_move_event;
  mouse_move_event.mPosition = Vec2f(static_cast<float>(inPosX), static_cast<float>(inPosY));
  CallInputEventCallback(inGLFWWindow, mouse_move_event);
}

void GLFWMouseEnterExitCallback(GLFWwindow* inGLFWWindow, int inMouseEntered)
{
  MouseEnterExitEvent mouse_enter_exit_event;
  mouse_enter_exit_event.mEntered = (inMouseEntered == GLFW_TRUE);
  CallInputEventCallback(inGLFWWindow, mouse_enter_exit_event);
}

void GLFWMouseScrollCallback(GLFWwindow* inGLFWWindow, double inDeltaScrollX, double inDeltaScrollY)
{
  MouseScrollEvent mouse_scroll_event;
  mouse_scroll_event.mDeltaScroll = Vec2f(static_cast<float>(inDeltaScrollX), static_cast<float>(inDeltaScrollY));
  CallInputEventCallback(inGLFWWindow, mouse_scroll_event);
}

void GLFWKeyCallback(GLFWwindow* inGLFWWindow, int inKey, int inScancode, int inAction, int inModifiers)
{
  UNUSED(inScancode);
  KeyEvent key_event;
  key_event.mKey = static_cast<Key>(inKey);
  key_event.mAction = static_cast<KeyAction>(inAction);
  key_event.mModifiers = static_cast<ModifierKey>(inModifiers);
  CallInputEventCallback(inGLFWWindow, key_event);
};

InputListener::~InputListener()
{
  if (const auto window = mWindow.lock())
    window->RemoveInputListener(this);
}

void InputListener::ListenToInput(const std::shared_ptr<Window>& inWindow)
{
  EXPECTS(inWindow);
  mWindow = inWindow;

  inWindow->AddInputListener(this);
}

const std::weak_ptr<Window>& InputListener::GetWindow() const { return mWindow; }

}